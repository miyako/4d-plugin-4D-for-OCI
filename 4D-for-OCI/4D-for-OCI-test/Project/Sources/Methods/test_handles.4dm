//%attributes = {"invisible":true,"preemptive":"capable"}
// Test Handle & Descriptor commands (no Oracle DB required)
// Tests: OCIEnvCreate, OCIHandleAlloc (all types), OCIHandleFree,
//        OCIDescriptorAlloc, OCIDescriptorFree

var $status : Integer
var $envhp : Integer
var $handle : Integer

// ---- OCIEnvCreate ----
$status:=OCIEnvCreate($envhp; OCI_DEFAULT)
ASSERT($status=OCI_SUCCESS; "OCIEnvCreate failed: "+String($status))
ASSERT($envhp#0; "OCIEnvCreate returned null")

// ---- OCIHandleAlloc — allocate each handle type ----
var $errhp : Integer
$status:=OCIHandleAlloc($envhp; $errhp; OCI_HTYPE_ERROR)
ASSERT($status=OCI_SUCCESS; "Alloc OCI_HTYPE_ERROR failed")
ASSERT($errhp#0; "Error handle is null")

var $srvhp : Integer
$status:=OCIHandleAlloc($envhp; $srvhp; OCI_HTYPE_SERVER)
ASSERT($status=OCI_SUCCESS; "Alloc OCI_HTYPE_SERVER failed")
ASSERT($srvhp#0; "Server handle is null")

var $svchp : Integer
$status:=OCIHandleAlloc($envhp; $svchp; OCI_HTYPE_SVCCTX)
ASSERT($status=OCI_SUCCESS; "Alloc OCI_HTYPE_SVCCTX failed")
ASSERT($svchp#0; "SvcCtx handle is null")

var $authp : Integer
$status:=OCIHandleAlloc($envhp; $authp; OCI_HTYPE_SESSION)
ASSERT($status=OCI_SUCCESS; "Alloc OCI_HTYPE_SESSION failed")
ASSERT($authp#0; "Session handle is null")

var $stmtp : Integer
$status:=OCIHandleAlloc($envhp; $stmtp; OCI_HTYPE_STMT)
ASSERT($status=OCI_SUCCESS; "Alloc OCI_HTYPE_STMT failed")
ASSERT($stmtp#0; "Stmt handle is null")

var $dschp : Integer
$status:=OCIHandleAlloc($envhp; $dschp; OCI_HTYPE_DESCRIBE)
ASSERT($status=OCI_SUCCESS; "Alloc OCI_HTYPE_DESCRIBE failed")
ASSERT($dschp#0; "Describe handle is null")

// All handle IDs should be unique
ASSERT($errhp#$srvhp; "Error and Server handles should differ")
ASSERT($srvhp#$svchp; "Server and SvcCtx handles should differ")
ASSERT($svchp#$authp; "SvcCtx and Session handles should differ")
ASSERT($authp#$stmtp; "Session and Stmt handles should differ")
ASSERT($stmtp#$dschp; "Stmt and Describe handles should differ")

// ---- OCIHandleAlloc — invalid parent ----
$status:=OCIHandleAlloc(0; $handle; OCI_HTYPE_ERROR)
ASSERT($status=OCI_ERROR; "Alloc with null parent should fail")

// ---- OCIDescriptorAlloc ----
var $descp : Integer
$status:=OCIDescriptorAlloc($envhp; $descp; OCI_DTYPE_TIMESTAMP)
ASSERT($status=OCI_SUCCESS; "OCIDescriptorAlloc TIMESTAMP failed: "+String($status))
ASSERT($descp#0; "Descriptor is null")

// Allocate a second descriptor
var $descp2 : Integer
$status:=OCIDescriptorAlloc($envhp; $descp2; OCI_DTYPE_LOB)
ASSERT($status=OCI_SUCCESS; "OCIDescriptorAlloc LOB failed: "+String($status))
ASSERT($descp2#0; "LOB descriptor is null")
ASSERT($descp#$descp2; "Descriptors should have different IDs")

// Invalid parent for descriptor
$status:=OCIDescriptorAlloc(0; $handle; OCI_DTYPE_TIMESTAMP)
ASSERT($status=OCI_ERROR; "DescriptorAlloc with null parent should fail")

// ---- OCIDescriptorFree ----
$status:=OCIDescriptorFree($descp)
ASSERT($status=OCI_SUCCESS; "OCIDescriptorFree failed")

$status:=OCIDescriptorFree($descp2)
ASSERT($status=OCI_SUCCESS; "OCIDescriptorFree LOB failed")

// Double-free should fail
$status:=OCIDescriptorFree($descp)
ASSERT($status=OCI_ERROR; "Double OCIDescriptorFree should fail")

// ---- OCIHandleFree ----
$status:=OCIHandleFree($dschp)
ASSERT($status=OCI_SUCCESS; "OCIHandleFree describe failed")

$status:=OCIHandleFree($stmtp)
ASSERT($status=OCI_SUCCESS; "OCIHandleFree stmt failed")

$status:=OCIHandleFree($authp)
ASSERT($status=OCI_SUCCESS; "OCIHandleFree session failed")

$status:=OCIHandleFree($svchp)
ASSERT($status=OCI_SUCCESS; "OCIHandleFree svcctx failed")

$status:=OCIHandleFree($srvhp)
ASSERT($status=OCI_SUCCESS; "OCIHandleFree server failed")

$status:=OCIHandleFree($errhp)
ASSERT($status=OCI_SUCCESS; "OCIHandleFree error failed")

// Free non-existent handle
$status:=OCIHandleFree(99999)
ASSERT($status=OCI_ERROR; "OCIHandleFree non-existent should fail")

// ---- Cleanup ----
$status:=OCITerminate
ASSERT($status=OCI_SUCCESS; "OCITerminate failed")
