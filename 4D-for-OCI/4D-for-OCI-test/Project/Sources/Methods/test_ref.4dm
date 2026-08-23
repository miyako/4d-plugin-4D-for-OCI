//%attributes = {"invisible":true}
// Phase 3a: Ref commands (7) — null-handle guard tests

C_LONGINT($envhp; $errhp; $status)
$status:=OCIEnvCreate($envhp; 2)
ASSERT($status=0; "OCIEnvCreate failed")
$status:=OCIHandleAlloc($envhp; $errhp; 2)
ASSERT($status=0; "error handle alloc failed")

// --- OCIRefIsNull with null ref → should return 1 (true) ---
$status:=OCIRefIsNull($envhp; 0)
ASSERT($status=1; "OCIRefIsNull(null) should return 1 (true)")

// --- OCIRefIsEqual with null refs → should return 0 (false) ---
$status:=OCIRefIsEqual($envhp; 0; 0)
ASSERT($status=0; "OCIRefIsEqual(null, null) should return 0")

// --- OCIRefHexSize with null ref → should return 0 ---
$status:=OCIRefHexSize($envhp; 0)
ASSERT($status=0; "OCIRefHexSize(null) should return 0")

// --- OCIRefClear with null ref — should not crash ---
OCIRefClear($envhp; 0)

// --- OCIRefAssign with null source → should return error ---
C_LONGINT($tgtId)
$status:=OCIRefAssign($envhp; $errhp; 0; $tgtId)
ASSERT($status#0; "OCIRefAssign(null) should fail")

// --- OCIRefToHex with null ref → should return error ---
C_TEXT($hexOut)
$status:=OCIRefToHex($envhp; $errhp; 0; ->$hexOut)
ASSERT($status#0; "OCIRefToHex(null) should fail")

// --- OCIRefFromHex with empty string → should return error ---
C_LONGINT($refId)
$status:=OCIRefFromHex($envhp; $errhp; 0; ""; $refId)
ASSERT($status#0; "OCIRefFromHex(empty) should fail")

// Cleanup
$status:=OCIHandleFree($errhp)
$status:=OCIHandleFree($envhp)

LOG EVENT(Into system standard outputs; "test_ref: PASS"; Information message)
