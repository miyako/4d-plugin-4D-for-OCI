//%attributes = {"invisible":true}
// Phase 3a: Date commands (8) — without Oracle connection

C_LONGINT($err; $status)
C_DATE($dateIn; $dateOut; $dateOut2)
C_TIME($timeIn; $timeOut)

// Create env + error handle for OCI calls
C_LONGINT($envhp; $errhp)
$status:=OCIEnvCreate($envhp; 2)
ASSERT($status=0; "OCIEnvCreate failed: "+String($status))
$status:=OCIHandleAlloc($envhp; $errhp; 2)
ASSERT($status=0; "error handle alloc failed")

// --- OCIDateSysDate ---
$status:=OCIDateSysDate($errhp; $dateOut; $timeOut)
ASSERT($status=0; "OCIDateSysDate failed: "+String($status))
ASSERT(Year of($dateOut)>=2024; "OCIDateSysDate returned invalid year")

// --- OCIDateAddDays ---
$dateIn:=!2024-01-15!
$status:=OCIDateAddDays($errhp; $dateIn; 10; $dateOut)
ASSERT($status=0; "OCIDateAddDays failed: "+String($status))
ASSERT($dateOut=!2024-01-25!; "OCIDateAddDays: expected 2024-01-25")

// --- OCIDateAddDays negative ---
$status:=OCIDateAddDays($errhp; $dateIn; -5; $dateOut)
ASSERT($status=0; "OCIDateAddDays negative failed")
ASSERT($dateOut=!2024-01-10!; "OCIDateAddDays negative: expected 2024-01-10")

// --- OCIDateAddMonths ---
$dateIn:=!2024-01-31!
$status:=OCIDateAddMonths($errhp; $dateIn; 1; $dateOut)
ASSERT($status=0; "OCIDateAddMonths failed: "+String($status))
// Jan 31 + 1 month = Feb 29 (2024 is leap year)
ASSERT($dateOut=!2024-02-29!; "OCIDateAddMonths: expected 2024-02-29, got "+String($dateOut))

// --- OCIDateLastDay ---
$dateIn:=!2024-02-15!
$status:=OCIDateLastDay($errhp; $dateIn; $dateOut)
ASSERT($status=0; "OCIDateLastDay failed: "+String($status))
ASSERT($dateOut=!2024-02-29!; "OCIDateLastDay: expected 2024-02-29 (leap year)")

// --- OCIDateNextDay ---
$dateIn:=!2024-01-15!  // Monday
$status:=OCIDateNextDay($errhp; $dateIn; "Friday"; $dateOut)
ASSERT($status=0; "OCIDateNextDay failed: "+String($status))
ASSERT($dateOut=!2024-01-19!; "OCIDateNextDay: expected 2024-01-19 (Friday)")

// --- OCIDateFromText + OCIDateToText round-trip ---
C_TEXT($fmt; $lang)
$fmt:="DD-MON-YYYY HH24:MI:SS"
$lang:=""
$status:=OCIDateFromText($errhp; "15-JAN-2024 14:30:00"; $fmt; $lang; $dateOut; $timeOut)
ASSERT($status=0; "OCIDateFromText failed: "+String($status))
ASSERT($dateOut=!2024-01-15!; "OCIDateFromText: wrong date")

C_TEXT($resultText)
$status:=OCIDateToText($errhp; $dateOut; $timeOut; "YYYY-MM-DD"; ""; ->$resultText)
ASSERT($status=0; "OCIDateToText failed: "+String($status))

// Cleanup
$status:=OCIHandleFree($errhp)
$status:=OCIHandleFree($envhp)

LOG EVENT(Into system standard outputs; "test_date: PASS"; Information message)
