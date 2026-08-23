//%attributes = {"invisible":true,"preemptive":"capable"}
// Test OCIAttrGet/Set commands
// Tests: OCIAttrGetVal, OCIAttrGetText, OCIAttrSetVal, OCIAttrSetText
// Some tests require a connected session to fully work.
// Here we test null-handle guards and basic dispatch.

var $status : Integer
var $envhp : Integer
var $errhp : Integer
var $svchp : Integer
var $authp : Integer

// Setup
$status:=OCIEnvCreate($envhp; OCI_DEFAULT)
ASSERT($status=OCI_SUCCESS; "OCIEnvCreate failed")

$status:=OCIHandleAlloc($envhp; $errhp; OCI_HTYPE_ERROR)
ASSERT($status=OCI_SUCCESS; "OCIHandleAlloc error failed")

$status:=OCIHandleAlloc($envhp; $svchp; OCI_HTYPE_SVCCTX)
ASSERT($status=OCI_SUCCESS; "OCIHandleAlloc svcctx failed")

$status:=OCIHandleAlloc($envhp; $authp; OCI_HTYPE_SESSION)
ASSERT($status=OCI_SUCCESS; "OCIHandleAlloc session failed")

// ---- OCIAttrSetVal — set server attr on service context ----
// Set the server handle on the service context
// This should work even without a connection
var $srvhp : Integer
$status:=OCIHandleAlloc($envhp; $srvhp; OCI_HTYPE_SERVER)
ASSERT($status=OCI_SUCCESS; "OCIHandleAlloc server failed")

$status:=OCIAttrSetVal($svchp; $srvhp; OCI_ATTR_SERVER; $errhp)
ASSERT($status=OCI_SUCCESS; "OCIAttrSetVal OCI_ATTR_SERVER failed: "+String($status))

// ---- OCIAttrSetText — set username on session handle ----
$status:=OCIAttrSetText($authp; "test_user"; OCI_ATTR_USERNAME; $errhp)
ASSERT($status=OCI_SUCCESS; "OCIAttrSetText USERNAME failed: "+String($status))

// ---- OCIAttrSetText — set password on session handle ----
$status:=OCIAttrSetText($authp; "test_pass"; OCI_ATTR_PASSWORD; $errhp)
ASSERT($status=OCI_SUCCESS; "OCIAttrSetText PASSWORD failed: "+String($status))

// ---- OCIAttrGetText — read username back ----
var $username : Text
$status:=OCIAttrGetText($authp; ->$username; OCI_ATTR_USERNAME; $errhp)
ASSERT($status=OCI_SUCCESS; "OCIAttrGetText USERNAME failed: "+String($status))
ASSERT($username="test_user"; "Username mismatch: '"+$username+"'")

// ---- OCIAttrGetVal — null handle guard ----
var $val : Integer
$val:=OCIAttrGetVal(0; OCI_HTYPE_SESSION; OCI_ATTR_USERNAME; $errhp)
ASSERT($val=0; "OCIAttrGetVal with null handle should return 0")

// ---- OCIAttrGetText — null handle guard ----
$status:=OCIAttrGetText(0; ->$username; OCI_ATTR_USERNAME; $errhp)
ASSERT($status=OCI_ERROR; "OCIAttrGetText with null handle should fail")

// ---- OCIAttrSetVal — null handle guard ----
$status:=OCIAttrSetVal(0; 0; OCI_ATTR_SERVER; $errhp)
ASSERT($status=OCI_ERROR; "OCIAttrSetVal with null handle should fail")

// ---- OCIAttrSetText — null handle guard ----
$status:=OCIAttrSetText(0; "x"; OCI_ATTR_USERNAME; $errhp)
ASSERT($status=OCI_ERROR; "OCIAttrSetText with null handle should fail")

// ---- Cleanup ----
$status:=OCIHandleFree($authp)
$status:=OCIHandleFree($svchp)
$status:=OCIHandleFree($srvhp)
$status:=OCIHandleFree($errhp)
$status:=OCITerminate
ASSERT($status=OCI_SUCCESS; "OCITerminate failed")
