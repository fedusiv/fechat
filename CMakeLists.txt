cmake_minimum_required(VERSION 3.16)

# By default clang for MacOS. GCC for Linux. It can be unify in future
if(UNIX)
    if(APPLE)
        set(CMAKE_HOST_SYSTEM_PROCESSOR arm64)
        set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
        set(CMAKE_C_COMPILER "/usr/bin/clang")
        add_compile_definitions(MACOSX)
        message("Running on MacOS systems.") 
    else()
        set(CMAKE_CXX_COMPILER "/usr/bin/g++")
        set(CMAKE_C_COMPILER "/usr/bin/gcc")
        add_compile_definitions(UNIX)
        message("Running on Linux systems.")
    endif()
endif()
# set the project name
project(fechat)

# Ui files
file(GLOB UI_SRC_FILES "ui/*.c")
# connect files
file(GLOB CONNECT_SRC_FILES "connection/*.c")
# Common files
file(GLOB COMMON_SRC_FILES "common/*.c")
#input capture files
file(GLOB INPUT_SRC_FILES "input_capture/*.c")

set(SOURCE_FILES ${UI_SRC_FILES} ${CONNECT_SRC_FILES} ${COMMON_SRC_FILES} ${INPUT_SRC_FILES})
set(SOURCES main.c ${SOURCE_FILES})
# add the executable
add_executable(${PROJECT_NAME} ${SOURCES})


target_include_directories(${PROJECT_NAME} PRIVATE "ui")
target_include_directories(${PROJECT_NAME} PRIVATE "connection")
target_include_directories(${PROJECT_NAME} PRIVATE "common")
target_include_directories(${PROJECT_NAME} PRIVATE "input_capture")

target_link_libraries(${PROJECT_NAME} pthread)