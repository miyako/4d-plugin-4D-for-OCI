//%attributes = {"invisible":true,"preemptive":"capable"}
// Test Transaction commands (null-handle guards, no Oracle DB required)
// Tests: OCITransStart, OCITransCommit, OCITransRollback,
//        OCITransPrepare, OCITransDetach, OCITransForget

var $status : Integer
var $envhp : Integer
var $errhp : Integer

// Setup
$status:=OCIEnvCreate($envhp; OCI_DEFAULT)
ASSERT($status=OCI_SUCCESS; "OCIEnvCreate failed")

$status:=OCIHandleAlloc($envhp; $errhp; OCI_HTYPE_ERROR)
ASSERT($status=OCI_SUCCESS; "OCIHandleAlloc error failed")

// ---- All transaction commands should fail with null service context ----

// OCITransStart
$status:=OCITransStart(0; $errhp; 60; OCI_TRANS_NEW)
ASSERT($status=OCI_ERROR; "OCITransStart with null svchp should fail")

// OCITransCommit
$status:=OCITransCommit(0; $errhp; OCI_DEFAULT)
ASSERT($status=OCI_ERROR; "OCITransCommit with null svchp should fail")

// OCITransRollback
$status:=OCITransRollback(0; $errhp; OCI_DEFAULT)
ASSERT($status=OCI_ERROR; "OCITransRollback with null svchp should fail")

// OCITransPrepare
$status:=OCITransPrepare(0; $errhp; OCI_DEFAULT)
ASSERT($status=OCI_ERROR; "OCITransPrepare with null svchp should fail")

// OCITransDetach
$status:=OCITransDetach(0; $errhp; OCI_DEFAULT)
ASSERT($status=OCI_ERROR; "OCITransDetach with null svchp should fail")

// OCITransForget
$status:=OCITransForget(0; $errhp; OCI_DEFAULT)
ASSERT($status=OCI_ERROR; "OCITransForget with null svchp should fail")

// ---- Also test with null error handle ----
var $svchp : Integer
$status:=OCIHandleAlloc($envhp; $svchp; OCI_HTYPE_SVCCTX)
ASSERT($status=OCI_SUCCESS; "OCIHandleAlloc svcctx failed")

$status:=OCITransStart($svchp; 0; 60; OCI_TRANS_NEW)
ASSERT($status=OCI_ERROR; "OCITransStart with null errhp should fail")

$status:=OCITransCommit($svchp; 0; OCI_DEFAULT)
ASSERT($status=OCI_ERROR; "OCITransCommit with null errhp should fail")

$status:=OCITransRollback($svchp; 0; OCI_DEFAULT)
ASSERT($status=OCI_ERROR; "OCITransRollback with null errhp should fail")

// ---- Cleanup ----
$status:=OCIHandleFree($svchp)
$status:=OCIHandleFree($errhp)
$status:=OCITerminate
ASSERT($status=OCI_SUCCESS; "OCITerminate failed")
