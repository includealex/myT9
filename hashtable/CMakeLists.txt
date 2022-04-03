cmake_minimum_required(VERSION 3.16.3)
project(HashTableLib VERSION 1.0.0 DESCRIPTION "Hashtable lib")

add_subdirectory(tests)
add_subdirectory(bench)

file(GLOB_RECURSE HASHTABLE_HEADERS *.hpp)
file(GLOB_RECURSE HASHTABLE_SOURCES *.cpp)

add_library(hashtable STATIC ${HASHTABLE_SOURCES} ${HASHTABLE_HEADERS})
target_link_libraries(hashtable rbtree)

set(HASHTABLE_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/includes)
target_include_directories(hashtable PUBLIC ${HASHTABLE_INCLUDE})

set_target_properties (hashtable PROPERTIES
    CXX_STANDARD          11
    CXX_STANDARD_REQUIRED ON
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g")