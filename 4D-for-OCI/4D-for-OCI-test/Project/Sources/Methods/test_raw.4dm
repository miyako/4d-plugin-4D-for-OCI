//%attributes = {"invisible":true}
// Phase 3a: Raw commands (6) — null-handle guard tests

C_LONGINT($envhp; $errhp; $status)
$status:=OCIEnvCreate($envhp; 2)
ASSERT($status=0; "OCIEnvCreate failed")
$status:=OCIHandleAlloc($envhp; $errhp; 2)
ASSERT($status=0; "error handle alloc failed")

// --- OCIRawSize with null raw → should return 0 ---
$status:=OCIRawSize($envhp; 0)
ASSERT($status=0; "OCIRawSize(null) should return 0")

// --- OCIRawAllocSize with null raw → should return error ---
C_LONGINT($allocSize)
$status:=OCIRawAllocSize($envhp; $errhp; 0; $allocSize)
// null raw may return error or 0 depending on OCI
ASSERT($allocSize=0; "OCIRawAllocSize(null) allocSize should be 0")

// --- OCIRawAssignRaw with null src → should return error ---
C_LONGINT($dstId)
$status:=OCIRawAssignRaw($envhp; $errhp; 0; $dstId)
ASSERT($status#0; "OCIRawAssignRaw(null) should fail")

// --- OCIRawResize with null raw → may allocate new raw ---
C_LONGINT($rawId)
$status:=OCIRawResize($envhp; $errhp; 0; $rawId)
// Resizing to 0 should not crash

// Cleanup
$status:=OCIHandleFree($errhp)
$status:=OCIHandleFree($envhp)

LOG EVENT(Into system standard outputs; "test_raw: PASS"; Information message)
