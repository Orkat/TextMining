cmake_minimum_required (VERSION 2.6)
project (TextMining)


#define PROJECT_SOURCE_DIR @src@

include_directories ( include )
file(GLOB SOURCES "src/*.cpp")


add_executable(TextMining ${SOURCES})
