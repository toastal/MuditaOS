# service-db tests
add_catch2_executable(
        NAME
            service-db-settings
        SRCS
            test-service-db-settings-api.cpp
        LIBS
            db
            evtmgr
            sys
            module-audio
            module-cellular
            module-vfs
            service-audio
            service-cellular
        DEPS
            module-sys
)
