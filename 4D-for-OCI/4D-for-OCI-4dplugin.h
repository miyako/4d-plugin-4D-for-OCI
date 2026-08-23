#ifndef PLUGIN_4D_FOR_OCI_H
#define PLUGIN_4D_FOR_OCI_H

#include "oci_bridge.h"

// ============================================================
// Selector constants (1-based, matching manifest.json order)
// ============================================================

// Connection
#define kSel_OCIEnvCreate        1
#define kSel_OCIHandleAlloc      2
#define kSel_OCIHandleFree       3
#define kSel_OCIServerAttach     4
#define kSel_OCIServerDetach     5
#define kSel_OCISessionBegin     6
#define kSel_OCISessionEnd       7
#define kSel_OCILogon            8
#define kSel_OCILogoff           9
#define kSel_OCIBreak           10
#define kSel_OCIReset           11

// Datatype
#define kSel_OCIAttrGetVal      12
#define kSel_OCIAttrGetText     13
#define kSel_OCIAttrSetVal      14
#define kSel_OCIAttrSetText     15
#define kSel_OCIDescriptorAlloc 16
#define kSel_OCIDescriptorFree  17
#define kSel_OCIErrorGet        18
#define kSel_OCIServerVersion   19
#define kSel_OCIStmtPrepare     20
#define kSel_OCIStmtExecute     21
#define kSel_OCIStmtFetch       22
#define kSel_OCIStmtGetBindInfo 23
#define kSel_OCIBindByPos       24
#define kSel_OCIBindByName      25
#define kSel_OCIDefineByPos     26
#define kSel_OCIDescribeAnyText 27
#define kSel_OCIDescribeAnyVal  28

// Transaction
#define kSel_OCITransStart      29
#define kSel_OCITransCommit     30
#define kSel_OCITransRollback   31
#define kSel_OCITransPrepare    32
#define kSel_OCITransDetach     33
#define kSel_OCITransForget     34

// Extras
#define kSel_OCITerminate       35
#define kSel_OCIOnErrCall       36
#define kSel_OCIGetTnsnamesPath 37
#define kSel_OCISetEnv          38
#define kSel_OCIGetEnv          39

// Connection extras
#define kSel_OCIParamGet        40
#define kSel_OCIParamSet        41
#define kSel_OCIPasswordChange  42

// Types/Cache
#define kSel_OCICacheFlush       43
#define kSel_OCICacheFree        44
#define kSel_OCICacheRefresh     45
#define kSel_OCICacheUnmark      46
#define kSel_OCICacheUnpin       47

// Math/OCINumber
#define kSel_OCINumberAdd        48
#define kSel_OCINumberSub        49
#define kSel_OCINumberMul        50
#define kSel_OCINumberDiv        51
#define kSel_OCINumberPower      52
#define kSel_OCINumberLog        53
#define kSel_OCINumberArcTan2    54
#define kSel_OCINumberSqrt       55
#define kSel_OCINumberLn         56
#define kSel_OCINumberExp        57
#define kSel_OCINumberSin        58
#define kSel_OCINumberCos        59
#define kSel_OCINumberTan        60
#define kSel_OCINumberArcSin     61
#define kSel_OCINumberArcCos     62
#define kSel_OCINumberArcTan     63
#define kSel_OCINumberHypSin     64
#define kSel_OCINumberHypCos     65
#define kSel_OCINumberHypTan     66
#define kSel_OCINumberTrunc      67
#define kSel_OCINumberRound      68
#define kSel_OCINumberIntPower   69
#define kSel_OCINumberFromText   70
#define kSel_OCINumberToText     71

#define kSel_OCILobAppend           72
#define kSel_OCILobAssign           73
#define kSel_OCILobCharSetForm      74
#define kSel_OCILobCharSetId        75
#define kSel_OCILobCopy             76
#define kSel_OCILobCreateTemporary  77
#define kSel_OCILobDisableBuffering 78
#define kSel_OCILobEnableBuffering  79
#define kSel_OCILobErase            80
#define kSel_OCILobFileClose        81
#define kSel_OCILobFileCloseAll     82
#define kSel_OCILobFileExists       83
#define kSel_OCILobFileGetName      84
#define kSel_OCILobFileIsOpen       85
#define kSel_OCILobFileOpen         86
#define kSel_OCILobFileSetName      87
#define kSel_OCILobFlushBuffer      88
#define kSel_OCILobFreeTemporary    89
#define kSel_OCILobGetChunkSize     90
#define kSel_OCILobGetLength        91
#define kSel_OCILobIsEqual          92
#define kSel_OCILobIsTemporary      93
#define kSel_OCILobLoadFromFile     94
#define kSel_OCILobLocatorIsInit    95
#define kSel_OCILobRead             96
#define kSel_OCILobTrim             97
#define kSel_OCILobWrite            98
#define kSel_OCILobWriteAppend      99
#define kSel_OCIDurationBegin       100
#define kSel_OCIDurationEnd         101

// Date
#define kSel_OCIDateAddDays         102
#define kSel_OCIDateAddMonths       103
#define kSel_OCIDateFromText        104
#define kSel_OCIDateToText          105
#define kSel_OCIDateSysDate         106
#define kSel_OCIDateLastDay         107
#define kSel_OCIDateNextDay         108
#define kSel_OCIDateZoneToZone      109

// Date Bind/Define
#define kSel_OCIBindDateByPos       110
#define kSel_OCIBindDateByName      111
#define kSel_OCIDefineDateByPos     112

// Ref
#define kSel_OCIRefAssign           113
#define kSel_OCIRefClear            114
#define kSel_OCIRefFromHex          115
#define kSel_OCIRefToHex            116
#define kSel_OCIRefHexSize          117
#define kSel_OCIRefIsEqual          118
#define kSel_OCIRefIsNull           119

// Raw
#define kSel_OCIRawAllocSize        120
#define kSel_OCIRawAssignBytes      121
#define kSel_OCIRawAssignRaw        122
#define kSel_OCIRawPtr              123
#define kSel_OCIRawResize           124
#define kSel_OCIRawSize             125

// Collection (stubs)
#define kSel_OCICollAppend          126
#define kSel_OCICollAssign          127
#define kSel_OCICollAssignElem      128
#define kSel_OCICollGetElem         129
#define kSel_OCICollMax             130
#define kSel_OCICollSize            131
#define kSel_OCICollTrim            132

// Iterator (stubs)
#define kSel_OCIIterCreate          133
#define kSel_OCIIterDelete          134
#define kSel_OCIIterInit            135
#define kSel_OCIIterGetCurrent      136
#define kSel_OCIIterNext            137
#define kSel_OCIIterPrev            138

// Table (stubs)
#define kSel_OCITableDelete         139
#define kSel_OCITableExists         140
#define kSel_OCITableFirst          141
#define kSel_OCITableLast           142
#define kSel_OCITableNext           143
#define kSel_OCITablePrev           144
#define kSel_OCITableSize           145

#define kSel_MaxCommand         145

// ============================================================
// Command function declarations
// ============================================================

// Connection
static void cmd_OCIEnvCreate(PA_PluginParameters params);
static void cmd_OCIHandleAlloc(PA_PluginParameters params);
static void cmd_OCIHandleFree(PA_PluginParameters params);
static void cmd_OCIServerAttach(PA_PluginParameters params);
static void cmd_OCIServerDetach(PA_PluginParameters params);
static void cmd_OCISessionBegin(PA_PluginParameters params);
static void cmd_OCISessionEnd(PA_PluginParameters params);
static void cmd_OCILogon(PA_PluginParameters params);
static void cmd_OCILogoff(PA_PluginParameters params);
static void cmd_OCIBreak(PA_PluginParameters params);
static void cmd_OCIReset(PA_PluginParameters params);

// Datatype
static void cmd_OCIAttrGetVal(PA_PluginParameters params);
static void cmd_OCIAttrGetText(PA_PluginParameters params);
static void cmd_OCIAttrSetVal(PA_PluginParameters params);
static void cmd_OCIAttrSetText(PA_PluginParameters params);
static void cmd_OCIDescriptorAlloc(PA_PluginParameters params);
static void cmd_OCIDescriptorFree(PA_PluginParameters params);
static void cmd_OCIErrorGet(PA_PluginParameters params);
static void cmd_OCIServerVersion(PA_PluginParameters params);
static void cmd_OCIStmtPrepare(PA_PluginParameters params);
static void cmd_OCIStmtExecute(PA_PluginParameters params);
static void cmd_OCIStmtFetch(PA_PluginParameters params);
static void cmd_OCIStmtGetBindInfo(PA_PluginParameters params);
static void cmd_OCIBindByPos(PA_PluginParameters params);
static void cmd_OCIBindByName(PA_PluginParameters params);
static void cmd_OCIDefineByPos(PA_PluginParameters params);
static void cmd_OCIDescribeAnyText(PA_PluginParameters params);
static void cmd_OCIDescribeAnyVal(PA_PluginParameters params);

// Transaction
static void cmd_OCITransStart(PA_PluginParameters params);
static void cmd_OCITransCommit(PA_PluginParameters params);
static void cmd_OCITransRollback(PA_PluginParameters params);
static void cmd_OCITransPrepare(PA_PluginParameters params);
static void cmd_OCITransDetach(PA_PluginParameters params);
static void cmd_OCITransForget(PA_PluginParameters params);

// Extras
static void cmd_OCITerminate(PA_PluginParameters params);
static void cmd_OCIOnErrCall(PA_PluginParameters params);
static void cmd_OCIGetTnsnamesPath(PA_PluginParameters params);
static void cmd_OCISetEnv(PA_PluginParameters params);
static void cmd_OCIGetEnv(PA_PluginParameters params);

// Connection extras
static void cmd_OCIParamGet(PA_PluginParameters params);
static void cmd_OCIParamSet(PA_PluginParameters params);
static void cmd_OCIPasswordChange(PA_PluginParameters params);

// Types/Cache
static void cmd_OCICacheFlush(PA_PluginParameters params);
static void cmd_OCICacheFree(PA_PluginParameters params);
static void cmd_OCICacheRefresh(PA_PluginParameters params);
static void cmd_OCICacheUnmark(PA_PluginParameters params);
static void cmd_OCICacheUnpin(PA_PluginParameters params);

// Math/OCINumber
static void cmd_OCINumberAdd(PA_PluginParameters params);
static void cmd_OCINumberSub(PA_PluginParameters params);
static void cmd_OCINumberMul(PA_PluginParameters params);
static void cmd_OCINumberDiv(PA_PluginParameters params);
static void cmd_OCINumberPower(PA_PluginParameters params);
static void cmd_OCINumberLog(PA_PluginParameters params);
static void cmd_OCINumberArcTan2(PA_PluginParameters params);
static void cmd_OCINumberSqrt(PA_PluginParameters params);
static void cmd_OCINumberLn(PA_PluginParameters params);
static void cmd_OCINumberExp(PA_PluginParameters params);
static void cmd_OCINumberSin(PA_PluginParameters params);
static void cmd_OCINumberCos(PA_PluginParameters params);
static void cmd_OCINumberTan(PA_PluginParameters params);
static void cmd_OCINumberArcSin(PA_PluginParameters params);
static void cmd_OCINumberArcCos(PA_PluginParameters params);
static void cmd_OCINumberArcTan(PA_PluginParameters params);
static void cmd_OCINumberHypSin(PA_PluginParameters params);
static void cmd_OCINumberHypCos(PA_PluginParameters params);
static void cmd_OCINumberHypTan(PA_PluginParameters params);
static void cmd_OCINumberTrunc(PA_PluginParameters params);
static void cmd_OCINumberRound(PA_PluginParameters params);
static void cmd_OCINumberIntPower(PA_PluginParameters params);
static void cmd_OCINumberFromText(PA_PluginParameters params);
static void cmd_OCINumberToText(PA_PluginParameters params);

// LOB commands
static void cmd_OCILobAppend(PA_PluginParameters params);
static void cmd_OCILobAssign(PA_PluginParameters params);
static void cmd_OCILobCharSetForm(PA_PluginParameters params);
static void cmd_OCILobCharSetId(PA_PluginParameters params);
static void cmd_OCILobCopy(PA_PluginParameters params);
static void cmd_OCILobCreateTemporary(PA_PluginParameters params);
static void cmd_OCILobDisableBuffering(PA_PluginParameters params);
static void cmd_OCILobEnableBuffering(PA_PluginParameters params);
static void cmd_OCILobErase(PA_PluginParameters params);
static void cmd_OCILobFileClose(PA_PluginParameters params);
static void cmd_OCILobFileCloseAll(PA_PluginParameters params);
static void cmd_OCILobFileExists(PA_PluginParameters params);
static void cmd_OCILobFileGetName(PA_PluginParameters params);
static void cmd_OCILobFileIsOpen(PA_PluginParameters params);
static void cmd_OCILobFileOpen(PA_PluginParameters params);
static void cmd_OCILobFileSetName(PA_PluginParameters params);
static void cmd_OCILobFlushBuffer(PA_PluginParameters params);
static void cmd_OCILobFreeTemporary(PA_PluginParameters params);
static void cmd_OCILobGetChunkSize(PA_PluginParameters params);
static void cmd_OCILobGetLength(PA_PluginParameters params);
static void cmd_OCILobIsEqual(PA_PluginParameters params);
static void cmd_OCILobIsTemporary(PA_PluginParameters params);
static void cmd_OCILobLoadFromFile(PA_PluginParameters params);
static void cmd_OCILobLocatorIsInit(PA_PluginParameters params);
static void cmd_OCILobRead(PA_PluginParameters params);
static void cmd_OCILobTrim(PA_PluginParameters params);
static void cmd_OCILobWrite(PA_PluginParameters params);
static void cmd_OCILobWriteAppend(PA_PluginParameters params);
static void cmd_OCIDurationBegin(PA_PluginParameters params);
static void cmd_OCIDurationEnd(PA_PluginParameters params);

// Date
static void cmd_OCIDateAddDays(PA_PluginParameters params);
static void cmd_OCIDateAddMonths(PA_PluginParameters params);
static void cmd_OCIDateFromText(PA_PluginParameters params);
static void cmd_OCIDateToText(PA_PluginParameters params);
static void cmd_OCIDateSysDate(PA_PluginParameters params);
static void cmd_OCIDateLastDay(PA_PluginParameters params);
static void cmd_OCIDateNextDay(PA_PluginParameters params);
static void cmd_OCIDateZoneToZone(PA_PluginParameters params);

// Date Bind/Define
static void cmd_OCIBindDateByPos(PA_PluginParameters params);
static void cmd_OCIBindDateByName(PA_PluginParameters params);
static void cmd_OCIDefineDateByPos(PA_PluginParameters params);

// Ref
static void cmd_OCIRefAssign(PA_PluginParameters params);
static void cmd_OCIRefClear(PA_PluginParameters params);
static void cmd_OCIRefFromHex(PA_PluginParameters params);
static void cmd_OCIRefToHex(PA_PluginParameters params);
static void cmd_OCIRefHexSize(PA_PluginParameters params);
static void cmd_OCIRefIsEqual(PA_PluginParameters params);
static void cmd_OCIRefIsNull(PA_PluginParameters params);

// Raw
static void cmd_OCIRawAllocSize(PA_PluginParameters params);
static void cmd_OCIRawAssignBytes(PA_PluginParameters params);
static void cmd_OCIRawAssignRaw(PA_PluginParameters params);
static void cmd_OCIRawPtr(PA_PluginParameters params);
static void cmd_OCIRawResize(PA_PluginParameters params);
static void cmd_OCIRawSize(PA_PluginParameters params);

// Collection (stubs)
static void cmd_OCICollAppend(PA_PluginParameters params);
static void cmd_OCICollAssign(PA_PluginParameters params);
static void cmd_OCICollAssignElem(PA_PluginParameters params);
static void cmd_OCICollGetElem(PA_PluginParameters params);
static void cmd_OCICollMax(PA_PluginParameters params);
static void cmd_OCICollSize(PA_PluginParameters params);
static void cmd_OCICollTrim(PA_PluginParameters params);

// Iterator (stubs)
static void cmd_OCIIterCreate(PA_PluginParameters params);
static void cmd_OCIIterDelete(PA_PluginParameters params);
static void cmd_OCIIterInit(PA_PluginParameters params);
static void cmd_OCIIterGetCurrent(PA_PluginParameters params);
static void cmd_OCIIterNext(PA_PluginParameters params);
static void cmd_OCIIterPrev(PA_PluginParameters params);

// Table (stubs)
static void cmd_OCITableDelete(PA_PluginParameters params);
static void cmd_OCITableExists(PA_PluginParameters params);
static void cmd_OCITableFirst(PA_PluginParameters params);
static void cmd_OCITableLast(PA_PluginParameters params);
static void cmd_OCITableNext(PA_PluginParameters params);
static void cmd_OCITablePrev(PA_PluginParameters params);
static void cmd_OCITableSize(PA_PluginParameters params);

#endif // PLUGIN_4D_FOR_OCI_H
