//%attributes = {"invisible":true,"preemptive":"capable"}
// Test Statement commands (no Oracle DB required for preparation)
// Tests: OCIStmtPrepare, OCIStmtExecute, OCIStmtFetch
// Execute/Fetch require a live connection; here we test null-handle guards
// and that StmtPrepare works on a standalone statement handle.

var $status : Integer
var $envhp : Integer
var $errhp : Integer
var $stmtp : Integer

// Setup
$status:=OCIEnvCreate($envhp; OCI_DEFAULT)
ASSERT($status=OCI_SUCCESS; "OCIEnvCreate failed")

$status:=OCIHandleAlloc($envhp; $errhp; OCI_HTYPE_ERROR)
ASSERT($status=OCI_SUCCESS; "OCIHandleAlloc error failed")

$status:=OCIHandleAlloc($envhp; $stmtp; OCI_HTYPE_STMT)
ASSERT($status=OCI_SUCCESS; "OCIHandleAlloc stmt failed")

// ---- OCIStmtPrepare — prepare a SELECT statement ----
// OCI_NTV_SYNTAX = 1 (native syntax)
$status:=OCIStmtPrepare($stmtp; $errhp; "SELECT 1 FROM DUAL"; OCI_NTV_SYNTAX)
ASSERT($status=OCI_SUCCESS; "OCIStmtPrepare SELECT failed: "+String($status))

// ---- OCIStmtPrepare — prepare an INSERT statement ----
var $stmtp2 : Integer
$status:=OCIHandleAlloc($envhp; $stmtp2; OCI_HTYPE_STMT)
ASSERT($status=OCI_SUCCESS; "OCIHandleAlloc stmt2 failed")

$status:=OCIStmtPrepare($stmtp2; $errhp; "INSERT INTO t (id, name) VALUES (:1, :2)"; OCI_NTV_SYNTAX)
ASSERT($status=OCI_SUCCESS; "OCIStmtPrepare INSERT failed: "+String($status))

// ---- OCIStmtPrepare — null handle guard ----
$status:=OCIStmtPrepare(0; $errhp; "SELECT 1 FROM DUAL"; OCI_NTV_SYNTAX)
ASSERT($status=OCI_ERROR; "OCIStmtPrepare with null stmt should fail")

// ---- OCIStmtExecute — null handle guards ----
$status:=OCIStmtExecute(0; $stmtp; $errhp; 0; 0; 0; 0; OCI_DEFAULT)
ASSERT($status=OCI_ERROR; "OCIStmtExecute with null svc should fail")

// ---- OCIStmtFetch — null handle guard ----
$status:=OCIStmtFetch(0; $errhp; OCI_NTV_SYNTAX)
ASSERT($status=OCI_ERROR; "OCIStmtFetch with null stmt should fail")

// ---- Cleanup ----
$status:=OCIHandleFree($stmtp2)
$status:=OCIHandleFree($stmtp)
$status:=OCIHandleFree($errhp)
$status:=OCITerminate
ASSERT($status=OCI_SUCCESS; "OCITerminate failed")
