cmake_minimum_required (VERSION 2.6)
project (TextMining)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O5 -std=c++14")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED on)

include_directories ( include )
add_subdirectory(src)

add_executable(TextMiningApp src/TextMiningAppMain.cpp src/utils/CommandLineArguments.cpp)
add_executable(TextMiningCompiler src/TextMiningCompilerMain.cpp src/utils/CommandLineArguments.cpp src/utils/WordsContainer.cpp)

target_link_libraries (TextMiningApp TextMiningLib)
target_link_libraries (TextMiningCompiler TextMiningLib)
