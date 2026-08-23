/*
 * 4D-for-OCI-4dplugin.cpp
 *
 * 4D plugin that wraps Oracle OCI (Oracle Call Interface) functions.
 * Phase 1: Core subset — Connection, Datatype, Transaction, Extras.
 *
 * Each 4D command maps to an OCI C function. OCI handles (pointers) are
 * stored in an internal handle table and exposed to 4D as integer IDs.
 */

#include "4D-for-OCI-4dplugin.h"

// ============================================================
// Plugin Lifecycle
// ============================================================

static void plugin_init() {
    handles().clear();
}

static void plugin_deinit() {
    stmtBuffers().clear();
    handles().clear();
    OCIErrorCallback::instance().setMethod("");
}

// ============================================================
// PluginMain — command dispatcher
// ============================================================

void PluginMain(PA_long32 selector, PA_PluginParameters params) {

    switch (selector) {
        // Lifecycle
        case kInitPlugin:   plugin_init();   break;
        case kDeinitPlugin: plugin_deinit(); break;

        // Connection
        case kSel_OCIEnvCreate:       cmd_OCIEnvCreate(params);       break;
        case kSel_OCIHandleAlloc:     cmd_OCIHandleAlloc(params);     break;
        case kSel_OCIHandleFree:      cmd_OCIHandleFree(params);      break;
        case kSel_OCIServerAttach:    cmd_OCIServerAttach(params);    break;
        case kSel_OCIServerDetach:    cmd_OCIServerDetach(params);    break;
        case kSel_OCISessionBegin:    cmd_OCISessionBegin(params);    break;
        case kSel_OCISessionEnd:      cmd_OCISessionEnd(params);      break;
        case kSel_OCILogon:           cmd_OCILogon(params);           break;
        case kSel_OCILogoff:          cmd_OCILogoff(params);          break;
        case kSel_OCIBreak:           cmd_OCIBreak(params);           break;
        case kSel_OCIReset:           cmd_OCIReset(params);           break;

        // Datatype
        case kSel_OCIAttrGetVal:      cmd_OCIAttrGetVal(params);      break;
        case kSel_OCIAttrGetText:     cmd_OCIAttrGetText(params);     break;
        case kSel_OCIAttrSetVal:      cmd_OCIAttrSetVal(params);      break;
        case kSel_OCIAttrSetText:     cmd_OCIAttrSetText(params);     break;
        case kSel_OCIDescriptorAlloc: cmd_OCIDescriptorAlloc(params); break;
        case kSel_OCIDescriptorFree:  cmd_OCIDescriptorFree(params);  break;
        case kSel_OCIErrorGet:        cmd_OCIErrorGet(params);        break;
        case kSel_OCIServerVersion:   cmd_OCIServerVersion(params);   break;
        case kSel_OCIStmtPrepare:     cmd_OCIStmtPrepare(params);     break;
        case kSel_OCIStmtExecute:     cmd_OCIStmtExecute(params);     break;
        case kSel_OCIStmtFetch:       cmd_OCIStmtFetch(params);       break;
        case kSel_OCIStmtGetBindInfo: cmd_OCIStmtGetBindInfo(params); break;
        case kSel_OCIBindByPos:       cmd_OCIBindByPos(params);       break;
        case kSel_OCIBindByName:      cmd_OCIBindByName(params);      break;
        case kSel_OCIDefineByPos:     cmd_OCIDefineByPos(params);     break;
        case kSel_OCIDescribeAnyText: cmd_OCIDescribeAnyText(params); break;
        case kSel_OCIDescribeAnyVal:  cmd_OCIDescribeAnyVal(params);  break;

        // Transaction
        case kSel_OCITransStart:      cmd_OCITransStart(params);      break;
        case kSel_OCITransCommit:     cmd_OCITransCommit(params);     break;
        case kSel_OCITransRollback:   cmd_OCITransRollback(params);   break;
        case kSel_OCITransPrepare:    cmd_OCITransPrepare(params);    break;
        case kSel_OCITransDetach:     cmd_OCITransDetach(params);     break;
        case kSel_OCITransForget:     cmd_OCITransForget(params);     break;

        // Extras
        case kSel_OCITerminate:       cmd_OCITerminate(params);       break;
        case kSel_OCIOnErrCall:       cmd_OCIOnErrCall(params);       break;
        case kSel_OCIGetTnsnamesPath: cmd_OCIGetTnsnamesPath(params); break;
        case kSel_OCISetEnv:          cmd_OCISetEnv(params);          break;
        case kSel_OCIGetEnv:          cmd_OCIGetEnv(params);          break;

        // Connection extras
        case kSel_OCIParamGet:        cmd_OCIParamGet(params);        break;
        case kSel_OCIParamSet:        cmd_OCIParamSet(params);        break;
        case kSel_OCIPasswordChange:  cmd_OCIPasswordChange(params);  break;
    }
}

// ============================================================
// CONNECTION commands
// ============================================================

// OCIEnvCreate(envhp_out; mode) : status
// Creates OCI environment. Writes handle ID to param 1.
static void cmd_OCIEnvCreate(PA_PluginParameters params) {
    PA_long32 mode = PA_GetLongParameter(params, 2);

    OCIEnv* envhp = nullptr;
    sword status = OCIEnvCreate(&envhp, (ub4)mode,
                                 nullptr, nullptr, nullptr, nullptr, 0, nullptr);

    if (status == OCI_SUCCESS || status == OCI_SUCCESS_WITH_INFO) {
        PA_long32 id = handles().add(envhp, OCI_HTYPE_ENV);
        PA_SetLongParameter(params, 1, id);
    } else {
        PA_SetLongParameter(params, 1, 0);
    }

    PA_ReturnLong(params, oci_check(status));
}

// OCIHandleAlloc(parenth; hndlp_out; type) : status
// Allocates OCI handle. Writes new handle ID to param 2.
static void cmd_OCIHandleAlloc(PA_PluginParameters params) {
    PA_long32 parentId = PA_GetLongParameter(params, 1);
    PA_long32 type     = PA_GetLongParameter(params, 3);

    void* parent = handles().get(parentId);
    if (!parent) {
        PA_SetLongParameter(params, 2, 0);
        PA_ReturnLong(params, (PA_long32)OCI_ERROR);
        return;
    }

    void* hndlp = nullptr;
    sword status = OCIHandleAlloc(parent, &hndlp, (ub4)type, 0, nullptr);

    if (status == OCI_SUCCESS) {
        PA_long32 id = handles().add(hndlp, (ub4)type);
        PA_SetLongParameter(params, 2, id);
    } else {
        PA_SetLongParameter(params, 2, 0);
    }

    PA_ReturnLong(params, oci_check(status));
}

// OCIHandleFree(hndlp) : status
static void cmd_OCIHandleFree(PA_PluginParameters params) {
    PA_long32 hndlId = PA_GetLongParameter(params, 1);

    ub4 type = 0;
    void* hndlp = handles().remove(hndlId, &type);
    if (!hndlp) {
        PA_ReturnLong(params, (PA_long32)OCI_ERROR);
        return;
    }

    // Clean up statement buffers if this is a statement handle
    if (type == OCI_HTYPE_STMT) {
        stmtBuffers().removeStmt(hndlId);
    }

    // Actually free the OCI handle if we know its type
    if (type != 0) {
        OCIHandleFree(hndlp, type);
    }

    PA_ReturnLong(params, (PA_long32)OCI_SUCCESS);
}

// OCIServerAttach(srvhp; errhp; dblink) : status
static void cmd_OCIServerAttach(PA_PluginParameters params) {
    PA_long32 srvhpId = PA_GetLongParameter(params, 1);
    PA_long32 errhpId = PA_GetLongParameter(params, 2);

    OCIServer* srvhp = handles().getAs<OCIServer>(srvhpId);
    OCIError*  errhp  = handles().getAs<OCIError>(errhpId);

    PA_Unistring* uDblink = PA_GetStringParameter(params, 3);
    std::string dblink = unistr_to_utf8(uDblink);

    if (!srvhp || !errhp) {
        PA_ReturnLong(params, (PA_long32)OCI_ERROR);
        return;
    }

    sword status = OCIServerAttach(srvhp, errhp,
                                    (const OraText*)dblink.c_str(),
                                    (sb4)dblink.size(),
                                    OCI_DEFAULT);

    PA_ReturnLong(params, oci_check(status));
}

// OCIServerDetach(srvhp; errhp) : status
static void cmd_OCIServerDetach(PA_PluginParameters params) {
    PA_long32 srvhpId = PA_GetLongParameter(params, 1);
    PA_long32 errhpId = PA_GetLongParameter(params, 2);

    OCIServer* srvhp = handles().getAs<OCIServer>(srvhpId);
    OCIError*  errhp  = handles().getAs<OCIError>(errhpId);

    if (!srvhp || !errhp) {
        PA_ReturnLong(params, (PA_long32)OCI_ERROR);
        return;
    }

    sword status = OCIServerDetach(srvhp, errhp, OCI_DEFAULT);
    PA_ReturnLong(params, oci_check(status));
}

// OCISessionBegin(svchp; errhp; usrhp; credt; mode) : status
static void cmd_OCISessionBegin(PA_PluginParameters params) {
    PA_long32 svchpId = PA_GetLongParameter(params, 1);
    PA_long32 errhpId = PA_GetLongParameter(params, 2);
    PA_long32 usrhpId = PA_GetLongParameter(params, 3);
    PA_long32 credt   = PA_GetLongParameter(params, 4);
    PA_long32 mode    = PA_GetLongParameter(params, 5);

    OCISvcCtx*  svchp = handles().getAs<OCISvcCtx>(svchpId);
    OCIError*   errhp = handles().getAs<OCIError>(errhpId);
    OCISession* usrhp = handles().getAs<OCISession>(usrhpId);

    if (!svchp || !errhp || !usrhp) {
        PA_ReturnLong(params, (PA_long32)OCI_ERROR);
        return;
    }

    sword status = OCISessionBegin(svchp, errhp, usrhp, (ub4)credt, (ub4)mode);
    PA_ReturnLong(params, oci_check(status));
}

// OCISessionEnd(svchp; errhp; usrhp) : status
static void cmd_OCISessionEnd(PA_PluginParameters params) {
    PA_long32 svchpId = PA_GetLongParameter(params, 1);
    PA_long32 errhpId = PA_GetLongParameter(params, 2);
    PA_long32 usrhpId = PA_GetLongParameter(params, 3);

    OCISvcCtx*  svchp = handles().getAs<OCISvcCtx>(svchpId);
    OCIError*   errhp = handles().getAs<OCIError>(errhpId);
    OCISession* usrhp = handles().getAs<OCISession>(usrhpId);

    if (!svchp || !errhp || !usrhp) {
        PA_ReturnLong(params, (PA_long32)OCI_ERROR);
        return;
    }

    sword status = OCISessionEnd(svchp, errhp, usrhp, OCI_DEFAULT);
    PA_ReturnLong(params, oci_check(status));
}

// OCILogon(envhp; errhp; svchp_out; username; password; dbname) : status
// Simplified logon. Allocates service context internally.
static void cmd_OCILogon(PA_PluginParameters params) {
    PA_long32 envhpId = PA_GetLongParameter(params, 1);
    PA_long32 errhpId = PA_GetLongParameter(params, 2);

    OCIEnv*   envhp = handles().getAs<OCIEnv>(envhpId);
    OCIError* errhp = handles().getAs<OCIError>(errhpId);

    PA_Unistring* uUser = PA_GetStringParameter(params, 4);
    PA_Unistring* uPass = PA_GetStringParameter(params, 5);
    PA_Unistring* uDb   = PA_GetStringParameter(params, 6);
    std::string user = unistr_to_utf8(uUser);
    std::string pass = unistr_to_utf8(uPass);
    std::string db   = unistr_to_utf8(uDb);

    if (!envhp || !errhp) {
        PA_SetLongParameter(params, 3, 0);
        PA_ReturnLong(params, (PA_long32)OCI_ERROR);
        return;
    }

    OCISvcCtx* svchp = nullptr;
    sword status = OCILogon(envhp, errhp, &svchp,
                             (const OraText*)user.c_str(), (ub4)user.size(),
                             (const OraText*)pass.c_str(), (ub4)pass.size(),
                             (const OraText*)db.c_str(), (ub4)db.size());

    if (status == OCI_SUCCESS || status == OCI_SUCCESS_WITH_INFO) {
        PA_long32 id = handles().add(svchp, OCI_HTYPE_SVCCTX);
        PA_SetLongParameter(params, 3, id);
    } else {
        PA_SetLongParameter(params, 3, 0);
    }

    PA_ReturnLong(params, oci_check(status));
}

// OCILogoff(svchp; errhp) : status
static void cmd_OCILogoff(PA_PluginParameters params) {
    PA_long32 svchpId = PA_GetLongParameter(params, 1);
    PA_long32 errhpId = PA_GetLongParameter(params, 2);

    OCISvcCtx* svchp = handles().getAs<OCISvcCtx>(svchpId);
    OCIError*  errhp = handles().getAs<OCIError>(errhpId);

    if (!svchp || !errhp) {
        PA_ReturnLong(params, (PA_long32)OCI_ERROR);
        return;
    }

    sword status = OCILogoff(svchp, errhp);
    handles().remove(svchpId);
    PA_ReturnLong(params, oci_check(status));
}

// OCIBreak(hndlp; errhp) : status
static void cmd_OCIBreak(PA_PluginParameters params) {
    PA_long32 hndlpId = PA_GetLongParameter(params, 1);
    PA_long32 errhpId = PA_GetLongParameter(params, 2);

    void*     hndlp = handles().get(hndlpId);
    OCIError* errhp = handles().getAs<OCIError>(errhpId);

    if (!hndlp || !errhp) {
        PA_ReturnLong(params, (PA_long32)OCI_ERROR);
        return;
    }

    sword status = OCIBreak(hndlp, errhp);
    PA_ReturnLong(params, oci_check(status));
}

// OCIReset(hndlp; errhp) : status
static void cmd_OCIReset(PA_PluginParameters params) {
    PA_long32 hndlpId = PA_GetLongParameter(params, 1);
    PA_long32 errhpId = PA_GetLongParameter(params, 2);

    void*     hndlp = handles().get(hndlpId);
    OCIError* errhp = handles().getAs<OCIError>(errhpId);

    if (!hndlp || !errhp) {
        PA_ReturnLong(params, (PA_long32)OCI_ERROR);
        return;
    }

    sword status = OCIReset(hndlp, errhp);
    PA_ReturnLong(params, oci_check(status));
}

// ============================================================
// DATATYPE commands
// ============================================================

// OCIAttrGetVal(trgthndlp; trghndltyp; attrtype; errhp) : value
// Returns a numeric attribute value as the return value.
static void cmd_OCIAttrGetVal(PA_PluginParameters params) {
    PA_long32 hndlpId  = PA_GetLongParameter(params, 1);
    PA_long32 hndltyp  = PA_GetLongParameter(params, 2);
    PA_long32 attrtype = PA_GetLongParameter(params, 3);
    PA_long32 errhpId  = PA_GetLongParameter(params, 4);

    void*     hndlp = handles().get(hndlpId);
    OCIError* errhp = handles().getAs<OCIError>(errhpId);

    if (!hndlp || !errhp) {
        PA_ReturnLong(params, 0);
        return;
    }

    ub4 value = 0;
    ub4 size = 0;
    sword status = OCIAttrGet(hndlp, (ub4)hndltyp,
                               &value, &size,
                               (ub4)attrtype, errhp);

    if (status == OCI_SUCCESS || status == OCI_SUCCESS_WITH_INFO) {
        PA_ReturnLong(params, (PA_long32)value);
    } else {
        oci_check(status);
        PA_ReturnLong(params, 0);
    }
}

// OCIAttrGetText(trgthndlp; attributep_out_ptr; attrtype; errhp) : status
// Reads a text attribute and writes it to the pointed-to Text variable.
static void cmd_OCIAttrGetText(PA_PluginParameters params) {
    PA_long32 hndlpId  = PA_GetLongParameter(params, 1);
    PA_long32 attrtype = PA_GetLongParameter(params, 3);
    PA_long32 errhpId  = PA_GetLongParameter(params, 4);

    void*     hndlp = handles().get(hndlpId);
    OCIError* errhp = handles().getAs<OCIError>(errhpId);

    if (!hndlp || !errhp) {
        PA_ReturnLong(params, (PA_long32)OCI_ERROR);
        return;
    }

    // For text attributes, OCIAttrGet returns a pointer to the string
    // and its length. The string is owned by OCI — do not free it.
    OraText* attrVal = nullptr;
    ub4 attrLen = 0;

    // Use tracked handle type instead of hardcoded value
    ub4 hndltyp = handles().getType(hndlpId);
    if (hndltyp == 0) hndltyp = OCI_HTYPE_SESSION; // fallback

    sword status = OCIAttrGet(hndlp, hndltyp,
                               &attrVal, &attrLen,
                               (ub4)attrtype, errhp);

    if (status == OCI_SUCCESS || status == OCI_SUCCESS_WITH_INFO) {
        if (attrVal && attrLen > 0) {
            set_pointer_text(params, 2, (const char*)attrVal, (PA_long32)attrLen);
        } else {
            set_pointer_text(params, 2, "");
        }
    }

    PA_ReturnLong(params, oci_check(status));
}

// OCIAttrSetVal(trgthndlp; attributep_val; attrtype; errhp) : status
static void cmd_OCIAttrSetVal(PA_PluginParameters params) {
    PA_long32 hndlpId  = PA_GetLongParameter(params, 1);
    PA_long32 attrVal  = PA_GetLongParameter(params, 2);
    PA_long32 attrtype = PA_GetLongParameter(params, 3);
    PA_long32 errhpId  = PA_GetLongParameter(params, 4);

    void*     hndlp = handles().get(hndlpId);
    OCIError* errhp = handles().getAs<OCIError>(errhpId);

    if (!hndlp || !errhp) {
        PA_ReturnLong(params, (PA_long32)OCI_ERROR);
        return;
    }

    // When setting OCI_ATTR_SERVER or OCI_ATTR_SESSION, the value is a handle
    void* valPtr = nullptr;
    ub4 valSize = 0;
    ub4 hndltyp = handles().getType(hndlpId);
    if (hndltyp == 0) hndltyp = OCI_HTYPE_SVCCTX; // fallback

    // Check if the value is a handle reference
    void* possibleHandle = handles().get(attrVal);
    if (possibleHandle) {
        valPtr = possibleHandle;
        valSize = 0; // for handle attributes, size is 0
    } else {
        // Treat as a raw numeric value
        valPtr = &attrVal;
        valSize = sizeof(PA_long32);
    }

    sword status = OCIAttrSet(hndlp, hndltyp,
                               valPtr, valSize,
                               (ub4)attrtype, errhp);

    PA_ReturnLong(params, oci_check(status));
}

// OCIAttrSetText(trgthndlp; text_value; attrtype; errhp) : status
static void cmd_OCIAttrSetText(PA_PluginParameters params) {
    PA_long32 hndlpId  = PA_GetLongParameter(params, 1);
    PA_long32 attrtype = PA_GetLongParameter(params, 3);
    PA_long32 errhpId  = PA_GetLongParameter(params, 4);

    void*     hndlp = handles().get(hndlpId);
    OCIError* errhp = handles().getAs<OCIError>(errhpId);

    PA_Unistring* uText = PA_GetStringParameter(params, 2);
    std::string text = unistr_to_utf8(uText);

    if (!hndlp || !errhp) {
        PA_ReturnLong(params, (PA_long32)OCI_ERROR);
        return;
    }

    ub4 hndltyp = handles().getType(hndlpId);
    if (hndltyp == 0) hndltyp = OCI_HTYPE_SESSION; // fallback

    sword status = OCIAttrSet(hndlp, hndltyp,
                               (void*)text.c_str(), (ub4)text.size(),
                               (ub4)attrtype, errhp);

    PA_ReturnLong(params, oci_check(status));
}

// OCIDescriptorAlloc(parenth; descp_out; type) : status
static void cmd_OCIDescriptorAlloc(PA_PluginParameters params) {
    PA_long32 parentId = PA_GetLongParameter(params, 1);
    PA_long32 type     = PA_GetLongParameter(params, 3);

    void* parent = handles().get(parentId);
    if (!parent) {
        PA_SetLongParameter(params, 2, 0);
        PA_ReturnLong(params, (PA_long32)OCI_ERROR);
        return;
    }

    void* descp = nullptr;
    sword status = OCIDescriptorAlloc(parent, &descp, (ub4)type, 0, nullptr);

    if (status == OCI_SUCCESS) {
        PA_long32 id = handles().add(descp, (ub4)type);
        PA_SetLongParameter(params, 2, id);
    } else {
        PA_SetLongParameter(params, 2, 0);
    }

    PA_ReturnLong(params, oci_check(status));
}

// OCIDescriptorFree(descp) : status
static void cmd_OCIDescriptorFree(PA_PluginParameters params) {
    PA_long32 descId = PA_GetLongParameter(params, 1);
    ub4 type = 0;
    void* descp = handles().remove(descId, &type);

    if (!descp) {
        PA_ReturnLong(params, (PA_long32)OCI_ERROR);
        return;
    }

    if (type != 0) {
        OCIDescriptorFree(descp, type);
    }

    PA_ReturnLong(params, (PA_long32)OCI_SUCCESS);
}

// OCIErrorGet(hndlp; recordno; errcodep_out; bufp_out_ptr) : status
static void cmd_OCIErrorGet(PA_PluginParameters params) {
    PA_long32 hndlpId   = PA_GetLongParameter(params, 1);
    PA_long32 recordno  = PA_GetLongParameter(params, 2);

    void* hndlp = handles().get(hndlpId);
    if (!hndlp) {
        PA_SetLongParameter(params, 3, 0);
        set_pointer_text(params, 4, "Invalid handle");
        PA_ReturnLong(params, (PA_long32)OCI_ERROR);
        return;
    }

    sb4 errcode = 0;
    OraText errbuf[2048] = {0};

    sword status = OCIErrorGet(hndlp, (ub4)recordno, nullptr,
                                &errcode, errbuf, sizeof(errbuf),
                                OCI_HTYPE_ERROR);

    PA_SetLongParameter(params, 3, (PA_long32)errcode);
    set_pointer_text(params, 4, (const char*)errbuf, (PA_long32)std::strlen((char*)errbuf));

    PA_ReturnLong(params, (PA_long32)status);
}

// OCIServerVersion(hndlp; errhp; bufp_out_ptr) : status
static void cmd_OCIServerVersion(PA_PluginParameters params) {
    PA_long32 hndlpId = PA_GetLongParameter(params, 1);
    PA_long32 errhpId = PA_GetLongParameter(params, 2);

    void*     hndlp = handles().get(hndlpId);
    OCIError* errhp = handles().getAs<OCIError>(errhpId);

    if (!hndlp || !errhp) {
        PA_ReturnLong(params, (PA_long32)OCI_ERROR);
        return;
    }

    OraText buf[512] = {0};
    sword status = OCIServerVersion(hndlp, errhp, buf, sizeof(buf), OCI_HTYPE_SVCCTX);

    set_pointer_text(params, 3, (const char*)buf, (PA_long32)std::strlen((char*)buf));
    PA_ReturnLong(params, oci_check(status));
}

// OCIStmtPrepare(stmtp; errhp; stmt_text; language) : status
static void cmd_OCIStmtPrepare(PA_PluginParameters params) {
    PA_long32 stmtpId  = PA_GetLongParameter(params, 1);
    PA_long32 errhpId  = PA_GetLongParameter(params, 2);
    PA_long32 language = PA_GetLongParameter(params, 4);

    OCIStmt*  stmtp = handles().getAs<OCIStmt>(stmtpId);
    OCIError* errhp = handles().getAs<OCIError>(errhpId);

    PA_Unistring* uStmt = PA_GetStringParameter(params, 3);
    std::string stmt = unistr_to_utf8(uStmt);

    if (!stmtp || !errhp) {
        PA_ReturnLong(params, (PA_long32)OCI_ERROR);
        return;
    }

    sword status = OCIStmtPrepare(stmtp, errhp,
                                   (const OraText*)stmt.c_str(), (ub4)stmt.size(),
                                   (ub4)language, OCI_DEFAULT);

    PA_ReturnLong(params, oci_check(status));
}

// OCIStmtExecute(svchp; stmtp; errhp; iters; rowoff; snap_in; snap_out; mode) : status
static void cmd_OCIStmtExecute(PA_PluginParameters params) {
    PA_long32 svchpId  = PA_GetLongParameter(params, 1);
    PA_long32 stmtpId  = PA_GetLongParameter(params, 2);
    PA_long32 errhpId  = PA_GetLongParameter(params, 3);
    PA_long32 iters    = PA_GetLongParameter(params, 4);
    PA_long32 rowoff   = PA_GetLongParameter(params, 5);
    PA_long32 snapIn   = PA_GetLongParameter(params, 6);
    PA_long32 snapOut  = PA_GetLongParameter(params, 7);
    PA_long32 mode     = PA_GetLongParameter(params, 8);

    OCISvcCtx* svchp = handles().getAs<OCISvcCtx>(svchpId);
    OCIStmt*   stmtp = handles().getAs<OCIStmt>(stmtpId);
    OCIError*  errhp = handles().getAs<OCIError>(errhpId);

    if (!svchp || !stmtp || !errhp) {
        PA_ReturnLong(params, (PA_long32)OCI_ERROR);
        return;
    }

    // Snapshot handles (usually 0/null for simple use)
    OCISnapshot* snapInPtr  = snapIn  ? handles().getAs<OCISnapshot>(snapIn)  : nullptr;
    OCISnapshot* snapOutPtr = snapOut ? handles().getAs<OCISnapshot>(snapOut) : nullptr;

    sword status = OCIStmtExecute(svchp, stmtp, errhp,
                                   (ub4)iters, (ub4)rowoff,
                                   snapInPtr, snapOutPtr, (ub4)mode);

    PA_ReturnLong(params, oci_check(status));
}

// OCIStmtFetch(stmtp; errhp; nrows) : status
static void cmd_OCIStmtFetch(PA_PluginParameters params) {
    PA_long32 stmtpId = PA_GetLongParameter(params, 1);
    PA_long32 errhpId = PA_GetLongParameter(params, 2);
    PA_long32 nrows   = PA_GetLongParameter(params, 3);

    OCIStmt*  stmtp = handles().getAs<OCIStmt>(stmtpId);
    OCIError* errhp = handles().getAs<OCIError>(errhpId);

    if (!stmtp || !errhp) {
        PA_ReturnLong(params, (PA_long32)OCI_ERROR);
        return;
    }

    sword status = OCIStmtFetch(stmtp, errhp, (ub4)nrows,
                                 OCI_FETCH_NEXT, OCI_DEFAULT);

    // After fetch, copy buffer data back to 4D variables via stored PA_Pointers
    if (status == OCI_SUCCESS || status == OCI_SUCCESS_WITH_INFO) {
        auto* defs = stmtBuffers().getDefines(stmtpId);
        if (defs) {
            for (auto& col : *defs) {
                writeBackDefine(col);
            }
        }
    }

    PA_ReturnLong(params, (PA_long32)status);
}

// OCIStmtGetBindInfo — retrieve bind variable info from a prepared statement
// OCIStmtGetBindInfo(stmtp; errhp; found_out; max_binds; names_ptr; schemas_ptr; ind_names_ptr; dups_ptr) : status
static void cmd_OCIStmtGetBindInfo(PA_PluginParameters params) {
    PA_long32 stmtpId   = PA_GetLongParameter(params, 1);
    PA_long32 errhpId   = PA_GetLongParameter(params, 2);
    PA_long32 maxBinds  = PA_GetLongParameter(params, 4);

    OCIStmt*  stmtp = handles().getAs<OCIStmt>(stmtpId);
    OCIError* errhp = handles().getAs<OCIError>(errhpId);

    if (!stmtp || !errhp || maxBinds <= 0) {
        PA_SetLongParameter(params, 3, 0);
        PA_ReturnLong(params, (PA_long32)OCI_ERROR);
        return;
    }

    // Allocate arrays for OCI to fill
    std::vector<OraText*> bvnp(maxBinds, nullptr);
    std::vector<ub1>      bvnl(maxBinds, 0);
    std::vector<OraText*> invp(maxBinds, nullptr);
    std::vector<ub1>      inpl(maxBinds, 0);
    std::vector<ub1>      dupl(maxBinds, 0);
    std::vector<OCIBind*> hndl(maxBinds, nullptr);

    sb4 found = 0;

    sword status = OCIStmtGetBindInfo(stmtp, errhp,
                                       (ub4)maxBinds, 1, &found,
                                       bvnp.data(), bvnl.data(),
                                       invp.data(), inpl.data(),
                                       dupl.data(), hndl.data());

    PA_SetLongParameter(params, 3, (PA_long32)found);

    // Write arrays back via PA_Pointers
    sb4 count = (found > 0) ? found : 0;

    // Bind variable names (param 5)
    PA_Pointer namesPtr = PA_GetPointerParameter(params, 5);
    if (namesPtr) {
        PA_Variable arr = PA_CreateVariable(eVK_ArrayUnicode);
        PA_ResizeArray(&arr, count);
        for (sb4 i = 0; i < count; i++) {
            PA_Unistring ustr = utf8_to_unistring(
                std::string((const char*)bvnp[i], bvnl[i]));
            PA_SetStringInArray(arr, i + 1, &ustr);
        }
        PA_SetPointerValue(namesPtr, arr);
    }

    // Indicator variable names (param 6)
    PA_Pointer indNamesPtr = PA_GetPointerParameter(params, 6);
    if (indNamesPtr) {
        PA_Variable arr = PA_CreateVariable(eVK_ArrayUnicode);
        PA_ResizeArray(&arr, count);
        for (sb4 i = 0; i < count; i++) {
            PA_Unistring ustr = utf8_to_unistring(
                std::string((const char*)invp[i], inpl[i]));
            PA_SetStringInArray(arr, i + 1, &ustr);
        }
        PA_SetPointerValue(indNamesPtr, arr);
    }

    // Schema names — OCI doesn't return this directly; write empty array (param 7)
    PA_Pointer schemasPtr = PA_GetPointerParameter(params, 7);
    if (schemasPtr) {
        PA_Variable arr = PA_CreateVariable(eVK_ArrayUnicode);
        PA_ResizeArray(&arr, count);
        PA_SetPointerValue(schemasPtr, arr);
    }

    // Duplicate flags as longint array (param 8)
    PA_Pointer dupsPtr = PA_GetPointerParameter(params, 8);
    if (dupsPtr) {
        PA_Variable arr = PA_CreateVariable(eVK_ArrayLongint);
        PA_ResizeArray(&arr, count);
        for (sb4 i = 0; i < count; i++) {
            PA_SetLongintInArray(arr, i + 1, (PA_long32)dupl[i]);
        }
        PA_SetPointerValue(dupsPtr, arr);
    }

    PA_ReturnLong(params, oci_check(status));
}

// OCIBindByPos — bind input variable by position
// OCIBindByPos(stmtp; errhp; bindp_out; position; valuep_ptr; value_sz; dty; indp_ptr; rlenp_ptr; maxarr_len; curelep) : status
static void cmd_OCIBindByPos(PA_PluginParameters params) {
    PA_long32 stmtpId  = PA_GetLongParameter(params, 1);
    PA_long32 errhpId  = PA_GetLongParameter(params, 2);
    PA_long32 position = PA_GetLongParameter(params, 4);
    PA_long32 valueSz  = PA_GetLongParameter(params, 6);
    PA_long32 dty      = PA_GetLongParameter(params, 7);

    OCIStmt*  stmtp = handles().getAs<OCIStmt>(stmtpId);
    OCIError* errhp = handles().getAs<OCIError>(errhpId);

    if (!stmtp || !errhp || position < 1) {
        PA_SetLongParameter(params, 3, 0);
        PA_ReturnLong(params, (PA_long32)OCI_ERROR);
        return;
    }

    // Allocate internal buffer
    OCIBindBuffer& buf = stmtBuffers().addBind(stmtpId, (ub4)position);
    buf.dty = (ub4)dty;
    buf.data.resize(valueSz > 0 ? valueSz : 256, 0);
    buf.indicator = 0;
    buf.returnLen = 0;
    buf.valuePtr = PA_GetPointerParameter(params, 5);
    buf.indPtr   = PA_GetPointerParameter(params, 8);
    buf.rlenPtr  = PA_GetPointerParameter(params, 9);

    // Read input value from 4D pointer into buffer
    if (buf.valuePtr) {
        PA_Variable val = PA_GetPointerValue(buf.valuePtr);
        switch (val.fType) {
            case eVK_Longint: {
                PA_long32 v = val.uValue.fLongint;
                if (buf.data.size() >= sizeof(PA_long32))
                    std::memcpy(buf.data.data(), &v, sizeof(PA_long32));
                buf.returnLen = sizeof(PA_long32);
                break;
            }
            case eVK_Real: {
                double v = val.uValue.fReal;
                if (buf.data.size() >= sizeof(double))
                    std::memcpy(buf.data.data(), &v, sizeof(double));
                buf.returnLen = sizeof(double);
                break;
            }
            case eVK_Unistring: {
                std::string utf8 = unistr_to_utf8(&val.uValue.fString);
                ub4 len = (ub4)utf8.size();
                if (len > buf.data.size()) len = (ub4)buf.data.size();
                std::memcpy(buf.data.data(), utf8.c_str(), len);
                buf.returnLen = (ub2)len;
                break;
            }
            default:
                break;
        }
    }

    buf.bindp = nullptr;
    sword status = OCIBindByPos(stmtp, &buf.bindp, errhp,
                                 (ub4)position,
                                 buf.data.data(), (sb4)buf.data.size(),
                                 (ub2)dty,
                                 &buf.indicator, &buf.returnLen,
                                 nullptr, 0, nullptr, OCI_DEFAULT);

    if (status == OCI_SUCCESS && buf.bindp) {
        PA_long32 id = handles().add(buf.bindp, 0);
        PA_SetLongParameter(params, 3, id);
    } else {
        PA_SetLongParameter(params, 3, 0);
    }

    PA_ReturnLong(params, oci_check(status));
}

// OCIBindByName — bind input variable by name
// OCIBindByName(stmtp; errhp; bindp_out; placeholder; valuep_ptr; value_sz; dty; indp_ptr; rlenp_ptr; maxarr_len; curelep) : status
static void cmd_OCIBindByName(PA_PluginParameters params) {
    PA_long32 stmtpId = PA_GetLongParameter(params, 1);
    PA_long32 errhpId = PA_GetLongParameter(params, 2);
    PA_long32 valueSz = PA_GetLongParameter(params, 6);
    PA_long32 dty     = PA_GetLongParameter(params, 7);

    OCIStmt*  stmtp = handles().getAs<OCIStmt>(stmtpId);
    OCIError* errhp = handles().getAs<OCIError>(errhpId);

    PA_Unistring* uPlaceholder = PA_GetStringParameter(params, 4);
    std::string placeholder = unistr_to_utf8(uPlaceholder);

    if (!stmtp || !errhp || placeholder.empty()) {
        PA_SetLongParameter(params, 3, 0);
        PA_ReturnLong(params, (PA_long32)OCI_ERROR);
        return;
    }

    // Use a synthetic position based on placeholder hash for buffer storage
    // (bind-by-name buffers are tracked separately but reuse same structure)
    ub4 syntheticPos = 1;
    for (char c : placeholder) syntheticPos = syntheticPos * 31 + (unsigned char)c;
    syntheticPos = (syntheticPos % 10000) + 10001; // avoid collision with position-based

    OCIBindBuffer& buf = stmtBuffers().addBind(stmtpId, syntheticPos);
    buf.dty = (ub4)dty;
    buf.data.resize(valueSz > 0 ? valueSz : 256, 0);
    buf.indicator = 0;
    buf.returnLen = 0;
    buf.valuePtr = PA_GetPointerParameter(params, 5);
    buf.indPtr   = PA_GetPointerParameter(params, 8);
    buf.rlenPtr  = PA_GetPointerParameter(params, 9);

    // Read input value
    if (buf.valuePtr) {
        PA_Variable val = PA_GetPointerValue(buf.valuePtr);
        switch (val.fType) {
            case eVK_Longint: {
                PA_long32 v = val.uValue.fLongint;
                if (buf.data.size() >= sizeof(PA_long32))
                    std::memcpy(buf.data.data(), &v, sizeof(PA_long32));
                buf.returnLen = sizeof(PA_long32);
                break;
            }
            case eVK_Real: {
                double v = val.uValue.fReal;
                if (buf.data.size() >= sizeof(double))
                    std::memcpy(buf.data.data(), &v, sizeof(double));
                buf.returnLen = sizeof(double);
                break;
            }
            case eVK_Unistring: {
                std::string utf8 = unistr_to_utf8(&val.uValue.fString);
                ub4 len = (ub4)utf8.size();
                if (len > buf.data.size()) len = (ub4)buf.data.size();
                std::memcpy(buf.data.data(), utf8.c_str(), len);
                buf.returnLen = (ub2)len;
                break;
            }
            default:
                break;
        }
    }

    buf.bindp = nullptr;
    sword status = OCIBindByName(stmtp, &buf.bindp, errhp,
                                  (const OraText*)placeholder.c_str(),
                                  (sb4)placeholder.size(),
                                  buf.data.data(), (sb4)buf.data.size(),
                                  (ub2)dty,
                                  &buf.indicator, &buf.returnLen,
                                  nullptr, 0, nullptr, OCI_DEFAULT);

    if (status == OCI_SUCCESS && buf.bindp) {
        PA_long32 id = handles().add(buf.bindp, 0);
        PA_SetLongParameter(params, 3, id);
    } else {
        PA_SetLongParameter(params, 3, 0);
    }

    PA_ReturnLong(params, oci_check(status));
}

// OCIDefineByPos — define output column buffer for fetch
// OCIDefineByPos(stmtp; errhp; defnp_out; position; valuep_ptr; value_sz; dty; indp_ptr; rlenp_ptr; mode) : status
static void cmd_OCIDefineByPos(PA_PluginParameters params) {
    PA_long32 stmtpId  = PA_GetLongParameter(params, 1);
    PA_long32 errhpId  = PA_GetLongParameter(params, 2);
    PA_long32 position = PA_GetLongParameter(params, 4);
    PA_long32 valueSz  = PA_GetLongParameter(params, 6);
    PA_long32 dty      = PA_GetLongParameter(params, 7);
    PA_long32 mode     = PA_GetLongParameter(params, 10);

    OCIStmt*  stmtp = handles().getAs<OCIStmt>(stmtpId);
    OCIError* errhp = handles().getAs<OCIError>(errhpId);

    if (!stmtp || !errhp || position < 1) {
        PA_SetLongParameter(params, 3, 0);
        PA_ReturnLong(params, (PA_long32)OCI_ERROR);
        return;
    }

    // Allocate internal buffer for OCI to write into during fetch
    OCIColumnBuffer& col = stmtBuffers().addDefine(stmtpId, (ub4)position);
    col.dty = (ub4)dty;
    col.data.resize(valueSz > 0 ? valueSz : 256, 0);
    col.indicator = 0;
    col.returnLen = 0;
    col.valuePtr = PA_GetPointerParameter(params, 5);
    col.indPtr   = PA_GetPointerParameter(params, 8);
    col.rlenPtr  = PA_GetPointerParameter(params, 9);

    col.defnp = nullptr;
    sword status = OCIDefineByPos(stmtp, &col.defnp, errhp,
                                   (ub4)position,
                                   col.data.data(), (sb4)col.data.size(),
                                   (ub2)dty,
                                   &col.indicator, &col.returnLen,
                                   nullptr, (ub4)mode);

    if (status == OCI_SUCCESS && col.defnp) {
        PA_long32 id = handles().add(col.defnp, 0);
        PA_SetLongParameter(params, 3, id);
    } else {
        PA_SetLongParameter(params, 3, 0);
    }

    PA_ReturnLong(params, oci_check(status));
}

// OCIDescribeAnyText(svchp; errhp; objnm; objnm_len; objptr_typ; info_level; dschp) : status
static void cmd_OCIDescribeAnyText(PA_PluginParameters params) {
    PA_long32 svchpId   = PA_GetLongParameter(params, 1);
    PA_long32 errhpId   = PA_GetLongParameter(params, 2);
    PA_long32 objnmLen  = PA_GetLongParameter(params, 4);
    PA_long32 objptrTyp = PA_GetLongParameter(params, 5);
    PA_long32 infoLevel = PA_GetLongParameter(params, 6);
    PA_long32 dschpId   = PA_GetLongParameter(params, 7);

    OCISvcCtx*  svchp = handles().getAs<OCISvcCtx>(svchpId);
    OCIError*   errhp = handles().getAs<OCIError>(errhpId);
    OCIDescribe* dschp = handles().getAs<OCIDescribe>(dschpId);

    PA_Unistring* uObjnm = PA_GetStringParameter(params, 3);
    std::string objnm = unistr_to_utf8(uObjnm);

    if (!svchp || !errhp || !dschp) {
        PA_ReturnLong(params, (PA_long32)OCI_ERROR);
        return;
    }

    sword status = OCIDescribeAny(svchp, errhp,
                                   (void*)objnm.c_str(), (ub4)objnm.size(),
                                   (ub1)objptrTyp, (ub1)infoLevel,
                                   (ub1)OCI_PTYPE_UNK, dschp);

    PA_ReturnLong(params, oci_check(status));
}

// OCIDescribeAnyVal(svchp; errhp; objptr; objptr_typ; info_level; objtyp; dschp) : status
static void cmd_OCIDescribeAnyVal(PA_PluginParameters params) {
    PA_long32 svchpId   = PA_GetLongParameter(params, 1);
    PA_long32 errhpId   = PA_GetLongParameter(params, 2);
    PA_long32 objptrId  = PA_GetLongParameter(params, 3);
    PA_long32 objptrTyp = PA_GetLongParameter(params, 4);
    PA_long32 infoLevel = PA_GetLongParameter(params, 5);
    PA_long32 objtyp    = PA_GetLongParameter(params, 6);
    PA_long32 dschpId   = PA_GetLongParameter(params, 7);

    OCISvcCtx*   svchp = handles().getAs<OCISvcCtx>(svchpId);
    OCIError*    errhp = handles().getAs<OCIError>(errhpId);
    void*        objptr = handles().get(objptrId);
    OCIDescribe* dschp  = handles().getAs<OCIDescribe>(dschpId);

    if (!svchp || !errhp || !dschp) {
        PA_ReturnLong(params, (PA_long32)OCI_ERROR);
        return;
    }

    sword status = OCIDescribeAny(svchp, errhp,
                                   objptr, 0,
                                   (ub1)objptrTyp, (ub1)infoLevel,
                                   (ub1)objtyp, dschp);

    PA_ReturnLong(params, oci_check(status));
}

// ============================================================
// TRANSACTION commands
// ============================================================

// OCITransStart(svchp; errhp; timeout; flags) : status
static void cmd_OCITransStart(PA_PluginParameters params) {
    PA_long32 svchpId = PA_GetLongParameter(params, 1);
    PA_long32 errhpId = PA_GetLongParameter(params, 2);
    PA_long32 timeout = PA_GetLongParameter(params, 3);
    PA_long32 flags   = PA_GetLongParameter(params, 4);

    OCISvcCtx* svchp = handles().getAs<OCISvcCtx>(svchpId);
    OCIError*  errhp = handles().getAs<OCIError>(errhpId);

    if (!svchp || !errhp) {
        PA_ReturnLong(params, (PA_long32)OCI_ERROR);
        return;
    }

    sword status = OCITransStart(svchp, errhp, (uword)timeout, (ub4)flags);
    PA_ReturnLong(params, oci_check(status));
}

// OCITransCommit(svchp; errhp; flags) : status
static void cmd_OCITransCommit(PA_PluginParameters params) {
    PA_long32 svchpId = PA_GetLongParameter(params, 1);
    PA_long32 errhpId = PA_GetLongParameter(params, 2);
    PA_long32 flags   = PA_GetLongParameter(params, 3);

    OCISvcCtx* svchp = handles().getAs<OCISvcCtx>(svchpId);
    OCIError*  errhp = handles().getAs<OCIError>(errhpId);

    if (!svchp || !errhp) {
        PA_ReturnLong(params, (PA_long32)OCI_ERROR);
        return;
    }

    sword status = OCITransCommit(svchp, errhp, (ub4)flags);
    PA_ReturnLong(params, oci_check(status));
}

// OCITransRollback(svchp; errhp; flags) : status
static void cmd_OCITransRollback(PA_PluginParameters params) {
    PA_long32 svchpId = PA_GetLongParameter(params, 1);
    PA_long32 errhpId = PA_GetLongParameter(params, 2);
    PA_long32 flags   = PA_GetLongParameter(params, 3);

    OCISvcCtx* svchp = handles().getAs<OCISvcCtx>(svchpId);
    OCIError*  errhp = handles().getAs<OCIError>(errhpId);

    if (!svchp || !errhp) {
        PA_ReturnLong(params, (PA_long32)OCI_ERROR);
        return;
    }

    sword status = OCITransRollback(svchp, errhp, (ub4)flags);
    PA_ReturnLong(params, oci_check(status));
}

// OCITransPrepare(svchp; errhp; flags) : status
static void cmd_OCITransPrepare(PA_PluginParameters params) {
    PA_long32 svchpId = PA_GetLongParameter(params, 1);
    PA_long32 errhpId = PA_GetLongParameter(params, 2);
    PA_long32 flags   = PA_GetLongParameter(params, 3);

    OCISvcCtx* svchp = handles().getAs<OCISvcCtx>(svchpId);
    OCIError*  errhp = handles().getAs<OCIError>(errhpId);

    if (!svchp || !errhp) {
        PA_ReturnLong(params, (PA_long32)OCI_ERROR);
        return;
    }

    sword status = OCITransPrepare(svchp, errhp, (ub4)flags);
    PA_ReturnLong(params, oci_check(status));
}

// OCITransDetach(svchp; errhp; flags) : status
static void cmd_OCITransDetach(PA_PluginParameters params) {
    PA_long32 svchpId = PA_GetLongParameter(params, 1);
    PA_long32 errhpId = PA_GetLongParameter(params, 2);
    PA_long32 flags   = PA_GetLongParameter(params, 3);

    OCISvcCtx* svchp = handles().getAs<OCISvcCtx>(svchpId);
    OCIError*  errhp = handles().getAs<OCIError>(errhpId);

    if (!svchp || !errhp) {
        PA_ReturnLong(params, (PA_long32)OCI_ERROR);
        return;
    }

    sword status = OCITransDetach(svchp, errhp, (ub4)flags);
    PA_ReturnLong(params, oci_check(status));
}

// OCITransForget(svchp; errhp; flags) : status
static void cmd_OCITransForget(PA_PluginParameters params) {
    PA_long32 svchpId = PA_GetLongParameter(params, 1);
    PA_long32 errhpId = PA_GetLongParameter(params, 2);
    PA_long32 flags   = PA_GetLongParameter(params, 3);

    OCISvcCtx* svchp = handles().getAs<OCISvcCtx>(svchpId);
    OCIError*  errhp = handles().getAs<OCIError>(errhpId);

    if (!svchp || !errhp) {
        PA_ReturnLong(params, (PA_long32)OCI_ERROR);
        return;
    }

    sword status = OCITransForget(svchp, errhp, (ub4)flags);
    PA_ReturnLong(params, oci_check(status));
}

// ============================================================
// EXTRAS commands
// ============================================================

// OCITerminate : status
static void cmd_OCITerminate(PA_PluginParameters params) {
    sword status = OCITerminate(OCI_DEFAULT);
    handles().clear();
    PA_ReturnLong(params, (PA_long32)status);
}

// OCIOnErrCall(methodName)
static void cmd_OCIOnErrCall(PA_PluginParameters params) {
    PA_Unistring* uName = PA_GetStringParameter(params, 1);
    std::string name = unistr_to_utf8(uName);
    OCIErrorCallback::instance().setMethod(name);
}

// OCIGetTnsnamesPath : path
// Returns the path to the tnsnames.ora file based on TNS_ADMIN env var.
static void cmd_OCIGetTnsnamesPath(PA_PluginParameters params) {
    const char* tnsAdmin = std::getenv("TNS_ADMIN");
    std::string path;
    if (tnsAdmin) {
        path = std::string(tnsAdmin);
        if (!path.empty() && path.back() != '/' && path.back() != '\\') {
            path += '/';
        }
        path += "tnsnames.ora";
    }

    PA_Unistring ustr = utf8_to_unistring(path);
    PA_ReturnString(params, ustr.fString);
    PA_DisposeUnistring(&ustr);
}

// OCISetEnv(name; value) : status
static void cmd_OCISetEnv(PA_PluginParameters params) {
    PA_Unistring* uName  = PA_GetStringParameter(params, 1);
    PA_Unistring* uValue = PA_GetStringParameter(params, 2);
    std::string name  = unistr_to_utf8(uName);
    std::string value = unistr_to_utf8(uValue);

    int result;
#ifdef _WIN32
    result = _putenv_s(name.c_str(), value.c_str());
#else
    result = setenv(name.c_str(), value.c_str(), 1);
#endif

    PA_ReturnLong(params, (PA_long32)(result == 0 ? OCI_SUCCESS : OCI_ERROR));
}

// OCIGetEnv(name; value_out_ptr) : status
static void cmd_OCIGetEnv(PA_PluginParameters params) {
    PA_Unistring* uName = PA_GetStringParameter(params, 1);
    std::string name = unistr_to_utf8(uName);

    const char* val = std::getenv(name.c_str());

    if (val) {
        set_pointer_text(params, 2, val, (PA_long32)std::strlen(val));
        PA_ReturnLong(params, (PA_long32)OCI_SUCCESS);
    } else {
        set_pointer_text(params, 2, "");
        PA_ReturnLong(params, (PA_long32)OCI_ERROR);
    }
}

// ============================================================
// CONNECTION extras
// ============================================================

// OCIParamGet(hndlp; htype; errhp; pos) : paramHandle
// Returns a parameter handle at the given position.
static void cmd_OCIParamGet(PA_PluginParameters params) {
    PA_long32 hndlpId = PA_GetLongParameter(params, 1);
    PA_long32 htype   = PA_GetLongParameter(params, 2);
    PA_long32 errhpId = PA_GetLongParameter(params, 3);
    PA_long32 pos     = PA_GetLongParameter(params, 4);

    void*     hndlp = handles().get(hndlpId);
    OCIError* errhp = handles().getAs<OCIError>(errhpId);

    if (!hndlp || !errhp) {
        PA_ReturnLong(params, 0);
        return;
    }

    void* parmhp = nullptr;
    sword status = OCIParamGet(hndlp, (ub4)htype, errhp, &parmhp, (ub4)pos);

    if (status == OCI_SUCCESS && parmhp) {
        PA_long32 id = handles().add(parmhp);
        PA_ReturnLong(params, id);
    } else {
        oci_check(status);
        PA_ReturnLong(params, 0);
    }
}

// OCIParamSet(hndlp; htype; errhp; pos) : status
static void cmd_OCIParamSet(PA_PluginParameters params) {
    // OCIParamSet is not commonly used — it sets a parameter in a complex descriptor
    // TODO: Implement if needed
    PA_ReturnLong(params, (PA_long32)OCI_ERROR);
}

// OCIPasswordChange(svchp; errhp; user; old_pw; new_pw; mode) : status
static void cmd_OCIPasswordChange(PA_PluginParameters params) {
    PA_long32 svchpId = PA_GetLongParameter(params, 1);
    PA_long32 errhpId = PA_GetLongParameter(params, 2);
    PA_long32 mode    = PA_GetLongParameter(params, 6);

    OCISvcCtx* svchp = handles().getAs<OCISvcCtx>(svchpId);
    OCIError*  errhp = handles().getAs<OCIError>(errhpId);

    PA_Unistring* uUser  = PA_GetStringParameter(params, 3);
    PA_Unistring* uOldPw = PA_GetStringParameter(params, 4);
    PA_Unistring* uNewPw = PA_GetStringParameter(params, 5);
    std::string user  = unistr_to_utf8(uUser);
    std::string oldPw = unistr_to_utf8(uOldPw);
    std::string newPw = unistr_to_utf8(uNewPw);

    if (!svchp || !errhp) {
        PA_ReturnLong(params, (PA_long32)OCI_ERROR);
        return;
    }

    sword status = OCIPasswordChange(svchp, errhp,
                                      (const OraText*)user.c_str(), (ub4)user.size(),
                                      (const OraText*)oldPw.c_str(), (ub4)oldPw.size(),
                                      (const OraText*)newPw.c_str(), (ub4)newPw.size(),
                                      (ub4)mode);

    PA_ReturnLong(params, oci_check(status));
}
