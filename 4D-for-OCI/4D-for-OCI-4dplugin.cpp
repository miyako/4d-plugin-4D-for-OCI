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

        // Types/Cache
        case kSel_OCICacheFlush:      cmd_OCICacheFlush(params);      break;
        case kSel_OCICacheFree:       cmd_OCICacheFree(params);       break;
        case kSel_OCICacheRefresh:    cmd_OCICacheRefresh(params);    break;
        case kSel_OCICacheUnmark:     cmd_OCICacheUnmark(params);     break;
        case kSel_OCICacheUnpin:      cmd_OCICacheUnpin(params);      break;

        // Math/OCINumber
        case kSel_OCINumberAdd:       cmd_OCINumberAdd(params);       break;
        case kSel_OCINumberSub:       cmd_OCINumberSub(params);       break;
        case kSel_OCINumberMul:       cmd_OCINumberMul(params);       break;
        case kSel_OCINumberDiv:       cmd_OCINumberDiv(params);       break;
        case kSel_OCINumberPower:     cmd_OCINumberPower(params);     break;
        case kSel_OCINumberLog:       cmd_OCINumberLog(params);       break;
        case kSel_OCINumberArcTan2:   cmd_OCINumberArcTan2(params);   break;
        case kSel_OCINumberSqrt:      cmd_OCINumberSqrt(params);      break;
        case kSel_OCINumberLn:        cmd_OCINumberLn(params);        break;
        case kSel_OCINumberExp:       cmd_OCINumberExp(params);       break;
        case kSel_OCINumberSin:       cmd_OCINumberSin(params);       break;
        case kSel_OCINumberCos:       cmd_OCINumberCos(params);       break;
        case kSel_OCINumberTan:       cmd_OCINumberTan(params);       break;
        case kSel_OCINumberArcSin:    cmd_OCINumberArcSin(params);    break;
        case kSel_OCINumberArcCos:    cmd_OCINumberArcCos(params);    break;
        case kSel_OCINumberArcTan:    cmd_OCINumberArcTan(params);    break;
        case kSel_OCINumberHypSin:    cmd_OCINumberHypSin(params);    break;
        case kSel_OCINumberHypCos:    cmd_OCINumberHypCos(params);    break;
        case kSel_OCINumberHypTan:    cmd_OCINumberHypTan(params);    break;
        case kSel_OCINumberTrunc:     cmd_OCINumberTrunc(params);     break;
        case kSel_OCINumberRound:     cmd_OCINumberRound(params);     break;
        case kSel_OCINumberIntPower:  cmd_OCINumberIntPower(params);  break;
        case kSel_OCINumberFromText:  cmd_OCINumberFromText(params);  break;
        case kSel_OCINumberToText:    cmd_OCINumberToText(params);    break;

        // LOB commands
        case kSel_OCILobAppend:           cmd_OCILobAppend(params);           break;
        case kSel_OCILobAssign:           cmd_OCILobAssign(params);           break;
        case kSel_OCILobCharSetForm:      cmd_OCILobCharSetForm(params);      break;
        case kSel_OCILobCharSetId:        cmd_OCILobCharSetId(params);        break;
        case kSel_OCILobCopy:             cmd_OCILobCopy(params);             break;
        case kSel_OCILobCreateTemporary:  cmd_OCILobCreateTemporary(params);  break;
        case kSel_OCILobDisableBuffering: cmd_OCILobDisableBuffering(params); break;
        case kSel_OCILobEnableBuffering:  cmd_OCILobEnableBuffering(params);  break;
        case kSel_OCILobErase:            cmd_OCILobErase(params);            break;
        case kSel_OCILobFileClose:        cmd_OCILobFileClose(params);        break;
        case kSel_OCILobFileCloseAll:     cmd_OCILobFileCloseAll(params);     break;
        case kSel_OCILobFileExists:       cmd_OCILobFileExists(params);       break;
        case kSel_OCILobFileGetName:      cmd_OCILobFileGetName(params);      break;
        case kSel_OCILobFileIsOpen:       cmd_OCILobFileIsOpen(params);       break;
        case kSel_OCILobFileOpen:         cmd_OCILobFileOpen(params);         break;
        case kSel_OCILobFileSetName:      cmd_OCILobFileSetName(params);      break;
        case kSel_OCILobFlushBuffer:      cmd_OCILobFlushBuffer(params);      break;
        case kSel_OCILobFreeTemporary:    cmd_OCILobFreeTemporary(params);    break;
        case kSel_OCILobGetChunkSize:     cmd_OCILobGetChunkSize(params);     break;
        case kSel_OCILobGetLength:        cmd_OCILobGetLength(params);        break;
        case kSel_OCILobIsEqual:          cmd_OCILobIsEqual(params);          break;
        case kSel_OCILobIsTemporary:      cmd_OCILobIsTemporary(params);      break;
        case kSel_OCILobLoadFromFile:     cmd_OCILobLoadFromFile(params);     break;
        case kSel_OCILobLocatorIsInit:    cmd_OCILobLocatorIsInit(params);    break;
        case kSel_OCILobRead:             cmd_OCILobRead(params);             break;
        case kSel_OCILobTrim:             cmd_OCILobTrim(params);             break;
        case kSel_OCILobWrite:            cmd_OCILobWrite(params);            break;
        case kSel_OCILobWriteAppend:      cmd_OCILobWriteAppend(params);      break;
        case kSel_OCIDurationBegin:       cmd_OCIDurationBegin(params);       break;
        case kSel_OCIDurationEnd:         cmd_OCIDurationEnd(params);         break;

        // Date
        case kSel_OCIDateAddDays:        cmd_OCIDateAddDays(params);        break;
        case kSel_OCIDateAddMonths:      cmd_OCIDateAddMonths(params);      break;
        case kSel_OCIDateFromText:       cmd_OCIDateFromText(params);       break;
        case kSel_OCIDateToText:         cmd_OCIDateToText(params);         break;
        case kSel_OCIDateSysDate:        cmd_OCIDateSysDate(params);        break;
        case kSel_OCIDateLastDay:        cmd_OCIDateLastDay(params);        break;
        case kSel_OCIDateNextDay:        cmd_OCIDateNextDay(params);        break;
        case kSel_OCIDateZoneToZone:     cmd_OCIDateZoneToZone(params);     break;

        // Date Bind/Define
        case kSel_OCIBindDateByPos:      cmd_OCIBindDateByPos(params);      break;
        case kSel_OCIBindDateByName:     cmd_OCIBindDateByName(params);     break;
        case kSel_OCIDefineDateByPos:    cmd_OCIDefineDateByPos(params);    break;

        // Ref
        case kSel_OCIRefAssign:          cmd_OCIRefAssign(params);          break;
        case kSel_OCIRefClear:           cmd_OCIRefClear(params);           break;
        case kSel_OCIRefFromHex:         cmd_OCIRefFromHex(params);         break;
        case kSel_OCIRefToHex:           cmd_OCIRefToHex(params);           break;
        case kSel_OCIRefHexSize:         cmd_OCIRefHexSize(params);         break;
        case kSel_OCIRefIsEqual:         cmd_OCIRefIsEqual(params);         break;
        case kSel_OCIRefIsNull:          cmd_OCIRefIsNull(params);          break;

        // Raw
        case kSel_OCIRawAllocSize:       cmd_OCIRawAllocSize(params);       break;
        case kSel_OCIRawAssignBytes:     cmd_OCIRawAssignBytes(params);     break;
        case kSel_OCIRawAssignRaw:       cmd_OCIRawAssignRaw(params);       break;
        case kSel_OCIRawPtr:             cmd_OCIRawPtr(params);             break;
        case kSel_OCIRawResize:          cmd_OCIRawResize(params);          break;
        case kSel_OCIRawSize:            cmd_OCIRawSize(params);            break;

        // Collection (stubs)
        case kSel_OCICollAppend:         cmd_OCICollAppend(params);         break;
        case kSel_OCICollAssign:         cmd_OCICollAssign(params);         break;
        case kSel_OCICollAssignElem:     cmd_OCICollAssignElem(params);     break;
        case kSel_OCICollGetElem:        cmd_OCICollGetElem(params);        break;
        case kSel_OCICollMax:            cmd_OCICollMax(params);            break;
        case kSel_OCICollSize:           cmd_OCICollSize(params);           break;
        case kSel_OCICollTrim:           cmd_OCICollTrim(params);           break;

        // Iterator (stubs)
        case kSel_OCIIterCreate:         cmd_OCIIterCreate(params);         break;
        case kSel_OCIIterDelete:         cmd_OCIIterDelete(params);         break;
        case kSel_OCIIterInit:           cmd_OCIIterInit(params);           break;
        case kSel_OCIIterGetCurrent:     cmd_OCIIterGetCurrent(params);     break;
        case kSel_OCIIterNext:           cmd_OCIIterNext(params);           break;
        case kSel_OCIIterPrev:           cmd_OCIIterPrev(params);           break;

        // Table (stubs)
        case kSel_OCITableDelete:        cmd_OCITableDelete(params);        break;
        case kSel_OCITableExists:        cmd_OCITableExists(params);        break;
        case kSel_OCITableFirst:         cmd_OCITableFirst(params);         break;
        case kSel_OCITableLast:          cmd_OCITableLast(params);          break;
        case kSel_OCITableNext:          cmd_OCITableNext(params);          break;
        case kSel_OCITablePrev:          cmd_OCITablePrev(params);          break;
        case kSel_OCITableSize:          cmd_OCITableSize(params);          break;
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

// OCIParamSet(hndlp; htype; errhp; dscp; pos) : status
// Manifest: OCIParamSet(&L;&L;&L;&L):L → hndlp, errhp, dscp, pos
// htype and dsctype are resolved from the handle table
static void cmd_OCIParamSet(PA_PluginParameters params) {
    PA_long32 hndlpId = PA_GetLongParameter(params, 1);
    PA_long32 errhpId = PA_GetLongParameter(params, 2);
    PA_long32 dscpId  = PA_GetLongParameter(params, 3);
    PA_long32 pos     = PA_GetLongParameter(params, 4);

    void*     hndlp = handles().get(hndlpId);
    OCIError* errhp = handles().getAs<OCIError>(errhpId);
    void*     dscp  = handles().get(dscpId);

    if (!hndlp || !errhp || !dscp) {
        PA_ReturnLong(params, (PA_long32)OCI_ERROR);
        return;
    }

    ub4 htype   = handles().getType(hndlpId);
    ub4 dsctype = handles().getType(dscpId);

    sword status = OCIParamSet(hndlp, htype, errhp,
                               (const void*)dscp, dsctype, (ub4)pos);
    PA_ReturnLong(params, oci_check(status));
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

// ---------------------------------------------------------------------------
// Helpers: double <-> OCINumber conversion
// ---------------------------------------------------------------------------

static sword double_to_OCINumber(OCIError* errhp, double val, OCINumber* num) {
    return OCINumberFromReal(errhp, &val, sizeof(double), num);
}

static sword OCINumber_to_double(OCIError* errhp, const OCINumber* num, double* val) {
    return OCINumberToReal(errhp, num, sizeof(double), val);
}

// ---------------------------------------------------------------------------
// Types/Cache commands
// ---------------------------------------------------------------------------

static void cmd_OCICacheFlush(PA_PluginParameters params) {
    PA_long32 envhpId = PA_GetLongParameter(params, 1);
    PA_long32 errhpId = PA_GetLongParameter(params, 2);
    PA_long32 svchpId = PA_GetLongParameter(params, 3);
    PA_long32 context = PA_GetLongParameter(params, 4);

    OCIEnv*    envhp = handles().getAs<OCIEnv>(envhpId);
    OCIError*  errhp = handles().getAs<OCIError>(errhpId);
    OCISvcCtx* svchp = handles().getAs<OCISvcCtx>(svchpId);

    if (!envhp || !errhp || !svchp) {
        PA_ReturnLong(params, (PA_long32)OCI_ERROR);
        return;
    }

    sword status = OCICacheFlush(envhp, errhp, svchp, (void*)(uintptr_t)context, nullptr, nullptr);
    PA_ReturnLong(params, oci_check(status));
}

static void cmd_OCICacheFree(PA_PluginParameters params) {
    PA_long32 envhpId = PA_GetLongParameter(params, 1);
    PA_long32 errhpId = PA_GetLongParameter(params, 2);
    PA_long32 svchpId = PA_GetLongParameter(params, 3);

    OCIEnv*    envhp = handles().getAs<OCIEnv>(envhpId);
    OCIError*  errhp = handles().getAs<OCIError>(errhpId);
    OCISvcCtx* svchp = handles().getAs<OCISvcCtx>(svchpId);

    if (!envhp || !errhp || !svchp) {
        PA_ReturnLong(params, (PA_long32)OCI_ERROR);
        return;
    }

    sword status = OCICacheFree(envhp, errhp, svchp);
    PA_ReturnLong(params, oci_check(status));
}

static void cmd_OCICacheRefresh(PA_PluginParameters params) {
    PA_long32 envhpId = PA_GetLongParameter(params, 1);
    PA_long32 errhpId = PA_GetLongParameter(params, 2);
    PA_long32 svchpId = PA_GetLongParameter(params, 3);
    PA_long32 option  = PA_GetLongParameter(params, 4);
    PA_long32 context = PA_GetLongParameter(params, 5);

    OCIEnv*    envhp = handles().getAs<OCIEnv>(envhpId);
    OCIError*  errhp = handles().getAs<OCIError>(errhpId);
    OCISvcCtx* svchp = handles().getAs<OCISvcCtx>(svchpId);

    if (!envhp || !errhp || !svchp) {
        PA_ReturnLong(params, (PA_long32)OCI_ERROR);
        return;
    }

    sword status = OCICacheRefresh(envhp, errhp, svchp, (OCIRefreshOpt)option,
                                   (void*)(uintptr_t)context, nullptr, nullptr);
    PA_ReturnLong(params, oci_check(status));
}

static void cmd_OCICacheUnmark(PA_PluginParameters params) {
    PA_long32 envhpId = PA_GetLongParameter(params, 1);
    PA_long32 errhpId = PA_GetLongParameter(params, 2);
    PA_long32 svchpId = PA_GetLongParameter(params, 3);

    OCIEnv*    envhp = handles().getAs<OCIEnv>(envhpId);
    OCIError*  errhp = handles().getAs<OCIError>(errhpId);
    OCISvcCtx* svchp = handles().getAs<OCISvcCtx>(svchpId);

    if (!envhp || !errhp || !svchp) {
        PA_ReturnLong(params, (PA_long32)OCI_ERROR);
        return;
    }

    sword status = OCICacheUnmark(envhp, errhp, svchp);
    PA_ReturnLong(params, oci_check(status));
}

static void cmd_OCICacheUnpin(PA_PluginParameters params) {
    PA_long32 envhpId = PA_GetLongParameter(params, 1);
    PA_long32 errhpId = PA_GetLongParameter(params, 2);
    PA_long32 svchpId = PA_GetLongParameter(params, 3);

    OCIEnv*    envhp = handles().getAs<OCIEnv>(envhpId);
    OCIError*  errhp = handles().getAs<OCIError>(errhpId);
    OCISvcCtx* svchp = handles().getAs<OCISvcCtx>(svchpId);

    if (!envhp || !errhp || !svchp) {
        PA_ReturnLong(params, (PA_long32)OCI_ERROR);
        return;
    }

    sword status = OCICacheUnpin(envhp, errhp, svchp);
    PA_ReturnLong(params, oci_check(status));
}

// ---------------------------------------------------------------------------
// Math/OCINumber: 3-operand (err, a, b, result)
// ---------------------------------------------------------------------------

#define IMPL_OCINUMBER_3OP(NAME, OCIFN)                                       \
static void cmd_##NAME(PA_PluginParameters params) {                          \
    PA_long32 errhpId = PA_GetLongParameter(params, 1);                       \
    double num1 = PA_GetDoubleParameter(params, 2);                           \
    double num2 = PA_GetDoubleParameter(params, 3);                           \
                                                                              \
    OCIError* errhp = handles().getAs<OCIError>(errhpId);                     \
    if (!errhp) { PA_ReturnLong(params, (PA_long32)OCI_ERROR); return; }      \
                                                                              \
    OCINumber on1, on2, onResult;                                             \
    sword status = double_to_OCINumber(errhp, num1, &on1);                    \
    if (status != OCI_SUCCESS) { PA_ReturnLong(params, oci_check(status)); return; } \
    status = double_to_OCINumber(errhp, num2, &on2);                          \
    if (status != OCI_SUCCESS) { PA_ReturnLong(params, oci_check(status)); return; } \
                                                                              \
    status = OCIFN(errhp, &on1, &on2, &onResult);                            \
    if (status == OCI_SUCCESS) {                                              \
        double result;                                                        \
        OCINumber_to_double(errhp, &onResult, &result);                       \
        PA_SetDoubleParameter(params, 4, result);                             \
    }                                                                         \
    PA_ReturnLong(params, oci_check(status));                                 \
}

IMPL_OCINUMBER_3OP(OCINumberAdd,     OCINumberAdd)
IMPL_OCINUMBER_3OP(OCINumberSub,     OCINumberSub)
IMPL_OCINUMBER_3OP(OCINumberMul,     OCINumberMul)
IMPL_OCINUMBER_3OP(OCINumberDiv,     OCINumberDiv)
IMPL_OCINUMBER_3OP(OCINumberPower,   OCINumberPower)
IMPL_OCINUMBER_3OP(OCINumberLog,     OCINumberLog)
IMPL_OCINUMBER_3OP(OCINumberArcTan2, OCINumberArcTan2)

// ---------------------------------------------------------------------------
// Math/OCINumber: 2-operand (err, num, result)
// ---------------------------------------------------------------------------

#define IMPL_OCINUMBER_2OP(NAME, OCIFN)                                       \
static void cmd_##NAME(PA_PluginParameters params) {                          \
    PA_long32 errhpId = PA_GetLongParameter(params, 1);                       \
    double num = PA_GetDoubleParameter(params, 2);                            \
                                                                              \
    OCIError* errhp = handles().getAs<OCIError>(errhpId);                     \
    if (!errhp) { PA_ReturnLong(params, (PA_long32)OCI_ERROR); return; }      \
                                                                              \
    OCINumber on, onResult;                                                   \
    sword status = double_to_OCINumber(errhp, num, &on);                      \
    if (status != OCI_SUCCESS) { PA_ReturnLong(params, oci_check(status)); return; } \
                                                                              \
    status = OCIFN(errhp, &on, &onResult);                                    \
    if (status == OCI_SUCCESS) {                                              \
        double result;                                                        \
        OCINumber_to_double(errhp, &onResult, &result);                       \
        PA_SetDoubleParameter(params, 3, result);                             \
    }                                                                         \
    PA_ReturnLong(params, oci_check(status));                                 \
}

IMPL_OCINUMBER_2OP(OCINumberSqrt,    OCINumberSqrt)
IMPL_OCINUMBER_2OP(OCINumberLn,      OCINumberLn)
IMPL_OCINUMBER_2OP(OCINumberExp,     OCINumberExp)
IMPL_OCINUMBER_2OP(OCINumberSin,     OCINumberSin)
IMPL_OCINUMBER_2OP(OCINumberCos,     OCINumberCos)
IMPL_OCINUMBER_2OP(OCINumberTan,     OCINumberTan)
IMPL_OCINUMBER_2OP(OCINumberArcSin,  OCINumberArcSin)
IMPL_OCINUMBER_2OP(OCINumberArcCos,  OCINumberArcCos)
IMPL_OCINUMBER_2OP(OCINumberArcTan,  OCINumberArcTan)
IMPL_OCINUMBER_2OP(OCINumberHypSin,  OCINumberHypSin)
IMPL_OCINUMBER_2OP(OCINumberHypCos,  OCINumberHypCos)
IMPL_OCINUMBER_2OP(OCINumberHypTan,  OCINumberHypTan)

// ---------------------------------------------------------------------------
// Math/OCINumber: special cases
// ---------------------------------------------------------------------------

static void cmd_OCINumberTrunc(PA_PluginParameters params) {
    PA_long32 errhpId  = PA_GetLongParameter(params, 1);
    double num         = PA_GetDoubleParameter(params, 2);
    PA_long32 decplace = PA_GetLongParameter(params, 3);

    OCIError* errhp = handles().getAs<OCIError>(errhpId);
    if (!errhp) { PA_ReturnLong(params, (PA_long32)OCI_ERROR); return; }

    OCINumber on, onResult;
    sword status = double_to_OCINumber(errhp, num, &on);
    if (status != OCI_SUCCESS) { PA_ReturnLong(params, oci_check(status)); return; }

    status = OCINumberTrunc(errhp, &on, (sword)decplace, &onResult);
    if (status == OCI_SUCCESS) {
        double result;
        OCINumber_to_double(errhp, &onResult, &result);
        PA_SetDoubleParameter(params, 4, result);
    }
    PA_ReturnLong(params, oci_check(status));
}

static void cmd_OCINumberRound(PA_PluginParameters params) {
    PA_long32 errhpId  = PA_GetLongParameter(params, 1);
    double num         = PA_GetDoubleParameter(params, 2);
    PA_long32 decplace = PA_GetLongParameter(params, 3);

    OCIError* errhp = handles().getAs<OCIError>(errhpId);
    if (!errhp) { PA_ReturnLong(params, (PA_long32)OCI_ERROR); return; }

    OCINumber on, onResult;
    sword status = double_to_OCINumber(errhp, num, &on);
    if (status != OCI_SUCCESS) { PA_ReturnLong(params, oci_check(status)); return; }

    status = OCINumberRound(errhp, &on, (sword)decplace, &onResult);
    if (status == OCI_SUCCESS) {
        double result;
        OCINumber_to_double(errhp, &onResult, &result);
        PA_SetDoubleParameter(params, 4, result);
    }
    PA_ReturnLong(params, oci_check(status));
}

static void cmd_OCINumberIntPower(PA_PluginParameters params) {
    PA_long32 errhpId = PA_GetLongParameter(params, 1);
    double base       = PA_GetDoubleParameter(params, 2);
    PA_long32 exp_int = PA_GetLongParameter(params, 3);

    OCIError* errhp = handles().getAs<OCIError>(errhpId);
    if (!errhp) { PA_ReturnLong(params, (PA_long32)OCI_ERROR); return; }

    OCINumber onBase, onResult;
    sword status = double_to_OCINumber(errhp, base, &onBase);
    if (status != OCI_SUCCESS) { PA_ReturnLong(params, oci_check(status)); return; }

    status = OCINumberIntPower(errhp, &onBase, (sword)exp_int, &onResult);
    if (status == OCI_SUCCESS) {
        double result;
        OCINumber_to_double(errhp, &onResult, &result);
        PA_SetDoubleParameter(params, 4, result);
    }
    PA_ReturnLong(params, oci_check(status));
}

static void cmd_OCINumberFromText(PA_PluginParameters params) {
    PA_long32 errhpId = PA_GetLongParameter(params, 1);
    PA_Unistring* uStr = PA_GetStringParameter(params, 2);
    PA_Unistring* uFmt = PA_GetStringParameter(params, 3);
    PA_Unistring* uNls = PA_GetStringParameter(params, 4);

    OCIError* errhp = handles().getAs<OCIError>(errhpId);
    if (!errhp) { PA_ReturnLong(params, (PA_long32)OCI_ERROR); return; }

    std::string str = unistr_to_utf8(uStr);
    std::string fmt = unistr_to_utf8(uFmt);
    std::string nls = unistr_to_utf8(uNls);

    OCINumber onResult;
    sword status = OCINumberFromText(errhp,
                                     (const OraText*)str.c_str(), (ub4)str.size(),
                                     (const OraText*)fmt.c_str(), (ub4)fmt.size(),
                                     (const OraText*)(nls.empty() ? nullptr : nls.c_str()),
                                     (ub4)nls.size(),
                                     &onResult);
    if (status == OCI_SUCCESS) {
        double result;
        OCINumber_to_double(errhp, &onResult, &result);
        PA_SetDoubleParameter(params, 5, result);
    }
    PA_ReturnLong(params, oci_check(status));
}

static void cmd_OCINumberToText(PA_PluginParameters params) {
    PA_long32 errhpId = PA_GetLongParameter(params, 1);
    double num        = PA_GetDoubleParameter(params, 2);
    PA_Unistring* uFmt = PA_GetStringParameter(params, 3);
    PA_Unistring* uNls = PA_GetStringParameter(params, 4);

    OCIError* errhp = handles().getAs<OCIError>(errhpId);
    if (!errhp) { PA_ReturnLong(params, (PA_long32)OCI_ERROR); return; }

    std::string fmt = unistr_to_utf8(uFmt);
    std::string nls = unistr_to_utf8(uNls);

    OCINumber on;
    sword status = double_to_OCINumber(errhp, num, &on);
    if (status != OCI_SUCCESS) { PA_ReturnLong(params, oci_check(status)); return; }

    OraText buf[256];
    ub4 bufLen = sizeof(buf);
    status = OCINumberToText(errhp, &on,
                             (const OraText*)fmt.c_str(), (ub4)fmt.size(),
                             (const OraText*)(nls.empty() ? nullptr : nls.c_str()),
                             (ub4)nls.size(),
                             &bufLen, buf);
    if (status == OCI_SUCCESS) {
        PA_Pointer ptr = PA_GetPointerParameter(params, 5);
        if (ptr) {
            PA_Unistring ustr = utf8_to_unistring(std::string((const char*)buf, bufLen));
            PA_Variable var = PA_CreateVariable(eVK_Unistring);
            PA_SetStringVariable(&var, &ustr);
            PA_SetPointerValue(ptr, var);
        }
    }
    PA_ReturnLong(params, oci_check(status));
}

// ============================================================
// LOB commands
// ============================================================

// OCILobAppend(svchp; errhp; dst_locp; src_locp) : status
static void cmd_OCILobAppend(PA_PluginParameters params) {
    PA_long32 svchpId  = PA_GetLongParameter(params, 1);
    PA_long32 errhpId  = PA_GetLongParameter(params, 2);
    PA_long32 dstId    = PA_GetLongParameter(params, 3);
    PA_long32 srcId    = PA_GetLongParameter(params, 4);

    OCISvcCtx*     svchp = handles().getAs<OCISvcCtx>(svchpId);
    OCIError*      errhp = handles().getAs<OCIError>(errhpId);
    OCILobLocator* dst   = (OCILobLocator*)handles().get(dstId);
    OCILobLocator* src   = (OCILobLocator*)handles().get(srcId);

    if (!svchp || !errhp || !dst || !src) {
        PA_ReturnLong(params, (PA_long32)OCI_ERROR); return;
    }
    sword status = OCILobAppend(svchp, errhp, dst, src);
    PA_ReturnLong(params, oci_check(status));
}

// OCILobAssign(envhp; errhp; src_locp; dst_locp) : status
static void cmd_OCILobAssign(PA_PluginParameters params) {
    PA_long32 envhpId = PA_GetLongParameter(params, 1);
    PA_long32 errhpId = PA_GetLongParameter(params, 2);
    PA_long32 srcId   = PA_GetLongParameter(params, 3);
    PA_long32 dstId   = PA_GetLongParameter(params, 4);

    OCIEnv*        envhp = handles().getAs<OCIEnv>(envhpId);
    OCIError*      errhp = handles().getAs<OCIError>(errhpId);
    OCILobLocator* src   = (OCILobLocator*)handles().get(srcId);
    OCILobLocator* dst   = (OCILobLocator*)handles().get(dstId);

    if (!envhp || !errhp || !src || !dst) {
        PA_ReturnLong(params, (PA_long32)OCI_ERROR); return;
    }
    sword status = OCILobAssign(envhp, errhp, (const OCILobLocator*)src, &dst);
    PA_ReturnLong(params, oci_check(status));
}

// OCILobCharSetForm(envhp; errhp; locp; csform_out) : status
static void cmd_OCILobCharSetForm(PA_PluginParameters params) {
    PA_long32 envhpId = PA_GetLongParameter(params, 1);
    PA_long32 errhpId = PA_GetLongParameter(params, 2);
    PA_long32 locpId  = PA_GetLongParameter(params, 3);

    OCIEnv*        envhp = handles().getAs<OCIEnv>(envhpId);
    OCIError*      errhp = handles().getAs<OCIError>(errhpId);
    OCILobLocator* locp  = (OCILobLocator*)handles().get(locpId);

    if (!envhp || !errhp || !locp) {
        PA_ReturnLong(params, (PA_long32)OCI_ERROR); return;
    }
    ub1 csform = 0;
    sword status = OCILobCharSetForm(envhp, errhp, (const OCILobLocator*)locp, &csform);
    PA_SetLongParameter(params, 4, (PA_long32)csform);
    PA_ReturnLong(params, oci_check(status));
}

// OCILobCharSetId(envhp; errhp; locp; csid_out) : status
static void cmd_OCILobCharSetId(PA_PluginParameters params) {
    PA_long32 envhpId = PA_GetLongParameter(params, 1);
    PA_long32 errhpId = PA_GetLongParameter(params, 2);
    PA_long32 locpId  = PA_GetLongParameter(params, 3);

    OCIEnv*        envhp = handles().getAs<OCIEnv>(envhpId);
    OCIError*      errhp = handles().getAs<OCIError>(errhpId);
    OCILobLocator* locp  = (OCILobLocator*)handles().get(locpId);

    if (!envhp || !errhp || !locp) {
        PA_ReturnLong(params, (PA_long32)OCI_ERROR); return;
    }
    ub2 csid = 0;
    sword status = OCILobCharSetId(envhp, errhp, (const OCILobLocator*)locp, &csid);
    PA_SetLongParameter(params, 4, (PA_long32)csid);
    PA_ReturnLong(params, oci_check(status));
}

// OCILobCopy(svchp; errhp; dst_locp; src_locp; amount; dst_offset; src_offset) : status
static void cmd_OCILobCopy(PA_PluginParameters params) {
    PA_long32 svchpId   = PA_GetLongParameter(params, 1);
    PA_long32 errhpId   = PA_GetLongParameter(params, 2);
    PA_long32 dstId     = PA_GetLongParameter(params, 3);
    PA_long32 srcId     = PA_GetLongParameter(params, 4);
    PA_long32 amount    = PA_GetLongParameter(params, 5);
    PA_long32 dstOffset = PA_GetLongParameter(params, 6);
    PA_long32 srcOffset = PA_GetLongParameter(params, 7);

    OCISvcCtx*     svchp = handles().getAs<OCISvcCtx>(svchpId);
    OCIError*      errhp = handles().getAs<OCIError>(errhpId);
    OCILobLocator* dst   = (OCILobLocator*)handles().get(dstId);
    OCILobLocator* src   = (OCILobLocator*)handles().get(srcId);

    if (!svchp || !errhp || !dst || !src) {
        PA_ReturnLong(params, (PA_long32)OCI_ERROR); return;
    }
    sword status = OCILobCopy(svchp, errhp, dst, src, (ub4)amount, (ub4)dstOffset, (ub4)srcOffset);
    PA_ReturnLong(params, oci_check(status));
}

// OCILobCreateTemporary(svchp; errhp; locp; csid; csform; lobtype) : status
static void cmd_OCILobCreateTemporary(PA_PluginParameters params) {
    PA_long32 svchpId  = PA_GetLongParameter(params, 1);
    PA_long32 errhpId  = PA_GetLongParameter(params, 2);
    PA_long32 locpId   = PA_GetLongParameter(params, 3);
    PA_long32 csid     = PA_GetLongParameter(params, 4);
    PA_long32 csform   = PA_GetLongParameter(params, 5);
    PA_long32 lobtype  = PA_GetLongParameter(params, 6);

    OCISvcCtx*     svchp = handles().getAs<OCISvcCtx>(svchpId);
    OCIError*      errhp = handles().getAs<OCIError>(errhpId);
    OCILobLocator* locp  = (OCILobLocator*)handles().get(locpId);

    if (!svchp || !errhp || !locp) {
        PA_ReturnLong(params, (PA_long32)OCI_ERROR); return;
    }
    sword status = OCILobCreateTemporary(svchp, errhp, locp,
        (ub2)csid, (ub1)csform, (ub1)lobtype, FALSE, OCI_DURATION_SESSION);
    PA_ReturnLong(params, oci_check(status));
}

// OCILobDisableBuffering(svchp; errhp; locp) : status
static void cmd_OCILobDisableBuffering(PA_PluginParameters params) {
    PA_long32 svchpId = PA_GetLongParameter(params, 1);
    PA_long32 errhpId = PA_GetLongParameter(params, 2);
    PA_long32 locpId  = PA_GetLongParameter(params, 3);

    OCISvcCtx*     svchp = handles().getAs<OCISvcCtx>(svchpId);
    OCIError*      errhp = handles().getAs<OCIError>(errhpId);
    OCILobLocator* locp  = (OCILobLocator*)handles().get(locpId);

    if (!svchp || !errhp || !locp) {
        PA_ReturnLong(params, (PA_long32)OCI_ERROR); return;
    }
    sword status = OCILobDisableBuffering(svchp, errhp, locp);
    PA_ReturnLong(params, oci_check(status));
}

// OCILobEnableBuffering(svchp; errhp; locp) : status
static void cmd_OCILobEnableBuffering(PA_PluginParameters params) {
    PA_long32 svchpId = PA_GetLongParameter(params, 1);
    PA_long32 errhpId = PA_GetLongParameter(params, 2);
    PA_long32 locpId  = PA_GetLongParameter(params, 3);

    OCISvcCtx*     svchp = handles().getAs<OCISvcCtx>(svchpId);
    OCIError*      errhp = handles().getAs<OCIError>(errhpId);
    OCILobLocator* locp  = (OCILobLocator*)handles().get(locpId);

    if (!svchp || !errhp || !locp) {
        PA_ReturnLong(params, (PA_long32)OCI_ERROR); return;
    }
    sword status = OCILobEnableBuffering(svchp, errhp, locp);
    PA_ReturnLong(params, oci_check(status));
}

// OCILobErase(svchp; errhp; locp; amount; offset) : status
static void cmd_OCILobErase(PA_PluginParameters params) {
    PA_long32 svchpId = PA_GetLongParameter(params, 1);
    PA_long32 errhpId = PA_GetLongParameter(params, 2);
    PA_long32 locpId  = PA_GetLongParameter(params, 3);
    PA_long32 amount  = PA_GetLongParameter(params, 4);
    PA_long32 offset  = PA_GetLongParameter(params, 5);

    OCISvcCtx*     svchp = handles().getAs<OCISvcCtx>(svchpId);
    OCIError*      errhp = handles().getAs<OCIError>(errhpId);
    OCILobLocator* locp  = (OCILobLocator*)handles().get(locpId);

    if (!svchp || !errhp || !locp) {
        PA_ReturnLong(params, (PA_long32)OCI_ERROR); return;
    }
    ub4 amtp = (ub4)amount;
    sword status = OCILobErase(svchp, errhp, locp, &amtp, (ub4)offset);
    PA_SetLongParameter(params, 4, (PA_long32)amtp);
    PA_ReturnLong(params, oci_check(status));
}

// OCILobFileClose(svchp; errhp; filep) : status
static void cmd_OCILobFileClose(PA_PluginParameters params) {
    PA_long32 svchpId = PA_GetLongParameter(params, 1);
    PA_long32 errhpId = PA_GetLongParameter(params, 2);
    PA_long32 filepId = PA_GetLongParameter(params, 3);

    OCISvcCtx*     svchp = handles().getAs<OCISvcCtx>(svchpId);
    OCIError*      errhp = handles().getAs<OCIError>(errhpId);
    OCILobLocator* filep = (OCILobLocator*)handles().get(filepId);

    if (!svchp || !errhp || !filep) {
        PA_ReturnLong(params, (PA_long32)OCI_ERROR); return;
    }
    sword status = OCILobFileClose(svchp, errhp, filep);
    PA_ReturnLong(params, oci_check(status));
}

// OCILobFileCloseAll(svchp; errhp) : status
static void cmd_OCILobFileCloseAll(PA_PluginParameters params) {
    PA_long32 svchpId = PA_GetLongParameter(params, 1);
    PA_long32 errhpId = PA_GetLongParameter(params, 2);

    OCISvcCtx* svchp = handles().getAs<OCISvcCtx>(svchpId);
    OCIError*  errhp = handles().getAs<OCIError>(errhpId);

    if (!svchp || !errhp) {
        PA_ReturnLong(params, (PA_long32)OCI_ERROR); return;
    }
    sword status = OCILobFileCloseAll(svchp, errhp);
    PA_ReturnLong(params, oci_check(status));
}

// OCILobFileExists(svchp; errhp; filep; flag_out) : status
static void cmd_OCILobFileExists(PA_PluginParameters params) {
    PA_long32 svchpId = PA_GetLongParameter(params, 1);
    PA_long32 errhpId = PA_GetLongParameter(params, 2);
    PA_long32 filepId = PA_GetLongParameter(params, 3);

    OCISvcCtx*     svchp = handles().getAs<OCISvcCtx>(svchpId);
    OCIError*      errhp = handles().getAs<OCIError>(errhpId);
    OCILobLocator* filep = (OCILobLocator*)handles().get(filepId);

    if (!svchp || !errhp || !filep) {
        PA_ReturnLong(params, (PA_long32)OCI_ERROR); return;
    }
    boolean flag = FALSE;
    sword status = OCILobFileExists(svchp, errhp, filep, &flag);
    PA_SetLongParameter(params, 4, (PA_long32)flag);
    PA_ReturnLong(params, oci_check(status));
}

// OCILobFileGetName(envhp; errhp; filep; dir_ptr; fname_ptr) : status
static void cmd_OCILobFileGetName(PA_PluginParameters params) {
    PA_long32 envhpId = PA_GetLongParameter(params, 1);
    PA_long32 errhpId = PA_GetLongParameter(params, 2);
    PA_long32 filepId = PA_GetLongParameter(params, 3);

    OCIEnv*        envhp = handles().getAs<OCIEnv>(envhpId);
    OCIError*      errhp = handles().getAs<OCIError>(errhpId);
    OCILobLocator* filep = (OCILobLocator*)handles().get(filepId);

    if (!envhp || !errhp || !filep) {
        PA_ReturnLong(params, (PA_long32)OCI_ERROR); return;
    }
    OraText dirBuf[256], fnameBuf[256];
    ub2 dirLen = sizeof(dirBuf), fnameLen = sizeof(fnameBuf);
    sword status = OCILobFileGetName(envhp, errhp, (const OCILobLocator*)filep,
        dirBuf, &dirLen, fnameBuf, &fnameLen);
    if (status == OCI_SUCCESS) {
        set_pointer_text(params, 4, (const char*)dirBuf, (PA_long32)dirLen);
        set_pointer_text(params, 5, (const char*)fnameBuf, (PA_long32)fnameLen);
    }
    PA_ReturnLong(params, oci_check(status));
}

// OCILobFileIsOpen(svchp; errhp; filep; flag_out) : status
static void cmd_OCILobFileIsOpen(PA_PluginParameters params) {
    PA_long32 svchpId = PA_GetLongParameter(params, 1);
    PA_long32 errhpId = PA_GetLongParameter(params, 2);
    PA_long32 filepId = PA_GetLongParameter(params, 3);

    OCISvcCtx*     svchp = handles().getAs<OCISvcCtx>(svchpId);
    OCIError*      errhp = handles().getAs<OCIError>(errhpId);
    OCILobLocator* filep = (OCILobLocator*)handles().get(filepId);

    if (!svchp || !errhp || !filep) {
        PA_ReturnLong(params, (PA_long32)OCI_ERROR); return;
    }
    boolean flag = FALSE;
    sword status = OCILobFileIsOpen(svchp, errhp, filep, &flag);
    PA_SetLongParameter(params, 4, (PA_long32)flag);
    PA_ReturnLong(params, oci_check(status));
}

// OCILobFileOpen(svchp; errhp; filep; mode) : status
static void cmd_OCILobFileOpen(PA_PluginParameters params) {
    PA_long32 svchpId = PA_GetLongParameter(params, 1);
    PA_long32 errhpId = PA_GetLongParameter(params, 2);
    PA_long32 filepId = PA_GetLongParameter(params, 3);
    PA_long32 mode    = PA_GetLongParameter(params, 4);

    OCISvcCtx*     svchp = handles().getAs<OCISvcCtx>(svchpId);
    OCIError*      errhp = handles().getAs<OCIError>(errhpId);
    OCILobLocator* filep = (OCILobLocator*)handles().get(filepId);

    if (!svchp || !errhp || !filep) {
        PA_ReturnLong(params, (PA_long32)OCI_ERROR); return;
    }
    sword status = OCILobFileOpen(svchp, errhp, filep, (ub1)mode);
    PA_ReturnLong(params, oci_check(status));
}

// OCILobFileSetName(envhp; errhp; filep; dir_alias; filename) : status
static void cmd_OCILobFileSetName(PA_PluginParameters params) {
    PA_long32 envhpId = PA_GetLongParameter(params, 1);
    PA_long32 errhpId = PA_GetLongParameter(params, 2);
    PA_long32 filepId = PA_GetLongParameter(params, 3);

    OCIEnv*        envhp = handles().getAs<OCIEnv>(envhpId);
    OCIError*      errhp = handles().getAs<OCIError>(errhpId);
    OCILobLocator* filep = (OCILobLocator*)handles().get(filepId);

    if (!envhp || !errhp || !filep) {
        PA_ReturnLong(params, (PA_long32)OCI_ERROR); return;
    }
    std::string dir   = unistr_to_utf8(PA_GetStringParameter(params, 4));
    std::string fname = unistr_to_utf8(PA_GetStringParameter(params, 5));
    sword status = OCILobFileSetName(envhp, errhp, &filep,
        (OraText*)dir.c_str(), (ub2)dir.size(),
        (OraText*)fname.c_str(), (ub2)fname.size());
    PA_ReturnLong(params, oci_check(status));
}

// OCILobFlushBuffer(svchp; errhp; locp; flag) : status
static void cmd_OCILobFlushBuffer(PA_PluginParameters params) {
    PA_long32 svchpId = PA_GetLongParameter(params, 1);
    PA_long32 errhpId = PA_GetLongParameter(params, 2);
    PA_long32 locpId  = PA_GetLongParameter(params, 3);
    PA_long32 flag    = PA_GetLongParameter(params, 4);

    OCISvcCtx*     svchp = handles().getAs<OCISvcCtx>(svchpId);
    OCIError*      errhp = handles().getAs<OCIError>(errhpId);
    OCILobLocator* locp  = (OCILobLocator*)handles().get(locpId);

    if (!svchp || !errhp || !locp) {
        PA_ReturnLong(params, (PA_long32)OCI_ERROR); return;
    }
    sword status = OCILobFlushBuffer(svchp, errhp, locp, (ub4)flag);
    PA_ReturnLong(params, oci_check(status));
}

// OCILobFreeTemporary(svchp; errhp; locp) : status
static void cmd_OCILobFreeTemporary(PA_PluginParameters params) {
    PA_long32 svchpId = PA_GetLongParameter(params, 1);
    PA_long32 errhpId = PA_GetLongParameter(params, 2);
    PA_long32 locpId  = PA_GetLongParameter(params, 3);

    OCISvcCtx*     svchp = handles().getAs<OCISvcCtx>(svchpId);
    OCIError*      errhp = handles().getAs<OCIError>(errhpId);
    OCILobLocator* locp  = (OCILobLocator*)handles().get(locpId);

    if (!svchp || !errhp || !locp) {
        PA_ReturnLong(params, (PA_long32)OCI_ERROR); return;
    }
    sword status = OCILobFreeTemporary(svchp, errhp, locp);
    PA_ReturnLong(params, oci_check(status));
}

// OCILobGetChunkSize(svchp; errhp; locp; chunk_size_out) : status
static void cmd_OCILobGetChunkSize(PA_PluginParameters params) {
    PA_long32 svchpId = PA_GetLongParameter(params, 1);
    PA_long32 errhpId = PA_GetLongParameter(params, 2);
    PA_long32 locpId  = PA_GetLongParameter(params, 3);

    OCISvcCtx*     svchp = handles().getAs<OCISvcCtx>(svchpId);
    OCIError*      errhp = handles().getAs<OCIError>(errhpId);
    OCILobLocator* locp  = (OCILobLocator*)handles().get(locpId);

    if (!svchp || !errhp || !locp) {
        PA_ReturnLong(params, (PA_long32)OCI_ERROR); return;
    }
    ub4 chunkSize = 0;
    sword status = OCILobGetChunkSize(svchp, errhp, locp, &chunkSize);
    PA_SetLongParameter(params, 4, (PA_long32)chunkSize);
    PA_ReturnLong(params, oci_check(status));
}

// OCILobGetLength(svchp; errhp; locp; length_out) : status
static void cmd_OCILobGetLength(PA_PluginParameters params) {
    PA_long32 svchpId = PA_GetLongParameter(params, 1);
    PA_long32 errhpId = PA_GetLongParameter(params, 2);
    PA_long32 locpId  = PA_GetLongParameter(params, 3);

    OCISvcCtx*     svchp = handles().getAs<OCISvcCtx>(svchpId);
    OCIError*      errhp = handles().getAs<OCIError>(errhpId);
    OCILobLocator* locp  = (OCILobLocator*)handles().get(locpId);

    if (!svchp || !errhp || !locp) {
        PA_ReturnLong(params, (PA_long32)OCI_ERROR); return;
    }
    ub4 lenp = 0;
    sword status = OCILobGetLength(svchp, errhp, locp, &lenp);
    PA_SetLongParameter(params, 4, (PA_long32)lenp);
    PA_ReturnLong(params, oci_check(status));
}

// OCILobIsEqual(envhp; x; y; is_equal_out) : status
static void cmd_OCILobIsEqual(PA_PluginParameters params) {
    PA_long32 envhpId = PA_GetLongParameter(params, 1);
    PA_long32 xId     = PA_GetLongParameter(params, 2);
    PA_long32 yId     = PA_GetLongParameter(params, 3);

    OCIEnv*        envhp = handles().getAs<OCIEnv>(envhpId);
    OCILobLocator* x     = (OCILobLocator*)handles().get(xId);
    OCILobLocator* y     = (OCILobLocator*)handles().get(yId);

    if (!envhp || !x || !y) {
        PA_ReturnLong(params, (PA_long32)OCI_ERROR); return;
    }
    boolean is_equal = FALSE;
    OCILobIsEqual(envhp, (const OCILobLocator*)x, (const OCILobLocator*)y, &is_equal);
    PA_SetLongParameter(params, 4, (PA_long32)is_equal);
    PA_ReturnLong(params, (PA_long32)OCI_SUCCESS);
}

// OCILobIsTemporary(envhp; errhp; locp; is_temporary_out) : status
static void cmd_OCILobIsTemporary(PA_PluginParameters params) {
    PA_long32 envhpId = PA_GetLongParameter(params, 1);
    PA_long32 errhpId = PA_GetLongParameter(params, 2);
    PA_long32 locpId  = PA_GetLongParameter(params, 3);

    OCIEnv*        envhp = handles().getAs<OCIEnv>(envhpId);
    OCIError*      errhp = handles().getAs<OCIError>(errhpId);
    OCILobLocator* locp  = (OCILobLocator*)handles().get(locpId);

    if (!envhp || !errhp || !locp) {
        PA_ReturnLong(params, (PA_long32)OCI_ERROR); return;
    }
    boolean is_temp = FALSE;
    sword status = OCILobIsTemporary(envhp, errhp, locp, &is_temp);
    PA_SetLongParameter(params, 4, (PA_long32)is_temp);
    PA_ReturnLong(params, oci_check(status));
}

// OCILobLoadFromFile(svchp; errhp; dst_locp; src_filep; amount; dst_offset; src_offset) : status
static void cmd_OCILobLoadFromFile(PA_PluginParameters params) {
    PA_long32 svchpId   = PA_GetLongParameter(params, 1);
    PA_long32 errhpId   = PA_GetLongParameter(params, 2);
    PA_long32 dstId     = PA_GetLongParameter(params, 3);
    PA_long32 srcId     = PA_GetLongParameter(params, 4);
    PA_long32 amount    = PA_GetLongParameter(params, 5);
    PA_long32 dstOffset = PA_GetLongParameter(params, 6);
    PA_long32 srcOffset = PA_GetLongParameter(params, 7);

    OCISvcCtx*     svchp = handles().getAs<OCISvcCtx>(svchpId);
    OCIError*      errhp = handles().getAs<OCIError>(errhpId);
    OCILobLocator* dst   = (OCILobLocator*)handles().get(dstId);
    OCILobLocator* src   = (OCILobLocator*)handles().get(srcId);

    if (!svchp || !errhp || !dst || !src) {
        PA_ReturnLong(params, (PA_long32)OCI_ERROR); return;
    }
    sword status = OCILobLoadFromFile(svchp, errhp, dst, src,
        (ub4)amount, (ub4)dstOffset, (ub4)srcOffset);
    PA_ReturnLong(params, oci_check(status));
}

// OCILobLocatorIsInit(envhp; errhp; locp; is_initialized_out) : status
static void cmd_OCILobLocatorIsInit(PA_PluginParameters params) {
    PA_long32 envhpId = PA_GetLongParameter(params, 1);
    PA_long32 errhpId = PA_GetLongParameter(params, 2);
    PA_long32 locpId  = PA_GetLongParameter(params, 3);

    OCIEnv*        envhp = handles().getAs<OCIEnv>(envhpId);
    OCIError*      errhp = handles().getAs<OCIError>(errhpId);
    OCILobLocator* locp  = (OCILobLocator*)handles().get(locpId);

    if (!envhp || !errhp || !locp) {
        PA_ReturnLong(params, (PA_long32)OCI_ERROR); return;
    }
    boolean is_init = FALSE;
    sword status = OCILobLocatorIsInit(envhp, errhp, (const OCILobLocator*)locp, &is_init);
    PA_SetLongParameter(params, 4, (PA_long32)is_init);
    PA_ReturnLong(params, oci_check(status));
}

// OCILobRead(svchp; errhp; locp; offset; blob_out) : status
static void cmd_OCILobRead(PA_PluginParameters params) {
    PA_long32 svchpId = PA_GetLongParameter(params, 1);
    PA_long32 errhpId = PA_GetLongParameter(params, 2);
    PA_long32 locpId  = PA_GetLongParameter(params, 3);
    PA_long32 offset  = PA_GetLongParameter(params, 4);

    OCISvcCtx*     svchp = handles().getAs<OCISvcCtx>(svchpId);
    OCIError*      errhp = handles().getAs<OCIError>(errhpId);
    OCILobLocator* locp  = (OCILobLocator*)handles().get(locpId);

    if (!svchp || !errhp || !locp) {
        PA_ReturnLong(params, (PA_long32)OCI_ERROR); return;
    }

    ub4 lobLen = 0;
    sword status = OCILobGetLength(svchp, errhp, locp, &lobLen);
    if (status != OCI_SUCCESS || lobLen == 0) {
        PA_SetBlobParameter(params, 5, nullptr, 0);
        PA_ReturnLong(params, oci_check(status));
        return;
    }

    std::vector<ub1> buf(lobLen);
    ub4 amtp = lobLen;
    status = OCILobRead(svchp, errhp, locp, &amtp, (ub4)offset,
                        buf.data(), (ub4)buf.size(), nullptr, nullptr, 0, SQLCS_IMPLICIT);
    if (status == OCI_SUCCESS || status == OCI_NEED_DATA) {
        PA_SetBlobParameter(params, 5, buf.data(), (PA_long32)amtp);
    }
    PA_ReturnLong(params, oci_check(status));
}

// OCILobTrim(svchp; errhp; locp; newlen) : status
static void cmd_OCILobTrim(PA_PluginParameters params) {
    PA_long32 svchpId = PA_GetLongParameter(params, 1);
    PA_long32 errhpId = PA_GetLongParameter(params, 2);
    PA_long32 locpId  = PA_GetLongParameter(params, 3);
    PA_long32 newlen  = PA_GetLongParameter(params, 4);

    OCISvcCtx*     svchp = handles().getAs<OCISvcCtx>(svchpId);
    OCIError*      errhp = handles().getAs<OCIError>(errhpId);
    OCILobLocator* locp  = (OCILobLocator*)handles().get(locpId);

    if (!svchp || !errhp || !locp) {
        PA_ReturnLong(params, (PA_long32)OCI_ERROR); return;
    }
    sword status = OCILobTrim(svchp, errhp, locp, (ub4)newlen);
    PA_ReturnLong(params, oci_check(status));
}

// OCILobWrite(svchp; errhp; locp; offset; blob_in) : status
static void cmd_OCILobWrite(PA_PluginParameters params) {
    PA_long32 svchpId = PA_GetLongParameter(params, 1);
    PA_long32 errhpId = PA_GetLongParameter(params, 2);
    PA_long32 locpId  = PA_GetLongParameter(params, 3);
    PA_long32 offset  = PA_GetLongParameter(params, 4);

    OCISvcCtx*     svchp = handles().getAs<OCISvcCtx>(svchpId);
    OCIError*      errhp = handles().getAs<OCIError>(errhpId);
    OCILobLocator* locp  = (OCILobLocator*)handles().get(locpId);

    if (!svchp || !errhp || !locp) {
        PA_ReturnLong(params, (PA_long32)OCI_ERROR); return;
    }

    PA_Handle hBlob = PA_GetBlobHandleParameter(params, 5);
    PA_long32 blobLen = PA_GetHandleSize(hBlob);
    char* blobPtr = PA_LockHandle(hBlob);

    ub4 amtp = (ub4)blobLen;
    sword status = OCILobWrite(svchp, errhp, locp, &amtp, (ub4)offset,
                               blobPtr, (ub4)blobLen, OCI_ONE_PIECE,
                               nullptr, nullptr, 0, SQLCS_IMPLICIT);
    PA_UnlockHandle(hBlob);
    PA_ReturnLong(params, oci_check(status));
}

// OCILobWriteAppend(svchp; errhp; locp; blob_in) : status
static void cmd_OCILobWriteAppend(PA_PluginParameters params) {
    PA_long32 svchpId = PA_GetLongParameter(params, 1);
    PA_long32 errhpId = PA_GetLongParameter(params, 2);
    PA_long32 locpId  = PA_GetLongParameter(params, 3);

    OCISvcCtx*     svchp = handles().getAs<OCISvcCtx>(svchpId);
    OCIError*      errhp = handles().getAs<OCIError>(errhpId);
    OCILobLocator* locp  = (OCILobLocator*)handles().get(locpId);

    if (!svchp || !errhp || !locp) {
        PA_ReturnLong(params, (PA_long32)OCI_ERROR); return;
    }

    PA_Handle hBlob = PA_GetBlobHandleParameter(params, 4);
    PA_long32 blobLen = PA_GetHandleSize(hBlob);
    char* blobPtr = PA_LockHandle(hBlob);

    ub4 amtp = (ub4)blobLen;
    sword status = OCILobWriteAppend(svchp, errhp, locp, &amtp,
                                     blobPtr, (ub4)blobLen, OCI_ONE_PIECE,
                                     nullptr, nullptr, 0, SQLCS_IMPLICIT);
    PA_UnlockHandle(hBlob);
    PA_ReturnLong(params, oci_check(status));
}

// OCIDurationBegin(envhp; errhp; svchp; parent_duration) : status
static void cmd_OCIDurationBegin(PA_PluginParameters params) {
    PA_long32 envhpId  = PA_GetLongParameter(params, 1);
    PA_long32 errhpId  = PA_GetLongParameter(params, 2);
    PA_long32 svchpId  = PA_GetLongParameter(params, 3);
    PA_long32 parent   = PA_GetLongParameter(params, 4);

    OCIEnv*    envhp = handles().getAs<OCIEnv>(envhpId);
    OCIError*  errhp = handles().getAs<OCIError>(errhpId);
    OCISvcCtx* svchp = handles().getAs<OCISvcCtx>(svchpId);

    if (!envhp || !errhp || !svchp) {
        PA_ReturnLong(params, (PA_long32)OCI_ERROR); return;
    }
    OCIDuration dur = 0;
    sword status = OCIDurationBegin(envhp, errhp, svchp, (OCIDuration)parent, &dur);
    PA_SetLongParameter(params, 4, (PA_long32)dur);
    PA_ReturnLong(params, oci_check(status));
}

// OCIDurationEnd(envhp; errhp; svchp; duration) : status
static void cmd_OCIDurationEnd(PA_PluginParameters params) {
    PA_long32 envhpId  = PA_GetLongParameter(params, 1);
    PA_long32 errhpId  = PA_GetLongParameter(params, 2);
    PA_long32 svchpId  = PA_GetLongParameter(params, 3);
    PA_long32 duration = PA_GetLongParameter(params, 4);

    OCIEnv*    envhp = handles().getAs<OCIEnv>(envhpId);
    OCIError*  errhp = handles().getAs<OCIError>(errhpId);
    OCISvcCtx* svchp = handles().getAs<OCISvcCtx>(svchpId);

    if (!envhp || !errhp || !svchp) {
        PA_ReturnLong(params, (PA_long32)OCI_ERROR); return;
    }
    sword status = OCIDurationEnd(envhp, errhp, svchp, (OCIDuration)duration);
    PA_ReturnLong(params, oci_check(status));
}

// ============================================================
// DATE commands
// ============================================================

// OCIDateAddDays(errhp; date_in; num_days; date_out) : status
static void cmd_OCIDateAddDays(PA_PluginParameters params) {
    PA_long32 errhpId = PA_GetLongParameter(params, 1);
    OCIError* errhp = handles().getAs<OCIError>(errhpId);
    if (!errhp) { PA_ReturnLong(params, (PA_long32)OCI_ERROR); return; }

    OCIDate dateIn;
    date_param_to_ocidate(params, 2, &dateIn);
    PA_long32 numDays = PA_GetLongParameter(params, 3);

    OCIDate result;
    sword status = OCIDateAddDays(errhp, &dateIn, (sb4)numDays, &result);
    if (status == OCI_SUCCESS) {
        ocidate_to_date_param(params, 4, &result);
    }
    PA_ReturnLong(params, oci_check(status));
}

// OCIDateAddMonths(errhp; date_in; num_months; date_out) : status
static void cmd_OCIDateAddMonths(PA_PluginParameters params) {
    PA_long32 errhpId = PA_GetLongParameter(params, 1);
    OCIError* errhp = handles().getAs<OCIError>(errhpId);
    if (!errhp) { PA_ReturnLong(params, (PA_long32)OCI_ERROR); return; }

    OCIDate dateIn;
    date_param_to_ocidate(params, 2, &dateIn);
    PA_long32 numMonths = PA_GetLongParameter(params, 3);

    OCIDate result;
    sword status = OCIDateAddMonths(errhp, &dateIn, (sb4)numMonths, &result);
    if (status == OCI_SUCCESS) {
        ocidate_to_date_param(params, 4, &result);
    }
    PA_ReturnLong(params, oci_check(status));
}

// OCIDateFromText(errhp; date_str; fmt; lang; date_out; time_out) : status
static void cmd_OCIDateFromText(PA_PluginParameters params) {
    PA_long32 errhpId = PA_GetLongParameter(params, 1);
    OCIError* errhp = handles().getAs<OCIError>(errhpId);
    if (!errhp) { PA_ReturnLong(params, (PA_long32)OCI_ERROR); return; }

    PA_Unistring* uStr  = PA_GetStringParameter(params, 2);
    PA_Unistring* uFmt  = PA_GetStringParameter(params, 3);
    PA_Unistring* uLang = PA_GetStringParameter(params, 4);
    std::string str  = unistr_to_utf8(uStr);
    std::string fmt  = unistr_to_utf8(uFmt);
    std::string lang = unistr_to_utf8(uLang);

    OCIDate result;
    sword status = OCIDateFromText(errhp,
        (const OraText*)str.c_str(), (ub4)str.size(),
        (const OraText*)(fmt.empty() ? nullptr : fmt.c_str()), (ub1)fmt.size(),
        (const OraText*)(lang.empty() ? nullptr : lang.c_str()), (ub4)lang.size(),
        &result);
    if (status == OCI_SUCCESS) {
        ocidate_to_params(params, 5, 6, &result);
    }
    PA_ReturnLong(params, oci_check(status));
}

// OCIDateToText(errhp; date_in; time_in; fmt; lang; text_out_ptr) : status
static void cmd_OCIDateToText(PA_PluginParameters params) {
    PA_long32 errhpId = PA_GetLongParameter(params, 1);
    OCIError* errhp = handles().getAs<OCIError>(errhpId);
    if (!errhp) { PA_ReturnLong(params, (PA_long32)OCI_ERROR); return; }

    OCIDate dateIn;
    params_to_ocidate(params, 2, 3, &dateIn);

    PA_Unistring* uFmt  = PA_GetStringParameter(params, 4);
    PA_Unistring* uLang = PA_GetStringParameter(params, 5);
    std::string fmt  = unistr_to_utf8(uFmt);
    std::string lang = unistr_to_utf8(uLang);

    OraText buf[256];
    ub4 bufLen = sizeof(buf);
    sword status = OCIDateToText(errhp, &dateIn,
        (const OraText*)(fmt.empty() ? nullptr : fmt.c_str()), (ub1)fmt.size(),
        (const OraText*)(lang.empty() ? nullptr : lang.c_str()), (ub4)lang.size(),
        &bufLen, buf);
    if (status == OCI_SUCCESS) {
        set_pointer_text(params, 6, (const char*)buf, (PA_long32)bufLen);
    }
    PA_ReturnLong(params, oci_check(status));
}

// OCIDateSysDate(errhp; date_out; time_out) : status
static void cmd_OCIDateSysDate(PA_PluginParameters params) {
    PA_long32 errhpId = PA_GetLongParameter(params, 1);
    OCIError* errhp = handles().getAs<OCIError>(errhpId);
    if (!errhp) { PA_ReturnLong(params, (PA_long32)OCI_ERROR); return; }

    OCIDate result;
    sword status = OCIDateSysDate(errhp, &result);
    if (status == OCI_SUCCESS) {
        ocidate_to_params(params, 2, 3, &result);
    }
    PA_ReturnLong(params, oci_check(status));
}

// OCIDateLastDay(errhp; date_in; date_out) : status
static void cmd_OCIDateLastDay(PA_PluginParameters params) {
    PA_long32 errhpId = PA_GetLongParameter(params, 1);
    OCIError* errhp = handles().getAs<OCIError>(errhpId);
    if (!errhp) { PA_ReturnLong(params, (PA_long32)OCI_ERROR); return; }

    OCIDate dateIn;
    date_param_to_ocidate(params, 2, &dateIn);

    OCIDate result;
    sword status = OCIDateLastDay(errhp, &dateIn, &result);
    if (status == OCI_SUCCESS) {
        ocidate_to_date_param(params, 3, &result);
    }
    PA_ReturnLong(params, oci_check(status));
}

// OCIDateNextDay(errhp; date_in; day_name; date_out) : status
static void cmd_OCIDateNextDay(PA_PluginParameters params) {
    PA_long32 errhpId = PA_GetLongParameter(params, 1);
    OCIError* errhp = handles().getAs<OCIError>(errhpId);
    if (!errhp) { PA_ReturnLong(params, (PA_long32)OCI_ERROR); return; }

    OCIDate dateIn;
    date_param_to_ocidate(params, 2, &dateIn);

    PA_Unistring* uDay = PA_GetStringParameter(params, 3);
    std::string dayName = unistr_to_utf8(uDay);

    OCIDate result;
    sword status = OCIDateNextDay(errhp, &dateIn,
        (const OraText*)dayName.c_str(), (ub4)dayName.size(), &result);
    if (status == OCI_SUCCESS) {
        ocidate_to_date_param(params, 4, &result);
    }
    PA_ReturnLong(params, oci_check(status));
}

// OCIDateZoneToZone(errhp; date_in; time_in; zone1; zone2; date_out; time_out) : status
static void cmd_OCIDateZoneToZone(PA_PluginParameters params) {
    PA_long32 errhpId = PA_GetLongParameter(params, 1);
    OCIError* errhp = handles().getAs<OCIError>(errhpId);
    if (!errhp) { PA_ReturnLong(params, (PA_long32)OCI_ERROR); return; }

    OCIDate dateIn;
    params_to_ocidate(params, 2, 3, &dateIn);

    PA_Unistring* uZone1 = PA_GetStringParameter(params, 4);
    PA_Unistring* uZone2 = PA_GetStringParameter(params, 5);
    std::string zone1 = unistr_to_utf8(uZone1);
    std::string zone2 = unistr_to_utf8(uZone2);

    OCIDate result;
    sword status = OCIDateZoneToZone(errhp, &dateIn,
        (const OraText*)zone1.c_str(), (ub4)zone1.size(),
        (const OraText*)zone2.c_str(), (ub4)zone2.size(),
        &result);
    if (status == OCI_SUCCESS) {
        ocidate_to_params(params, 6, 7, &result);
    }
    PA_ReturnLong(params, oci_check(status));
}

// ============================================================
// DATE BIND/DEFINE commands
// ============================================================

// OCIBindDateByPos(stmtp; errhp; bindp_out; position; date_ptr; time_ptr; dty; ind_ptr; rlen_ptr; rcode_ptr; maxarr_len; curelep) : status
static void cmd_OCIBindDateByPos(PA_PluginParameters params) {
    PA_long32 stmtpId  = PA_GetLongParameter(params, 1);
    PA_long32 errhpId  = PA_GetLongParameter(params, 2);
    PA_long32 position = PA_GetLongParameter(params, 4);
    PA_long32 dty      = PA_GetLongParameter(params, 7);

    OCIStmt*  stmtp = handles().getAs<OCIStmt>(stmtpId);
    OCIError* errhp = handles().getAs<OCIError>(errhpId);

    if (!stmtp || !errhp || position < 1) {
        PA_SetLongParameter(params, 3, 0);
        PA_ReturnLong(params, (PA_long32)OCI_ERROR);
        return;
    }

    OCIBindBuffer& buf = stmtBuffers().addBind(stmtpId, (ub4)position);
    buf.dty = (ub4)dty;
    buf.data.resize(sizeof(OCIDate), 0);
    buf.indicator = 0;
    buf.returnLen = (ub2)sizeof(OCIDate);
    buf.valuePtr = PA_GetPointerParameter(params, 5);
    buf.indPtr   = PA_GetPointerParameter(params, 8);
    buf.rlenPtr  = PA_GetPointerParameter(params, 9);

    // Read input date/time from pointers into OCIDate buffer
    if (buf.valuePtr) {
        PA_Variable val = PA_GetPointerValue(buf.valuePtr);
        // Expect date value; time from param 6 pointer
        // For simplicity, init OCIDate to zero
        OCIDate* od = reinterpret_cast<OCIDate*>(buf.data.data());
        OCIDateSetDate(od, 0, 0, 0);
        OCIDateSetTime(od, 0, 0, 0);
    }

    buf.bindp = nullptr;
    sword status = OCIBindByPos(stmtp, &buf.bindp, errhp,
                                 (ub4)position,
                                 buf.data.data(), (sb4)sizeof(OCIDate),
                                 SQLT_ODT,
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

// OCIBindDateByName(stmtp; errhp; bindp_out; placeholder; date_ptr; time_ptr; dty; ind_ptr; rlen_ptr; rcode_ptr; maxarr_len; curelep) : status
static void cmd_OCIBindDateByName(PA_PluginParameters params) {
    PA_long32 stmtpId = PA_GetLongParameter(params, 1);
    PA_long32 errhpId = PA_GetLongParameter(params, 2);
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

    ub4 syntheticPos = 1;
    for (char c : placeholder) syntheticPos = syntheticPos * 31 + (unsigned char)c;
    syntheticPos = (syntheticPos % 10000) + 20001;

    OCIBindBuffer& buf = stmtBuffers().addBind(stmtpId, syntheticPos);
    buf.dty = (ub4)dty;
    buf.data.resize(sizeof(OCIDate), 0);
    buf.indicator = 0;
    buf.returnLen = (ub2)sizeof(OCIDate);
    buf.valuePtr = PA_GetPointerParameter(params, 5);
    buf.indPtr   = PA_GetPointerParameter(params, 8);
    buf.rlenPtr  = PA_GetPointerParameter(params, 9);

    buf.bindp = nullptr;
    sword status = OCIBindByName(stmtp, &buf.bindp, errhp,
                                  (const OraText*)placeholder.c_str(),
                                  (sb4)placeholder.size(),
                                  buf.data.data(), (sb4)sizeof(OCIDate),
                                  SQLT_ODT,
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

// OCIDefineDateByPos(stmtp; errhp; defnp_out; position; date_ptr; time_ptr; dty; ind_ptr; rlen_ptr; rcode_ptr; mode) : status
static void cmd_OCIDefineDateByPos(PA_PluginParameters params) {
    PA_long32 stmtpId  = PA_GetLongParameter(params, 1);
    PA_long32 errhpId  = PA_GetLongParameter(params, 2);
    PA_long32 position = PA_GetLongParameter(params, 4);
    PA_long32 dty      = PA_GetLongParameter(params, 7);
    PA_long32 mode     = PA_GetLongParameter(params, 11);

    OCIStmt*  stmtp = handles().getAs<OCIStmt>(stmtpId);
    OCIError* errhp = handles().getAs<OCIError>(errhpId);

    if (!stmtp || !errhp || position < 1) {
        PA_SetLongParameter(params, 3, 0);
        PA_ReturnLong(params, (PA_long32)OCI_ERROR);
        return;
    }

    OCIColumnBuffer& col = stmtBuffers().addDefine(stmtpId, (ub4)position);
    col.dty = (ub4)dty;
    col.data.resize(sizeof(OCIDate), 0);
    col.indicator = 0;
    col.returnLen = 0;
    col.valuePtr = PA_GetPointerParameter(params, 5);
    col.indPtr   = PA_GetPointerParameter(params, 8);
    col.rlenPtr  = PA_GetPointerParameter(params, 9);

    col.defnp = nullptr;
    sword status = OCIDefineByPos(stmtp, &col.defnp, errhp,
                                   (ub4)position,
                                   col.data.data(), (sb4)sizeof(OCIDate),
                                   SQLT_ODT,
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

// ============================================================
// REF commands
// ============================================================

// OCIRefAssign(envhp; errhp; source_ref; target_ref) : status
static void cmd_OCIRefAssign(PA_PluginParameters params) {
    PA_long32 envId = PA_GetLongParameter(params, 1);
    PA_long32 errId = PA_GetLongParameter(params, 2);
    PA_long32 srcId = PA_GetLongParameter(params, 3);
    PA_long32 tgtId = PA_GetLongParameter(params, 4);

    OCIEnv*   env = handles().getAs<OCIEnv>(envId);
    OCIError* err = handles().getAs<OCIError>(errId);
    OCIRef*   src = (OCIRef*)handles().get(srcId);
    OCIRef*   tgt = (OCIRef*)handles().get(tgtId);

    if (!env || !err || !src) {
        PA_ReturnLong(params, (PA_long32)OCI_ERROR); return;
    }
    sword status = OCIRefAssign(env, err, (const OCIRef*)src, &tgt);
    PA_ReturnLong(params, oci_check(status));
}

// OCIRefClear(envhp; ref) — no return value
static void cmd_OCIRefClear(PA_PluginParameters params) {
    PA_long32 envId = PA_GetLongParameter(params, 1);
    PA_long32 refId = PA_GetLongParameter(params, 2);
    OCIEnv* env = handles().getAs<OCIEnv>(envId);
    OCIRef* ref = (OCIRef*)handles().get(refId);
    if (env && ref) OCIRefClear(env, ref);
}

// OCIRefFromHex(envhp; errhp; svchp; hex_text; ref_handle) : status
static void cmd_OCIRefFromHex(PA_PluginParameters params) {
    PA_long32 envId  = PA_GetLongParameter(params, 1);
    PA_long32 errId  = PA_GetLongParameter(params, 2);
    PA_long32 svcId  = PA_GetLongParameter(params, 3);
    PA_long32 refId  = PA_GetLongParameter(params, 5);

    OCIEnv*    env  = handles().getAs<OCIEnv>(envId);
    OCIError*  err  = handles().getAs<OCIError>(errId);
    OCISvcCtx* svc  = handles().getAs<OCISvcCtx>(svcId);
    OCIRef*    ref  = (OCIRef*)handles().get(refId);

    PA_Unistring* uHex = PA_GetStringParameter(params, 4);
    std::string hex = unistr_to_utf8(uHex);

    if (!env || !err || !svc) {
        PA_ReturnLong(params, (PA_long32)OCI_ERROR); return;
    }

    ub4 hexLen = (ub4)hex.size();
    sword status = OCIRefFromHex(env, err, svc,
        (const OraText*)hex.c_str(), hexLen, &ref);
    PA_ReturnLong(params, oci_check(status));
}

// OCIRefToHex(envhp; errhp; ref_handle; hex_text_ptr) : status
static void cmd_OCIRefToHex(PA_PluginParameters params) {
    PA_long32 envId = PA_GetLongParameter(params, 1);
    PA_long32 errId = PA_GetLongParameter(params, 2);
    PA_long32 refId = PA_GetLongParameter(params, 3);

    OCIEnv*   env = handles().getAs<OCIEnv>(envId);
    OCIError* err = handles().getAs<OCIError>(errId);
    OCIRef*   ref = (OCIRef*)handles().get(refId);

    if (!env || !err || !ref) {
        PA_ReturnLong(params, (PA_long32)OCI_ERROR); return;
    }

    OraText buf[1024];
    ub4 bufLen = sizeof(buf);
    sword status = OCIRefToHex(env, err, (const OCIRef*)ref, buf, &bufLen);
    if (status == OCI_SUCCESS) {
        set_pointer_text(params, 4, (const char*)buf, (PA_long32)bufLen);
    }
    PA_ReturnLong(params, oci_check(status));
}

// OCIRefHexSize(envhp; ref_handle) : size
static void cmd_OCIRefHexSize(PA_PluginParameters params) {
    PA_long32 envId = PA_GetLongParameter(params, 1);
    PA_long32 refId = PA_GetLongParameter(params, 2);

    OCIEnv* env = handles().getAs<OCIEnv>(envId);
    OCIRef* ref = (OCIRef*)handles().get(refId);

    if (!env || !ref) {
        PA_ReturnLong(params, 0); return;
    }
    ub4 size = OCIRefHexSize(env, (const OCIRef*)ref);
    PA_ReturnLong(params, (PA_long32)size);
}

// OCIRefIsEqual(envhp; ref1; ref2) : boolean (1/0)
static void cmd_OCIRefIsEqual(PA_PluginParameters params) {
    PA_long32 envId  = PA_GetLongParameter(params, 1);
    PA_long32 ref1Id = PA_GetLongParameter(params, 2);
    PA_long32 ref2Id = PA_GetLongParameter(params, 3);

    OCIEnv* env  = handles().getAs<OCIEnv>(envId);
    OCIRef* ref1 = (OCIRef*)handles().get(ref1Id);
    OCIRef* ref2 = (OCIRef*)handles().get(ref2Id);

    if (!env || !ref1 || !ref2) {
        PA_ReturnLong(params, 0); return;
    }
    boolean result = OCIRefIsEqual(env, (const OCIRef*)ref1, (const OCIRef*)ref2);
    PA_ReturnLong(params, (PA_long32)(result ? 1 : 0));
}

// OCIRefIsNull(envhp; ref) : boolean (1/0)
static void cmd_OCIRefIsNull(PA_PluginParameters params) {
    PA_long32 envId = PA_GetLongParameter(params, 1);
    PA_long32 refId = PA_GetLongParameter(params, 2);

    OCIEnv* env = handles().getAs<OCIEnv>(envId);
    OCIRef* ref = (OCIRef*)handles().get(refId);

    if (!env) {
        PA_ReturnLong(params, 1); return;
    }
    boolean result = OCIRefIsNull(env, (const OCIRef*)ref);
    PA_ReturnLong(params, (PA_long32)(result ? 1 : 0));
}

// ============================================================
// RAW commands
// ============================================================

// OCIRawAllocSize(envhp; errhp; raw_handle; alloc_size_out) : status
static void cmd_OCIRawAllocSize(PA_PluginParameters params) {
    PA_long32 envId = PA_GetLongParameter(params, 1);
    PA_long32 errId = PA_GetLongParameter(params, 2);
    PA_long32 rawId = PA_GetLongParameter(params, 3);

    OCIEnv*   env = handles().getAs<OCIEnv>(envId);
    OCIError* err = handles().getAs<OCIError>(errId);
    OCIRaw*   raw = (OCIRaw*)handles().get(rawId);

    if (!env || !err || !raw) {
        PA_ReturnLong(params, (PA_long32)OCI_ERROR); return;
    }
    ub4 allocSize = 0;
    sword status = OCIRawAllocSize(env, err, (const OCIRaw*)raw, &allocSize);
    PA_SetLongParameter(params, 4, (PA_long32)allocSize);
    PA_ReturnLong(params, oci_check(status));
}

// OCIRawAssignBytes(envhp; errhp; blob_data; raw_handle) : status
static void cmd_OCIRawAssignBytes(PA_PluginParameters params) {
    PA_long32 envId = PA_GetLongParameter(params, 1);
    PA_long32 errId = PA_GetLongParameter(params, 2);
    PA_long32 rawId = PA_GetLongParameter(params, 4);

    OCIEnv*   env = handles().getAs<OCIEnv>(envId);
    OCIError* err = handles().getAs<OCIError>(errId);
    OCIRaw*   raw = (OCIRaw*)handles().get(rawId);

    if (!env || !err) {
        PA_ReturnLong(params, (PA_long32)OCI_ERROR); return;
    }

    PA_Handle hBlob = PA_GetBlobHandleParameter(params, 3);
    PA_long32 blobLen = PA_GetHandleSize(hBlob);
    char* blobPtr = PA_LockHandle(hBlob);

    sword status = OCIRawAssignBytes(env, err, (const ub1*)blobPtr, (ub4)blobLen, &raw);
    PA_UnlockHandle(hBlob);
    PA_ReturnLong(params, oci_check(status));
}

// OCIRawAssignRaw(envhp; errhp; src_raw; dst_raw) : status
static void cmd_OCIRawAssignRaw(PA_PluginParameters params) {
    PA_long32 envId = PA_GetLongParameter(params, 1);
    PA_long32 errId = PA_GetLongParameter(params, 2);
    PA_long32 srcId = PA_GetLongParameter(params, 3);
    PA_long32 dstId = PA_GetLongParameter(params, 4);

    OCIEnv*   env = handles().getAs<OCIEnv>(envId);
    OCIError* err = handles().getAs<OCIError>(errId);
    OCIRaw*   src = (OCIRaw*)handles().get(srcId);
    OCIRaw*   dst = (OCIRaw*)handles().get(dstId);

    if (!env || !err || !src) {
        PA_ReturnLong(params, (PA_long32)OCI_ERROR); return;
    }
    sword status = OCIRawAssignRaw(env, err, (const OCIRaw*)src, &dst);
    PA_ReturnLong(params, oci_check(status));
}

// OCIRawPtr(envhp; raw_handle) : blob
static void cmd_OCIRawPtr(PA_PluginParameters params) {
    PA_long32 envId = PA_GetLongParameter(params, 1);
    PA_long32 rawId = PA_GetLongParameter(params, 2);

    OCIEnv* env = handles().getAs<OCIEnv>(envId);
    OCIRaw* raw = (OCIRaw*)handles().get(rawId);

    if (!env || !raw) {
        PA_ReturnBlob(params, nullptr, 0);
        return;
    }
    ub1* ptr = OCIRawPtr(env, raw);
    ub4 size = OCIRawSize(env, (const OCIRaw*)raw);
    PA_ReturnBlob(params, ptr, (PA_long32)size);
}

// OCIRawResize(envhp; errhp; new_size; raw_handle) : status
static void cmd_OCIRawResize(PA_PluginParameters params) {
    PA_long32 envId   = PA_GetLongParameter(params, 1);
    PA_long32 errId   = PA_GetLongParameter(params, 2);
    PA_long32 newSize = PA_GetLongParameter(params, 3);
    PA_long32 rawId   = PA_GetLongParameter(params, 4);

    OCIEnv*   env = handles().getAs<OCIEnv>(envId);
    OCIError* err = handles().getAs<OCIError>(errId);
    OCIRaw*   raw = (OCIRaw*)handles().get(rawId);

    if (!env || !err) {
        PA_ReturnLong(params, (PA_long32)OCI_ERROR); return;
    }
    sword status = OCIRawResize(env, err, (ub2)newSize, &raw);
    PA_ReturnLong(params, oci_check(status));
}

// OCIRawSize(envhp; raw_handle) : size
static void cmd_OCIRawSize(PA_PluginParameters params) {
    PA_long32 envId = PA_GetLongParameter(params, 1);
    PA_long32 rawId = PA_GetLongParameter(params, 2);

    OCIEnv* env = handles().getAs<OCIEnv>(envId);
    OCIRaw* raw = (OCIRaw*)handles().get(rawId);

    if (!env || !raw) {
        PA_ReturnLong(params, 0); return;
    }
    ub4 size = OCIRawSize(env, (const OCIRaw*)raw);
    PA_ReturnLong(params, (PA_long32)size);
}

// ============================================================
// COLLECTION stubs
// ============================================================

static void cmd_OCICollAppend(PA_PluginParameters params) {
    PA_ReturnLong(params, (PA_long32)OCI_ERROR);
}
static void cmd_OCICollAssign(PA_PluginParameters params) {
    PA_ReturnLong(params, (PA_long32)OCI_ERROR);
}
static void cmd_OCICollAssignElem(PA_PluginParameters params) {
    PA_ReturnLong(params, (PA_long32)OCI_ERROR);
}
static void cmd_OCICollGetElem(PA_PluginParameters params) {
    PA_ReturnLong(params, (PA_long32)OCI_ERROR);
}
static void cmd_OCICollMax(PA_PluginParameters params) {
    PA_ReturnLong(params, (PA_long32)OCI_ERROR);
}
static void cmd_OCICollSize(PA_PluginParameters params) {
    PA_ReturnLong(params, (PA_long32)OCI_ERROR);
}
static void cmd_OCICollTrim(PA_PluginParameters params) {
    PA_ReturnLong(params, (PA_long32)OCI_ERROR);
}

// ============================================================
// ITERATOR stubs
// ============================================================

static void cmd_OCIIterCreate(PA_PluginParameters params) {
    PA_ReturnLong(params, (PA_long32)OCI_ERROR);
}
static void cmd_OCIIterDelete(PA_PluginParameters params) {
    PA_ReturnLong(params, (PA_long32)OCI_ERROR);
}
static void cmd_OCIIterInit(PA_PluginParameters params) {
    PA_ReturnLong(params, (PA_long32)OCI_ERROR);
}
static void cmd_OCIIterGetCurrent(PA_PluginParameters params) {
    PA_ReturnLong(params, (PA_long32)OCI_ERROR);
}
static void cmd_OCIIterNext(PA_PluginParameters params) {
    PA_ReturnLong(params, (PA_long32)OCI_ERROR);
}
static void cmd_OCIIterPrev(PA_PluginParameters params) {
    PA_ReturnLong(params, (PA_long32)OCI_ERROR);
}

// ============================================================
// TABLE stubs
// ============================================================

static void cmd_OCITableDelete(PA_PluginParameters params) {
    PA_ReturnLong(params, (PA_long32)OCI_ERROR);
}
static void cmd_OCITableExists(PA_PluginParameters params) {
    PA_ReturnLong(params, (PA_long32)OCI_ERROR);
}
static void cmd_OCITableFirst(PA_PluginParameters params) {
    PA_ReturnLong(params, (PA_long32)OCI_ERROR);
}
static void cmd_OCITableLast(PA_PluginParameters params) {
    PA_ReturnLong(params, (PA_long32)OCI_ERROR);
}
static void cmd_OCITableNext(PA_PluginParameters params) {
    PA_ReturnLong(params, (PA_long32)OCI_ERROR);
}
static void cmd_OCITablePrev(PA_PluginParameters params) {
    PA_ReturnLong(params, (PA_long32)OCI_ERROR);
}
static void cmd_OCITableSize(PA_PluginParameters params) {
    PA_ReturnLong(params, (PA_long32)OCI_ERROR);
}
