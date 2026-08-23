#ifndef OCI_BRIDGE_H
#define OCI_BRIDGE_H

/*
 * oci_bridge.h — Bridge layer between 4D Plugin API and Oracle OCI
 *
 * Provides:
 *   - Handle table: maps PA_long32 IDs ↔ void* OCI pointers
 *   - String conversion: PA_Unistring (UTF-16) ↔ OraText (UTF-8)
 *   - Pointer param helpers: read/write 4D variables via PA_Variable
 *   - Error callback support
 */

#include "4DPluginAPI.h"
#include <oci.h>

#include <string>
#include <vector>
#include <unordered_map>
#include <mutex>
#include <cstring>
#include <cstdlib>

// ============================================================
// Handle Table
// ============================================================
// OCI pointers are 8 bytes on 64-bit but PA_long32 is 4 bytes.
// We map small integer IDs (1, 2, 3, ...) to actual OCI pointers.

class OCIHandleTable {
public:
    static OCIHandleTable& instance() {
        static OCIHandleTable table;
        return table;
    }

    // Register a new OCI pointer, returns its integer ID
    PA_long32 add(void* ptr) {
        std::lock_guard<std::mutex> lock(m_mutex);
        PA_long32 id = m_nextId++;
        m_handles[id] = ptr;
        return id;
    }

    // Look up an OCI pointer by its integer ID (returns nullptr if not found)
    void* get(PA_long32 id) const {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto it = m_handles.find(id);
        return (it != m_handles.end()) ? it->second : nullptr;
    }

    // Remove a handle by its integer ID, returns the pointer (for freeing)
    void* remove(PA_long32 id) {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto it = m_handles.find(id);
        if (it == m_handles.end()) return nullptr;
        void* ptr = it->second;
        m_handles.erase(it);
        return ptr;
    }

    // Clear all handles (used during plugin deinit)
    void clear() {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_handles.clear();
        m_nextId = 1;
    }

    // Typed convenience getters
    template<typename T>
    T* getAs(PA_long32 id) const {
        return static_cast<T*>(get(id));
    }

private:
    OCIHandleTable() : m_nextId(1) {}
    mutable std::mutex m_mutex;
    std::unordered_map<PA_long32, void*> m_handles;
    PA_long32 m_nextId;
};

// Shorthand
inline OCIHandleTable& handles() { return OCIHandleTable::instance(); }

// ============================================================
// String Conversion: PA_Unistring (UTF-16) ↔ std::string (UTF-8)
// ============================================================

// Convert PA_Unistring* (UTF-16) to std::string (UTF-8)
inline std::string unistr_to_utf8(PA_Unistring* ustr) {
    if (!ustr || ustr->fLength == 0) return std::string();

    PA_long32 srcLen = ustr->fLength * sizeof(PA_Unichar);
    PA_long32 dstLen = ustr->fLength * 4 + 1; // worst case UTF-8
    std::vector<char> buf(dstLen, 0);

    PA_ConvertCharsetToCharset(
        (char*)ustr->fString, srcLen, eVTC_UTF_16,
        buf.data(), dstLen, eVTC_UTF_8
    );

    return std::string(buf.data());
}

// Convert UTF-8 C string to PA_Unichar buffer (UTF-16)
// Caller must ensure dst has room for at least maxChars+1 PA_Unichar
inline PA_long32 utf8_to_unichar(const char* src, PA_long32 srcLen,
                                  PA_Unichar* dst, PA_long32 maxChars) {
    if (!src || srcLen == 0) {
        if (dst) dst[0] = 0;
        return 0;
    }
    PA_long32 dstBytes = maxChars * sizeof(PA_Unichar);
    std::memset(dst, 0, dstBytes);

    PA_ConvertCharsetToCharset(
        (char*)src, srcLen, eVTC_UTF_8,
        (char*)dst, dstBytes, eVTC_UTF_16
    );

    // Find actual length
    PA_long32 len = 0;
    while (len < maxChars && dst[len] != 0) len++;
    return len;
}

// Convert UTF-8 std::string to PA_Unistring (caller must PA_DisposeUnistring)
inline PA_Unistring utf8_to_unistring(const std::string& s) {
    if (s.empty()) {
        return PA_CreateUnistring(nullptr);
    }
    PA_long32 maxChars = (PA_long32)s.size() + 1;
    std::vector<PA_Unichar> buf(maxChars + 1, 0);
    PA_long32 len = utf8_to_unichar(s.c_str(), (PA_long32)s.size(), buf.data(), maxChars);
    buf[len] = 0;
    return PA_CreateUnistring(buf.data());
}

// ============================================================
// Pointer Parameter Helpers
// ============================================================
// For &Z params that point to Text variables, read/write the pointed-to variable

// Write a UTF-8 string to a 4D Text variable referenced by a pointer parameter (&Z)
inline void set_pointer_text(PA_PluginParameters params, short index,
                              const char* utf8, PA_long32 utf8Len) {
    PA_Pointer ptr = PA_GetPointerParameter(params, index);
    if (!ptr) return;

    PA_Unistring ustr = utf8_to_unistring(std::string(utf8, utf8Len));

    PA_Variable var;
    std::memset(&var, 0, sizeof(var));
    var.fType = (char)eVK_Text;
    var.uValue.fString = ustr;

    // After PA_SetPointerValue, 4D owns the variable content — do NOT dispose
    PA_SetPointerValue(ptr, var);
}

// Write a UTF-8 std::string to a 4D Text variable
inline void set_pointer_text(PA_PluginParameters params, short index,
                              const std::string& str) {
    set_pointer_text(params, index, str.c_str(), (PA_long32)str.size());
}

// ============================================================
// Error Callback Support
// ============================================================

class OCIErrorCallback {
public:
    static OCIErrorCallback& instance() {
        static OCIErrorCallback cb;
        return cb;
    }

    void setMethod(const std::string& methodName) {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_methodName = methodName;
    }

    const std::string& getMethod() const {
        return m_methodName;
    }

    bool hasMethod() const {
        return !m_methodName.empty();
    }

    // Call the 4D error method if registered
    // The error method receives no parameters — it's a simple notification
    void invoke() const {
        if (!hasMethod()) return;
        PA_Unistring name = utf8_to_unistring(m_methodName);
        PA_ExecuteMethodByID(PA_GetMethodID(name.fString), nullptr, 0);
        PA_DisposeUnistring(&name);
    }

private:
    OCIErrorCallback() {}
    mutable std::mutex m_mutex;
    std::string m_methodName;
};

// ============================================================
// OCI Return Code Check + Error Callback
// ============================================================

// Check OCI status and invoke error callback if registered
inline sword oci_check(sword status) {
    if (status != OCI_SUCCESS && status != OCI_SUCCESS_WITH_INFO) {
        OCIErrorCallback::instance().invoke();
    }
    return status;
}

#endif // OCI_BRIDGE_H
