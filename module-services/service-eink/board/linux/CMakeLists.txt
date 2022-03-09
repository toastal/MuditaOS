target_include_directories(
        service-eink
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)
add_subdirectory(renderer)

include_directories("/opt/homebrew/include"
        /opt/homebrew/opt
        /opt/homebrew/opt/glib
        /opt/homebrew/include/glib-2.0
        )
