cmake_minimum_required(VERSION 2.8)
project(pcp-machine C)

find_package(Lua51)
include_directories(${LUA_INCLUDE_DIR})

set(SOURCE_FILES pcp-machine.c pcp_ez.c pcp_ez.h)
add_executable(pcp-machine ${SOURCE_FILES})

target_link_libraries(pcp-machine pcp ${LUA_LIBRARIES})

install(TARGETS pcp-machine DESTINATION bin)
install(DIRECTORY samples/ DESTINATION share/pcp-machine/samples PATTERN "samples/*.lua")
install(FILES doc/pcp-machine.1 DESTINATION share/man/man1)
install(FILES doc/LICENCE.txt DESTINATION share/pcp-machine/)
install(FILES doc/README.md DESTINATION share/pcp-machine/)

enable_testing()
add_subdirectory(test)