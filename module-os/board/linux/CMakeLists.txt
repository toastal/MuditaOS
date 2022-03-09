target_sources(module-os
    PRIVATE
)

target_sources(
        module-os
    PRIVATE
		utils/wait_for_event.c
        port.c 
        fsl_runtimestat_gpt.c
)

target_include_directories(
        module-os
    PUBLIC
    $<BUILD_INTERFACE:
        ${CMAKE_CURRENT_SOURCE_DIR}
    >
)

# suppress unused variable warning
set_source_files_properties(
        port.c 
        fsl_runtimestat_gpt.c
    PROPERTIES COMPILE_FLAGS
	    -Wno-unused-but-set-variable
)
