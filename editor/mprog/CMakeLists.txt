cmake_minimum_required(VERSION 3.16.3)
project(EditorRun)

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)

file(GLOB_RECURSE MAIN_SOURCES myT9.cpp)
set(EDITOR_MAIN_NAME editor_main)

add_executable(${EDITOR_MAIN_NAME} ${MAIN_SOURCES})

target_link_libraries(${EDITOR_MAIN_NAME} editor Threads::Threads)

add_custom_target(run_${EDITOR_MAIN_NAME}
 	COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${EDITOR_MAIN_NAME})

add_dependencies(run_${EDITOR_MAIN_NAME} ${EDITOR_MAIN_NAME})