//%attributes = {"invisible":true,"preemptive":"capable"}

// Test basic OCI environment creation and teardown
// This test does NOT require an Oracle database connection.

var $envhp : Integer
var $errhp : Integer
var $status : Integer

// Create environment
$status:=OCIEnvCreate($envhp; OCI_DEFAULT)
ASSERT($status=OCI_SUCCESS; "OCIEnvCreate failed: "+String($status))
ASSERT($envhp#0; "OCIEnvCreate returned null handle")

// Allocate error handle
$status:=OCIHandleAlloc($envhp; $errhp; OCI_HTYPE_ERROR)
ASSERT($status=OCI_SUCCESS; "OCIHandleAlloc (error) failed: "+String($status))
ASSERT($errhp#0; "OCIHandleAlloc returned null error handle")

// Free error handle
$status:=OCIHandleFree($errhp)
ASSERT($status=OCI_SUCCESS; "OCIHandleFree (error) failed: "+String($status))

// Terminate
$status:=OCITerminate
ASSERT($status=OCI_SUCCESS; "OCITerminate failed: "+String($status))

// Test environment variable functions
$status:=OCISetEnv("OCI_TEST_VAR"; "hello_4d")
ASSERT($status=OCI_SUCCESS; "OCISetEnv failed")

var $val : Text
$status:=OCIGetEnv("OCI_TEST_VAR"; ->$val)
ASSERT($status=OCI_SUCCESS; "OCIGetEnv failed")
ASSERT($val="hello_4d"; "OCIGetEnv returned wrong value: "+$val)
