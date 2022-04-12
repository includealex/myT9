cmake_minimum_required(VERSION 3.16.3)
project(myT9)

add_subdirectory(rbtree)
add_subdirectory(hashtable)
add_subdirectory(Stack)
add_subdirectory(dictionary)
add_subdirectory(editor)

file(GLOB_RECURSE ALL_CXX_SOURCE_FILES *.cpp *.hpp)

find_program(CLANG_FORMAT "clang-format" REQUIRED)

add_custom_target (
    format
    COMMAND clang-format
    -i
    -style=file
    ${ALL_CXX_SOURCE_FILES}
)

set(CMAKE_CXX_CLANG_TIDY
    clang-tidy
    --format-style=file
    --header-filter=*.hpp
)

add_custom_target(run_every_test)
add_dependencies(run_every_test run_stack_test run_redblack_test run_hashtable_test run_dictionary_test run_editor_test)

add_custom_target(run_every_bench)
add_dependencies(run_every_bench run_stack_bench run_htable_all_bench)