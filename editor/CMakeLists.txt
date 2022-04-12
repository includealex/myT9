cmake_minimum_required(VERSION 3.16.3)
project(EditorLib VERSION 1.0.0 DESCRIPTION "Editor library")

file(GLOB_RECURSE ALL_CXX_SOURCE_FILES ${PROJECT_SOURCE_DIR}/*.[ch]pp)

add_library(editor STATIC ${ALL_CXX_SOURCE_FILES})
target_link_libraries(editor dictionary)

target_include_directories(editor PUBLIC includes/)

add_subdirectory(tests)

set_target_properties (editor PROPERTIES
    CXX_STANDARD          11
    CXX_STANDARD_REQUIRED ON
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g")