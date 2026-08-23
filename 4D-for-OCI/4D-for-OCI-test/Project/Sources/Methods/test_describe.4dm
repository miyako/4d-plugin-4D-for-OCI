//%attributes = {"invisible":true,"preemptive":"capable"}
// Test Describe & Param commands (null-handle guards, no Oracle DB required)
// Tests: OCIDescribeAnyText, OCIDescribeAnyVal, OCIParamGet
// Full describe requires a live connection; here we test null-handle guards.

var $status : Integer
var $envhp : Integer
var $errhp : Integer
var $dschp : Integer

// Setup
$status:=OCIEnvCreate($envhp; OCI_DEFAULT)
ASSERT($status=OCI_SUCCESS; "OCIEnvCreate failed")

$status:=OCIHandleAlloc($envhp; $errhp; OCI_HTYPE_ERROR)
ASSERT($status=OCI_SUCCESS; "OCIHandleAlloc error failed")

$status:=OCIHandleAlloc($envhp; $dschp; OCI_HTYPE_DESCRIBE)
ASSERT($status=OCI_SUCCESS; "OCIHandleAlloc describe failed")
ASSERT($dschp#0; "Describe handle is null")

// ---- OCIDescribeAnyText — null handle guards ----
$status:=OCIDescribeAnyText(0; $errhp; "MY_TABLE"; 0; OCI_PTYPE_TABLE; OCI_DEFAULT; $dschp)
ASSERT($status=OCI_ERROR; "OCIDescribeAnyText with null svchp should fail")

// ---- OCIDescribeAnyVal — null handle guards ----
$status:=OCIDescribeAnyVal(0; $errhp; 0; OCI_OTYPE_NAME; OCI_DEFAULT; OCI_PTYPE_TABLE; $dschp)
ASSERT($status=OCI_ERROR; "OCIDescribeAnyVal with null svchp should fail")

// ---- OCIParamGet — null handle guard ----
var $parmhp : Integer
$parmhp:=OCIParamGet(0; OCI_HTYPE_DESCRIBE; $errhp; 1)
ASSERT($parmhp=0; "OCIParamGet with null handle should return 0")

// ---- OCIParamGet — valid describe handle but no describe done yet ----
// This may crash or return OCI_ERROR depending on OCI version
// Skipped: requires a completed describe call

// ---- Cleanup ----
$status:=OCIHandleFree($dschp)
$status:=OCIHandleFree($errhp)
$status:=OCITerminate
ASSERT($status=OCI_SUCCESS; "OCITerminate failed")
