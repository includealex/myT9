cmake_minimum_required(VERSION 3.16.3)
project(RBTreeTest)

include(FetchContent)
FetchContent_Declare(googletest
        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
set(BUILD_GTEST ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

file(GLOB_RECURSE RBTREE_TEST_SOURCES redblack.cpp)
set(EXEC_NAME redblack_test)

add_executable(${EXEC_NAME} ${RBTREE_TEST_SOURCES})

add_custom_target(run_${EXEC_NAME}
                  COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${EXEC_NAME}
)

add_dependencies(run_${EXEC_NAME} ${EXEC_NAME})

if(ADD_SANITIZERS)
    target_compile_options(${EXEC_NAME} PUBLIC -fsanitize=address -g)
    set_target_properties(${EXEC_NAME} PROPERTIES LINK_FLAGS "-fsanitize=address")
endif()

target_link_libraries(${EXEC_NAME} gtest_main rbtree)

include(GoogleTest)
gtest_discover_tests(${EXEC_NAME} PROPERTIES LABELS "unit")