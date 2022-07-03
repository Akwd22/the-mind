# CMakeList.txt : Common source files between all subprojects.

# Include shared libraries.
add_library (shared_socket STATIC "socket/utils_io.c" "socket/utils_io.h" "utils.c")
add_library (shared_messaging INTERFACE)

# Add include search paths.
target_include_directories (shared_socket PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories (shared_messaging INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/messaging")