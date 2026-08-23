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

#define kSel_MaxCommand         42

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

#endif // PLUGIN_4D_FOR_OCI_H
