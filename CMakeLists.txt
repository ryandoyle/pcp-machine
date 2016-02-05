cmake_minimum_required(VERSION 2.8)
project(pcp-machine)

find_package(Lua51)
include_directories(${LUA_INCLUDE_DIR})

set(SOURCE_FILES pcp-machine.c pcp_ez.c pcp_ez.h)
add_executable(pcp-machine ${SOURCE_FILES})

target_link_libraries(pcp-machine pcp ${LUA_LIBRARIES})

install(TARGETS pcp-machine DESTINATION bin)
install(DIRECTORY samples/ DESTINATION share/pcp-machine PATTERN "scripts/*.lua")
install(FILES doc/pcp-machine.1 DESTINATION share/man/man1)

enable_testing()
add_subdirectory(test)