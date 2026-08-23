//%attributes = {"invisible":true,"preemptive":"capable"}
// Test LOB commands (no Oracle DB required for basic calls)
// LOB locators require OCIDescriptorAlloc with OCI_DTYPE_LOB.
// Without a live connection, most LOB operations will return OCI_ERROR,
// but we can test handle creation and null-guard paths.

var $status : Integer
var $envhp : Integer
var $errhp : Integer
var $lobhp : Integer
var $flag : Integer

// ---- Setup ----
$status:=OCIEnvCreate($envhp; OCI_DEFAULT)
ASSERT($status=OCI_SUCCESS; "OCIEnvCreate failed")

$status:=OCIHandleAlloc($envhp; $errhp; OCI_HTYPE_ERROR)
ASSERT($status=OCI_SUCCESS; "OCIHandleAlloc error failed")

// ---- Allocate LOB descriptor ----
$status:=OCIDescriptorAlloc($envhp; $lobhp; OCI_DTYPE_LOB)
ASSERT($status=OCI_SUCCESS; "OCIDescriptorAlloc LOB failed: "+String($status))
ASSERT($lobhp#0; "OCIDescriptorAlloc returned null LOB handle")

// ---- OCILobLocatorIsInit — freshly allocated should be uninitialized ----
$status:=OCILobLocatorIsInit($envhp; $errhp; $lobhp; $flag)
ASSERT($status=OCI_SUCCESS; "OCILobLocatorIsInit failed: "+String($status))
// flag should be 0 (not initialized) for a fresh descriptor
ASSERT($flag=0; "Fresh LOB locator should not be initialized")

// ---- OCILobIsTemporary — fresh descriptor is not temporary ----
$status:=OCILobIsTemporary($envhp; $errhp; $lobhp; $flag)
ASSERT($status=OCI_SUCCESS; "OCILobIsTemporary failed: "+String($status))
ASSERT($flag=0; "Fresh LOB locator should not be temporary")

// ---- OCILobIsEqual — compare with itself ----
$status:=OCILobIsEqual($envhp; $lobhp; $lobhp; $flag)
ASSERT($status=OCI_SUCCESS; "OCILobIsEqual failed: "+String($status))
// Two identical locator handles should be equal
ASSERT($flag=1; "Same LOB locator should be equal to itself")

// ---- OCILobCharSetForm — should work on fresh descriptor ----
$status:=OCILobCharSetForm($envhp; $errhp; $lobhp; $flag)
ASSERT($status=OCI_SUCCESS; "OCILobCharSetForm failed: "+String($status))

// ---- OCILobCharSetId — should work on fresh descriptor ----
$status:=OCILobCharSetId($envhp; $errhp; $lobhp; $flag)
ASSERT($status=OCI_SUCCESS; "OCILobCharSetId failed: "+String($status))

// ---- Free LOB descriptor ----
$status:=OCIDescriptorFree($lobhp)
ASSERT($status=OCI_SUCCESS; "OCIDescriptorFree LOB failed")

// ---- Cleanup ----
$status:=OCIHandleFree($errhp)
ASSERT($status=OCI_SUCCESS; "OCIHandleFree error failed")

$status:=OCITerminate
ASSERT($status=OCI_SUCCESS; "OCITerminate failed")
