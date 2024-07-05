
set(CMAKE_C_FLAGS_DEBUG "-Wall -DDEBUG")
set(CMAKE_C_FLAGS_RELEASE "-Wall -Werror=return-type -Werror=unused-variable")
set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")

cmake_minimum_required(VERSION 3.5)
project(sicalc_cli)

include_directories(include)

add_library(sicalc SHARED "src/sicalc.c")
target_include_directories(sicalc PRIVATE src include)

add_executable(cli_sicalc main.c)
target_link_libraries(cli_sicalc sicalc)

add_executable(test test.c)
target_link_libraries(test sicalc)