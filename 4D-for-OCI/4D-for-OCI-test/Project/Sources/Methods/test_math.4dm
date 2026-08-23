//%attributes = {"invisible":true,"preemptive":"capable"}
// Test OCINumber math commands (no Oracle DB required)
// Uses OCIEnvCreate + OCIHandleAlloc for the error handle only.

var $status : Integer
var $envhp : Integer
var $errhp : Integer
var $result : Real

// ---- Setup: create env + error handle ----
$status:=OCIEnvCreate($envhp; OCI_DEFAULT)
ASSERT($status=OCI_SUCCESS; "OCIEnvCreate failed")

$status:=OCIHandleAlloc($envhp; $errhp; OCI_HTYPE_ERROR)
ASSERT($status=OCI_SUCCESS; "OCIHandleAlloc error failed")

// ---- OCINumberAdd ----
$status:=OCINumberAdd($errhp; 2.5; 3.5; $result)
ASSERT($status=OCI_SUCCESS; "OCINumberAdd failed: "+String($status))
ASSERT($result=6; "OCINumberAdd: expected 6, got "+String($result))

// ---- OCINumberSub ----
$status:=OCINumberSub($errhp; 10; 3; $result)
ASSERT($status=OCI_SUCCESS; "OCINumberSub failed")
ASSERT($result=7; "OCINumberSub: expected 7, got "+String($result))

// ---- OCINumberMul ----
$status:=OCINumberMul($errhp; 4; 5; $result)
ASSERT($status=OCI_SUCCESS; "OCINumberMul failed")
ASSERT($result=20; "OCINumberMul: expected 20, got "+String($result))

// ---- OCINumberDiv ----
$status:=OCINumberDiv($errhp; 15; 3; $result)
ASSERT($status=OCI_SUCCESS; "OCINumberDiv failed")
ASSERT($result=5; "OCINumberDiv: expected 5, got "+String($result))

// ---- OCINumberSqrt ----
$status:=OCINumberSqrt($errhp; 25; $result)
ASSERT($status=OCI_SUCCESS; "OCINumberSqrt failed")
ASSERT($result=5; "OCINumberSqrt: expected 5, got "+String($result))

// ---- OCINumberPower ----
$status:=OCINumberPower($errhp; 2; 10; $result)
ASSERT($status=OCI_SUCCESS; "OCINumberPower failed")
ASSERT($result=1024; "OCINumberPower: expected 1024, got "+String($result))

// ---- OCINumberLn ----
$status:=OCINumberLn($errhp; 1; $result)
ASSERT($status=OCI_SUCCESS; "OCINumberLn failed")
ASSERT($result=0; "OCINumberLn(1): expected 0, got "+String($result))

// ---- OCINumberExp ----
$status:=OCINumberExp($errhp; 0; $result)
ASSERT($status=OCI_SUCCESS; "OCINumberExp failed")
ASSERT($result=1; "OCINumberExp(0): expected 1, got "+String($result))

// ---- OCINumberSin ----
$status:=OCINumberSin($errhp; 0; $result)
ASSERT($status=OCI_SUCCESS; "OCINumberSin failed")
ASSERT($result=0; "OCINumberSin(0): expected 0, got "+String($result))

// ---- OCINumberCos ----
$status:=OCINumberCos($errhp; 0; $result)
ASSERT($status=OCI_SUCCESS; "OCINumberCos failed")
ASSERT($result=1; "OCINumberCos(0): expected 1, got "+String($result))

// ---- OCINumberTan ----
$status:=OCINumberTan($errhp; 0; $result)
ASSERT($status=OCI_SUCCESS; "OCINumberTan failed")
ASSERT($result=0; "OCINumberTan(0): expected 0, got "+String($result))

// ---- OCINumberRound ----
$status:=OCINumberRound($errhp; 3.456; 2; $result)
ASSERT($status=OCI_SUCCESS; "OCINumberRound failed")
ASSERT($result=3.46; "OCINumberRound: expected 3.46, got "+String($result))

// ---- OCINumberTrunc ----
$status:=OCINumberTrunc($errhp; 3.456; 2; $result)
ASSERT($status=OCI_SUCCESS; "OCINumberTrunc failed")
ASSERT($result=3.45; "OCINumberTrunc: expected 3.45, got "+String($result))

// ---- OCINumberIntPower ----
$status:=OCINumberIntPower($errhp; 3; 4; $result)
ASSERT($status=OCI_SUCCESS; "OCINumberIntPower failed")
ASSERT($result=81; "OCINumberIntPower: expected 81, got "+String($result))

// ---- Cleanup ----
$status:=OCIHandleFree($errhp)
ASSERT($status=OCI_SUCCESS; "OCIHandleFree error failed")

$status:=OCITerminate
ASSERT($status=OCI_SUCCESS; "OCITerminate failed")
