cmake_minimum_required(VERSION 3.16.3)
project(EditorTest)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRECTORIES})

enable_testing()

file(GLOB_RECURSE EDITOR_TESTS_SOURCES editor_tests.cpp)
set(EXEC_NAME editor_test)

add_executable(${EXEC_NAME} ${EDITOR_TESTS_SOURCES})

if(SANITIZERS)
	target_compile_options(${EXEC_NAME} PUBLIC -fsanitize=address -g)
	set_target_properties(${EXEC_NAME} PROPERTIES LINK_FLAGS "-fsanitize=address")
endif()

target_link_libraries(${EXEC_NAME} GTest::Main editor)

add_custom_target(run_${EXEC_NAME}
	  COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${EXEC_NAME})

add_dependencies(run_${EXEC_NAME} ${EXEC_NAME})