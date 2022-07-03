# CMakeList.txt : Include and configure global libraries used throughout all subprojects here.

# Include global libraries.
add_library (sds STATIC "sds/sds.c" "sds/sds.h" "sds/sdsalloc.h")

# Add include search paths.
target_include_directories (sds PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")