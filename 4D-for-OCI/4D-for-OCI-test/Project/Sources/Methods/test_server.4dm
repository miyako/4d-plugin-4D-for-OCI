//%attributes = {"invisible":true,"preemptive":"capable"}
// Test OCIServerVersion and OCIPasswordChange (null-handle guards)
// Full tests require a live Oracle connection.

var $status : Integer
var $envhp : Integer
var $errhp : Integer

// Setup
$status:=OCIEnvCreate($envhp; OCI_DEFAULT)
ASSERT($status=OCI_SUCCESS; "OCIEnvCreate failed")

$status:=OCIHandleAlloc($envhp; $errhp; OCI_HTYPE_ERROR)
ASSERT($status=OCI_SUCCESS; "OCIHandleAlloc error failed")

// ---- OCIServerVersion — null handle guard ----
var $version : Text
$status:=OCIServerVersion(0; $errhp; ->$version)
ASSERT($status=OCI_ERROR; "OCIServerVersion with null handle should fail")

// ---- OCIPasswordChange — null handle guard ----
$status:=OCIPasswordChange(0; $errhp; "user"; "old_pw"; "new_pw"; OCI_DEFAULT)
ASSERT($status=OCI_ERROR; "OCIPasswordChange with null handle should fail")

// ---- Cleanup ----
$status:=OCIHandleFree($errhp)
$status:=OCITerminate
ASSERT($status=OCI_SUCCESS; "OCITerminate failed")
