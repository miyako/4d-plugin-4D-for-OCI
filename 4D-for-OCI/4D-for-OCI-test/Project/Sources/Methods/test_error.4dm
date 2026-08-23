//%attributes = {"invisible":true,"preemptive":"capable"}
// Test OCIErrorGet (no Oracle DB required)
// Creates an environment and error handle, then tests error retrieval

var $status : Integer
var $envhp : Integer
var $errhp : Integer

// Setup: create env + error handle
$status:=OCIEnvCreate($envhp; OCI_DEFAULT)
ASSERT($status=OCI_SUCCESS; "OCIEnvCreate failed")

$status:=OCIHandleAlloc($envhp; $errhp; OCI_HTYPE_ERROR)
ASSERT($status=OCI_SUCCESS; "OCIHandleAlloc error failed")

// ---- OCIErrorGet — with a valid but clean error handle ----
// No error has occurred yet, so OCIErrorGet should return OCI_NO_DATA (100)
var $errcode : Integer
var $errmsg : Text
$status:=OCIErrorGet($errhp; 1; $errcode; ->$errmsg)
// OCI_NO_DATA = 100 when no error has been recorded
ASSERT(($status=OCI_NO_DATA) | ($status=OCI_SUCCESS); \
  "OCIErrorGet on clean handle: "+String($status))

// ---- OCIErrorGet — with invalid handle ----
$status:=OCIErrorGet(0; 1; $errcode; ->$errmsg)
ASSERT($status=OCI_ERROR; "OCIErrorGet with null handle should fail")

// ---- Cleanup ----
$status:=OCIHandleFree($errhp)
$status:=OCITerminate
ASSERT($status=OCI_SUCCESS; "OCITerminate failed")
