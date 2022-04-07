cmake_minimum_required(VERSION 3.16.3)
project(DictionaryLib VERSION 1.0.0 DESCRIPTION "Dictionary library")

file(GLOB_RECURSE ALL_CXX_SOURCE_FILES ${PROJECT_SOURCE_DIR}/*.[ch]pp)

add_library(dictionary STATIC ${ALL_CXX_SOURCE_FILES})
target_link_libraries(dictionary hashtable)

target_include_directories(dictionary PUBLIC includes/)

add_subdirectory(tests)

set_target_properties (dictionary PROPERTIES
    CXX_STANDARD          11
    CXX_STANDARD_REQUIRED ON
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g")