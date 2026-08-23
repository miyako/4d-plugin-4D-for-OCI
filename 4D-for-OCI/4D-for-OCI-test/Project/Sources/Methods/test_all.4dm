//%attributes = {"invisible":true}
If (Application info.headless)

    test_extras
    test_handles
    test_error
    test_connection
    test_attr
    test_stmt
    test_transaction
    test_describe
    test_server
    test_math
    test_cache
    test_lob

    LOG EVENT(Into system standard outputs; "PASS"; Information message)

End if
