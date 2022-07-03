# CMakeList.txt : CMake project for BotProject, include source and define
# project specific logic here.
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (BotProject
	"main.c"
	"socket.c" "socket.h"
	"handlers.c"
	"logic.c" "logic.h"
)

# Link libraries to this project.
target_link_libraries(BotProject PRIVATE sds)

# Link shared source files to this project.
target_link_libraries(BotProject PRIVATE shared_socket)

# Compile options.
target_compile_options (BotProject PRIVATE -Wall -Wextra -pedantic -Wno-unused-parameter -Wno-return-type -Wno-unused-variable)