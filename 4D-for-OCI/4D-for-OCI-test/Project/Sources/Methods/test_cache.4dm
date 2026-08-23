//%attributes = {"invisible":true,"preemptive":"capable"}
// Test Types/Cache commands (no Oracle DB required for basic calls)
// These commands require a service context, so we guard with null-handle checks.

var $status : Integer
var $envhp : Integer
var $errhp : Integer

// ---- Setup ----
$status:=OCIEnvCreate($envhp; OCI_DEFAULT)
ASSERT($status=OCI_SUCCESS; "OCIEnvCreate failed")

$status:=OCIHandleAlloc($envhp; $errhp; OCI_HTYPE_ERROR)
ASSERT($status=OCI_SUCCESS; "OCIHandleAlloc error failed")

// ---- OCICacheFree — with null svc should return error gracefully ----
$status:=OCICacheFree($envhp; $errhp; 0)
// Expected: OCI_ERROR (no valid svc context)
ASSERT($status#OCI_SUCCESS; "OCICacheFree with null svc should fail")

// ---- OCICacheUnmark — with null svc should return error ----
$status:=OCICacheUnmark($envhp; $errhp; 0)
ASSERT($status#OCI_SUCCESS; "OCICacheUnmark with null svc should fail")

// ---- OCICacheUnpin — with null svc should return error ----
$status:=OCICacheUnpin($envhp; $errhp; 0)
ASSERT($status#OCI_SUCCESS; "OCICacheUnpin with null svc should fail")

// ---- Cleanup ----
$status:=OCIHandleFree($errhp)
ASSERT($status=OCI_SUCCESS; "OCIHandleFree error failed")

$status:=OCITerminate
ASSERT($status=OCI_SUCCESS; "OCITerminate failed")
