cmake_minimum_required(VERSION 2.6)
project(pcp-machine)

find_package(PkgConfig REQUIRED)
#find_package(Lua REQUIRED)
pkg_check_modules(LUA5.3 REQUIRED lua5.3)

include_directories(${LUA5.3_INCLUDE_DIRS})
link_directories(${LUA5.3_LIBRARY_DIRS})

#pkg_check_modules(LUA5.3 required lua5.3)

set(SOURCE_FILES pcp-machine.c pcp_ez.c pcp_ez.h)
add_executable(pcp-machine ${SOURCE_FILES})

target_link_libraries(pcp-machine lua5.3 pcp)