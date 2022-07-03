# CMakeList.txt : CMake project for ServerProject, include source and define
# project specific logic here.
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (ServerProject
	"main.c"
	"socket.c" "socket.h"
	"handlers.c"
	"logic/joueur.c" "logic/joueur.h"
	"logic/lobby.c" "logic/lobby.h"
	"logic/partie.c" "logic/partie.h"
	"logic/stats.c" "logic/stats.h"
)

# Link libraries to this project.
target_link_libraries (ServerProject PRIVATE sds)
target_link_libraries (ServerProject PRIVATE pthread)

# Link shared source files to this project.
target_link_libraries(ServerProject PRIVATE shared_socket)

# Compile options.
target_compile_options (ServerProject PRIVATE -Wall -Wextra -pedantic -Wno-unused-parameter -Wno-return-type -Wno-unused-variable)

# Copy template tex files to build dir.
add_custom_command(
	TARGET ServerProject POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
			${CMAKE_CURRENT_SOURCE_DIR}/template
			${CMAKE_CURRENT_BINARY_DIR}/template)