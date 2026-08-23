//%attributes = {"invisible":true,"preemptive":"capable"}
// Test Extras commands (no Oracle DB required)
// Tests: OCISetEnv, OCIGetEnv, OCIGetTnsnamesPath, OCIOnErrCall

var $status : Integer

// ---- OCISetEnv / OCIGetEnv ----
$status:=OCISetEnv("OCI_TEST_VAR"; "hello_4d")
ASSERT($status=OCI_SUCCESS; "OCISetEnv failed: "+String($status))

var $val : Text
$status:=OCIGetEnv("OCI_TEST_VAR"; ->$val)
ASSERT($status=OCI_SUCCESS; "OCIGetEnv failed: "+String($status))
ASSERT($val="hello_4d"; "OCIGetEnv returned wrong value: '"+$val+"'")

// Overwrite with new value
$status:=OCISetEnv("OCI_TEST_VAR"; "updated_value")
ASSERT($status=OCI_SUCCESS; "OCISetEnv overwrite failed")

$status:=OCIGetEnv("OCI_TEST_VAR"; ->$val)
ASSERT($status=OCI_SUCCESS; "OCIGetEnv after overwrite failed")
ASSERT($val="updated_value"; "OCIGetEnv overwrite mismatch: '"+$val+"'")

// Get non-existent variable
$status:=OCIGetEnv("OCI_NONEXISTENT_VAR_12345"; ->$val)
ASSERT($status=OCI_ERROR; "OCIGetEnv should fail for non-existent var")

// Unicode value
$status:=OCISetEnv("OCI_TEST_UNICODE"; "日本語テスト")
ASSERT($status=OCI_SUCCESS; "OCISetEnv unicode failed")

$status:=OCIGetEnv("OCI_TEST_UNICODE"; ->$val)
ASSERT($status=OCI_SUCCESS; "OCIGetEnv unicode failed")
ASSERT($val="日本語テスト"; "OCIGetEnv unicode mismatch: '"+$val+"'")

// Empty value
$status:=OCISetEnv("OCI_TEST_EMPTY"; "")
ASSERT($status=OCI_SUCCESS; "OCISetEnv empty failed")

$status:=OCIGetEnv("OCI_TEST_EMPTY"; ->$val)
ASSERT($status=OCI_SUCCESS; "OCIGetEnv empty should succeed")

// ---- OCIGetTnsnamesPath ----
// Returns empty string if TNS_ADMIN is not set, or path/tnsnames.ora if set
var $path : Text
$status:=OCISetEnv("TNS_ADMIN"; "/tmp/oracle")
$path:=OCIGetTnsnamesPath
ASSERT($path="/tmp/oracle/tnsnames.ora"; "OCIGetTnsnamesPath mismatch: '"+$path+"'")

// ---- OCIOnErrCall ----
// Just test that it doesn't crash; we can't easily verify the callback
OCIOnErrCall("myErrorHandler")

// Clear it
OCIOnErrCall("")

// Clean up test environment variables
$status:=OCISetEnv("OCI_TEST_VAR"; "")
$status:=OCISetEnv("OCI_TEST_UNICODE"; "")
$status:=OCISetEnv("OCI_TEST_EMPTY"; "")
