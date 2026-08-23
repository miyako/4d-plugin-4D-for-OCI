//%attributes = {"invisible":true,"preemptive":"capable"}
// Test environment variable functions
$status:=OCISetEnv:P11900:38("OCI_TEST_VAR"; "hello_4d")
ASSERT:C1129($status=OCI_SUCCESS:K11903:1; "OCISetEnv failed")

var $val : Text
$status:=OCIGetEnv:P11900:39("OCI_TEST_VAR"; ->$val)
ASSERT:C1129($status=OCI_SUCCESS:K11903:1; "OCIGetEnv failed")
ASSERT:C1129($val="hello_4d"; "OCIGetEnv returned wrong value: "+$val)



// Test basic OCI environment creation and teardown
// This test does NOT require an Oracle database connection.

var $envhp : Integer
var $errhp : Integer
var $status : Integer

// Create environment
$status:=OCIEnvCreate:P11900:1($envhp; OCI_DEFAULT:K11901:1)
ASSERT:C1129($status=OCI_SUCCESS:K11903:1; "OCIEnvCreate failed: "+String:C10($status))
ASSERT:C1129($envhp#0; "OCIEnvCreate returned null handle")

// Allocate error handle
$status:=OCIHandleAlloc:P11900:2($envhp; $errhp; OCI_HTYPE_ERROR:K11900:3)
ASSERT:C1129($status=OCI_SUCCESS:K11903:1; "OCIHandleAlloc (error) failed: "+String:C10($status))
ASSERT:C1129($errhp#0; "OCIHandleAlloc returned null error handle")

// Free error handle
$status:=OCIHandleFree:P11900:3($errhp)
ASSERT:C1129($status=OCI_SUCCESS:K11903:1; "OCIHandleFree (error) failed: "+String:C10($status))

// Terminate
$status:=OCITerminate:P11900:35
ASSERT:C1129($status=OCI_SUCCESS:K11903:1; "OCITerminate failed: "+String:C10($status))


