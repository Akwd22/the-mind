# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
cmake_minimum_required (VERSION 3.8)

project ("TheMind")

# Include sub-directories.
add_subdirectory ("Libs")
add_subdirectory ("Shared")
add_subdirectory ("ClientProject")
add_subdirectory ("ServerProject")
add_subdirectory ("BotProject")