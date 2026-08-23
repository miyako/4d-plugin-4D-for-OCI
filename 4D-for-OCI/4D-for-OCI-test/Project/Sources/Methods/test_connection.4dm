//%attributes = {"invisible":true,"preemptive":"capable"}
// Test Connection commands (no Oracle DB required)
// Tests: OCIEnvCreate, OCIHandleAlloc, OCIHandleFree,
//        OCIServerAttach, OCIServerDetach, OCISessionBegin, OCISessionEnd,
//        OCILogon, OCILogoff, OCIBreak, OCIReset, OCITerminate

var $status : Integer
var $envhp : Integer
var $errhp : Integer
var $srvhp : Integer
var $svchp : Integer
var $authp : Integer

// ---- OCIEnvCreate ----
$status:=OCIEnvCreate($envhp; OCI_DEFAULT)
ASSERT($status=OCI_SUCCESS; "OCIEnvCreate failed: "+String($status))
ASSERT($envhp#0; "OCIEnvCreate returned null handle")

// ---- OCIHandleAlloc — error handle ----
$status:=OCIHandleAlloc($envhp; $errhp; OCI_HTYPE_ERROR)
ASSERT($status=OCI_SUCCESS; "OCIHandleAlloc error failed: "+String($status))
ASSERT($errhp#0; "Error handle is null")

// ---- OCIHandleAlloc — server handle ----
$status:=OCIHandleAlloc($envhp; $srvhp; OCI_HTYPE_SERVER)
ASSERT($status=OCI_SUCCESS; "OCIHandleAlloc server failed: "+String($status))
ASSERT($srvhp#0; "Server handle is null")

// ---- OCIHandleAlloc — service context ----
$status:=OCIHandleAlloc($envhp; $svchp; OCI_HTYPE_SVCCTX)
ASSERT($status=OCI_SUCCESS; "OCIHandleAlloc svcctx failed: "+String($status))
ASSERT($svchp#0; "Service context handle is null")

// ---- OCIHandleAlloc — session handle ----
$status:=OCIHandleAlloc($envhp; $authp; OCI_HTYPE_SESSION)
ASSERT($status=OCI_SUCCESS; "OCIHandleAlloc session failed: "+String($status))
ASSERT($authp#0; "Session handle is null")

// ---- Null-handle guard tests ----
// OCIServerAttach with invalid handle
$status:=OCIServerAttach(0; $errhp; "dummy")
ASSERT($status=OCI_ERROR; "OCIServerAttach should fail with null handle")

// OCIServerDetach with invalid handle
$status:=OCIServerDetach(0; $errhp)
ASSERT($status=OCI_ERROR; "OCIServerDetach should fail with null handle")

// OCISessionBegin with invalid handle
$status:=OCISessionBegin(0; $errhp; $authp; OCI_CRED_RDBMS; OCI_DEFAULT)
ASSERT($status=OCI_ERROR; "OCISessionBegin should fail with null handle")

// OCISessionEnd with invalid handle
$status:=OCISessionEnd(0; $errhp; $authp)
ASSERT($status=OCI_ERROR; "OCISessionEnd should fail with null handle")

// OCILogon with invalid env
var $svchp2 : Integer
$status:=OCILogon(0; $errhp; $svchp2; "user"; "pass"; "db")
ASSERT($status=OCI_ERROR; "OCILogon should fail with null env")

// OCILogoff with invalid handle
$status:=OCILogoff(0; $errhp)
ASSERT($status=OCI_ERROR; "OCILogoff should fail with null handle")

// OCIBreak with invalid handle
$status:=OCIBreak(0; $errhp)
ASSERT($status=OCI_ERROR; "OCIBreak should fail with null handle")

// OCIReset with invalid handle
$status:=OCIReset(0; $errhp)
ASSERT($status=OCI_ERROR; "OCIReset should fail with null handle")

// ---- OCIHandleFree ----
$status:=OCIHandleFree($authp)
ASSERT($status=OCI_SUCCESS; "OCIHandleFree session failed")

$status:=OCIHandleFree($svchp)
ASSERT($status=OCI_SUCCESS; "OCIHandleFree svcctx failed")

$status:=OCIHandleFree($srvhp)
ASSERT($status=OCI_SUCCESS; "OCIHandleFree server failed")

$status:=OCIHandleFree($errhp)
ASSERT($status=OCI_SUCCESS; "OCIHandleFree error failed")

// Free already-freed handle should fail
$status:=OCIHandleFree($errhp)
ASSERT($status=OCI_ERROR; "Double-free should fail")

// ---- OCITerminate ----
$status:=OCITerminate
ASSERT($status=OCI_SUCCESS; "OCITerminate failed")

