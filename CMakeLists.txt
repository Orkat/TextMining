cmake_minimum_required (VERSION 2.6)
project (TextMining)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O5 -c++14")

include_directories ( include )
add_subdirectory(src)

add_executable(TextMiningApp src/TextMiningAppMain.cpp)
add_executable(TextMiningCompiler src/TextMiningCompilerMain.cpp)

target_link_libraries (TextMiningApp TextMiningLib)
target_link_libraries (TextMiningCompiler TextMiningLib)
