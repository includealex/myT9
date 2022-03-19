cmake_minimum_required(VERSION 3.16.3)
project(RBTreeLib VERSION 1.0.0 DESCRIPTION "Red black tree lib")

add_subdirectory(tests)

file(GLOB_RECURSE RBTREE_HEADERS *.hpp)
file(GLOB_RECURSE RBTREE_SOURCES *.cpp)


add_library(rbtree STATIC ${RBTREE_SOURCES} ${RBTREE_HEADERS})

set(RBTREE_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/includes)
target_include_directories(rbtree PUBLIC ${RBTREE_INCLUDE})

set_target_properties (rbtree PROPERTIES
    CXX_STANDARD          11
    CXX_STANDARD_REQUIRED ON
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g")