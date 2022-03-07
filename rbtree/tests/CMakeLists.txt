cmake_minimum_required(VERSION 3.16.3)
project(RBTreeTest)

enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIR})

file(GLOB_RECURSE RBTREE_TEST_SOURCES redblack.cpp)
set(EXEC_NAME redblack_test)

add_executable(${EXEC_NAME} ${RBTREE_TEST_SOURCES})

if(ADD_SANITIZERS)
    target_compile_options(${EXEC_NAME} PUBLIC -fsanitize=address -g)
    set_target_properties(${EXEC_NAME} PROPERTIES LINK_FLAGS "-fsanitize=address")
endif()

target_link_libraries(${EXEC_NAME} gtest_main rbtree)

add_custom_target(run_${EXEC_NAME}
                  COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${EXEC_NAME}
)

add_dependencies(run_${EXEC_NAME} ${EXEC_NAME})