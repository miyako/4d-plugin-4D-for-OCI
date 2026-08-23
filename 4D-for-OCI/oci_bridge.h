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

    // Register a new OCI pointer with its type, returns its integer ID
    PA_long32 add(void* ptr, ub4 type = 0) {
        std::lock_guard<std::mutex> lock(m_mutex);
        PA_long32 id = m_nextId++;
        m_handles[id] = {ptr, type};
        return id;
    }

    // Look up an OCI pointer by its integer ID (returns nullptr if not found)
    void* get(PA_long32 id) const {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto it = m_handles.find(id);
        return (it != m_handles.end()) ? it->second.ptr : nullptr;
    }

    // Get the OCI type (OCI_HTYPE_* or OCI_DTYPE_*) for a handle
    ub4 getType(PA_long32 id) const {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto it = m_handles.find(id);
        return (it != m_handles.end()) ? it->second.type : 0;
    }

    // Remove a handle by its integer ID, returns the pointer and type
    void* remove(PA_long32 id, ub4* outType = nullptr) {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto it = m_handles.find(id);
        if (it == m_handles.end()) {
            if (outType) *outType = 0;
            return nullptr;
        }
        void* ptr = it->second.ptr;
        if (outType) *outType = it->second.type;
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
    struct HandleEntry {
        void* ptr;
        ub4   type;
    };
    OCIHandleTable() : m_nextId(1) {}
    mutable std::mutex m_mutex;
    std::unordered_map<PA_long32, HandleEntry> m_handles;
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

    PA_Variable var = PA_CreateVariable(eVK_Unistring);
    PA_SetStringVariable(&var, &ustr);

    // After PA_SetPointerValue, 4D owns the variable content — do NOT dispose
    PA_SetPointerValue(ptr, var);
}

// Write a UTF-8 std::string to a 4D Text variable
inline void set_pointer_text(PA_PluginParameters params, short index,
                              const std::string& str) {
    set_pointer_text(params, index, str.c_str(), (PA_long32)str.size());
}

// Write a longint value to a 4D variable referenced by a pointer parameter (&Z)
inline void set_pointer_long(PA_Pointer ptr, PA_long32 value) {
    if (!ptr) return;
    PA_Variable var = PA_CreateVariable(eVK_Longint);
    var.uValue.fLongint = value;
    PA_SetPointerValue(ptr, var);
}

// Write a real (double) value to a 4D variable referenced by a pointer parameter (&Z)
inline void set_pointer_real(PA_Pointer ptr, double value) {
    if (!ptr) return;
    PA_Variable var = PA_CreateVariable(eVK_Real);
    var.uValue.fReal = value;
    PA_SetPointerValue(ptr, var);
}

// Write a text value to a 4D variable via PA_Pointer
inline void set_pointer_text(PA_Pointer ptr, const char* utf8, PA_long32 len) {
    if (!ptr) return;
    PA_Unistring ustr = utf8_to_unistring(std::string(utf8, len));
    PA_Variable var = PA_CreateVariable(eVK_Unistring);
    PA_SetStringVariable(&var, &ustr);
    PA_SetPointerValue(ptr, var);
}

// ============================================================
// Define/Bind Buffer Management
// ============================================================
// OCI requires persistent C buffers for OCIDefineByPos / OCIBindByPos.
// The plugin allocates buffers, OCI writes into them during fetch,
// then we copy data back to 4D variables via stored PA_Pointers.

struct OCIColumnBuffer {
    std::vector<char> data;   // value buffer
    sb2   indicator;          // null indicator (-1=NULL, 0=not null)
    ub2   returnLen;          // actual data length
    ub4   dty;                // OCI data type (SQLT_*)
    PA_Pointer valuePtr;      // 4D pointer for write-back (may be null)
    PA_Pointer indPtr;        // 4D pointer for indicator write-back
    PA_Pointer rlenPtr;       // 4D pointer for return length write-back
    OCIDefine* defnp;         // OCI define handle (owned by OCI)

    OCIColumnBuffer() : indicator(0), returnLen(0), dty(0),
        valuePtr(nullptr), indPtr(nullptr), rlenPtr(nullptr), defnp(nullptr) {}
};

struct OCIBindBuffer {
    std::vector<char> data;
    sb2   indicator;
    ub2   returnLen;
    ub4   dty;
    PA_Pointer valuePtr;
    PA_Pointer indPtr;
    PA_Pointer rlenPtr;
    OCIBind* bindp;

    OCIBindBuffer() : indicator(0), returnLen(0), dty(0),
        valuePtr(nullptr), indPtr(nullptr), rlenPtr(nullptr), bindp(nullptr) {}
};

// Per-statement buffer storage
class OCIStmtBuffers {
public:
    static OCIStmtBuffers& instance() {
        static OCIStmtBuffers mgr;
        return mgr;
    }

    // Register define buffer for a statement column
    OCIColumnBuffer& addDefine(PA_long32 stmtId, ub4 position) {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto& defs = m_defines[stmtId];
        // Resize if needed (position is 1-based)
        if (position > defs.size()) defs.resize(position);
        return defs[position - 1];
    }

    // Get define buffers for a statement (for fetch write-back)
    std::vector<OCIColumnBuffer>* getDefines(PA_long32 stmtId) {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto it = m_defines.find(stmtId);
        return (it != m_defines.end()) ? &it->second : nullptr;
    }

    // Register bind buffer for a statement
    OCIBindBuffer& addBind(PA_long32 stmtId, ub4 position) {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto& binds = m_binds[stmtId];
        if (position > binds.size()) binds.resize(position);
        return binds[position - 1];
    }

    // Clean up buffers when a statement is freed
    void removeStmt(PA_long32 stmtId) {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_defines.erase(stmtId);
        m_binds.erase(stmtId);
    }

    void clear() {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_defines.clear();
        m_binds.clear();
    }

private:
    OCIStmtBuffers() {}
    mutable std::mutex m_mutex;
    std::unordered_map<PA_long32, std::vector<OCIColumnBuffer>> m_defines;
    std::unordered_map<PA_long32, std::vector<OCIBindBuffer>> m_binds;
};

inline OCIStmtBuffers& stmtBuffers() { return OCIStmtBuffers::instance(); }

// Write fetched data from internal buffer back to 4D variable through PA_Pointer
inline void writeBackDefine(OCIColumnBuffer& col) {
    if (col.indicator == -1) {
        // NULL value — write empty string or 0
        if (col.valuePtr) {
            switch (col.dty) {
                case SQLT_INT: case SQLT_UIN:
                    set_pointer_long(col.valuePtr, 0); break;
                case SQLT_FLT: case SQLT_BFLOAT: case SQLT_BDOUBLE:
                    set_pointer_real(col.valuePtr, 0.0); break;
                default:
                    set_pointer_text(col.valuePtr, "", 0); break;
            }
        }
    } else {
        if (col.valuePtr) {
            switch (col.dty) {
                case SQLT_INT:
                    set_pointer_long(col.valuePtr,
                        *reinterpret_cast<PA_long32*>(col.data.data()));
                    break;
                case SQLT_UIN:
                    set_pointer_long(col.valuePtr,
                        (PA_long32)*reinterpret_cast<unsigned int*>(col.data.data()));
                    break;
                case SQLT_FLT: case SQLT_BDOUBLE:
                    set_pointer_real(col.valuePtr,
                        *reinterpret_cast<double*>(col.data.data()));
                    break;
                case SQLT_BFLOAT:
                    set_pointer_real(col.valuePtr,
                        (double)*reinterpret_cast<float*>(col.data.data()));
                    break;
                default:
                    // String types: SQLT_STR, SQLT_CHR, SQLT_AFC, SQLT_AVC, etc.
                    set_pointer_text(col.valuePtr, col.data.data(),
                        (PA_long32)col.returnLen);
                    break;
            }
        }
    }
    // Write indicator and return length
    set_pointer_long(col.indPtr, (PA_long32)col.indicator);
    set_pointer_long(col.rlenPtr, (PA_long32)col.returnLen);
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
