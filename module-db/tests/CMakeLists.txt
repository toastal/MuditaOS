add_catch2_executable(
    NAME
        db
    SRCS
        AlarmEventRecord_tests.cpp
        CalllogRecord_tests.cpp
        CalllogTable_tests.cpp
        ContactGroups_tests.cpp
        ContactsAddressTable_tests.cpp
        ContactsNameTable_tests.cpp
        ContactsNumberTable_tests.cpp
        ContactsRecord_tests.cpp
        ContactsRingtonesTable_tests.cpp
        ContactsTable_tests.cpp
        DbInitializer.cpp
        MultimediaFilesTable_tests.cpp
        NotesRecord_tests.cpp
        NotesTable_tests.cpp
        NotificationsRecord_tests.cpp
        NotificationsTable_tests.cpp
        QueryInterface.cpp
        SMSRecord_tests.cpp
        SMSTable_tests.cpp
        SMSTemplateRecord_tests.cpp
        SMSTemplateTable_tests.cpp
        ThreadRecord_tests.cpp
        ThreadsTable_tests.cpp
        common.cpp
        
    LIBS
        module-sys
        module-db
        json::json
    USE_FS
)
add_subdirectory(test-initializer)
