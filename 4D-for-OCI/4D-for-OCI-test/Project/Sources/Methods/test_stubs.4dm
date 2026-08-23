//%attributes = {"invisible":true}
// Phase 3b: Collection, Iterator, Table stubs — all should return OCI_ERROR (-1 or non-zero)

C_LONGINT($status)

// --- Collection stubs ---
$status:=OCICollAppend(0; 0; 0; 0; 0)
ASSERT($status#0; "OCICollAppend stub should return error")

$status:=OCICollAssign(0; 0; 0; 0)
ASSERT($status#0; "OCICollAssign stub should return error")

$status:=OCICollMax(0; 0)
ASSERT($status=0; "OCICollMax stub returns 0")

$status:=OCICollSize(0; 0; 0; 0)
ASSERT($status#0; "OCICollSize stub should return error")

$status:=OCICollTrim(0; 0; 0; 0)
ASSERT($status#0; "OCICollTrim stub should return error")

// --- Iterator stubs ---
$status:=OCIIterCreate(0; 0; 0; 0)
ASSERT($status#0; "OCIIterCreate stub should return error")

$status:=OCIIterDelete(0; 0; 0)
ASSERT($status#0; "OCIIterDelete stub should return error")

// --- Table stubs ---
$status:=OCITableDelete(0; 0; 0; 0)
ASSERT($status#0; "OCITableDelete stub should return error")

$status:=OCITableFirst(0; 0; 0; 0)
ASSERT($status#0; "OCITableFirst stub should return error")

$status:=OCITableSize(0; 0; 0; 0)
ASSERT($status#0; "OCITableSize stub should return error")

LOG EVENT(Into system standard outputs; "test_stubs: PASS"; Information message)
