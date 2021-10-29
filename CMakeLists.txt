cmake_minimum_required(VERSION 3.10)

# set the project name
project(fechat)

# Ui files
file(GLOB UI_INCLUDE_FILES "ui/include/*.h") 
file(GLOB UI_SRC_FILES "ui/src/*.c")

set(UI_FILES ${UI_INCLUDE_FILES} ${UI_SRC_FILES})

# connect files
file(GLOB CONNECT_INCLUDE_FILES "connection/include/*.h") 
file(GLOB CONNECT_SRC_FILES "connection/src/*.c")

set(CONNECT_FILES ${CONNECT_SRC_FILES} ${CONNECT_INCLUDE_FILES})

# Common files
file(GLOB COMMON_INCLUDE_FILES "common/include/*.h") 
file(GLOB COMMON_SRC_FILES "common/src/*.c")

set(COMMON_FILES ${COMMON_SRC_FILES} ${COMMON_INCLUDE_FILES})

#input capture files
file(GLOB INPUT_INCLUDE_FILES "input_capture/include/*.h") 
file(GLOB INPUT_SRC_FILES "input_capture/src/*.c")

set(INPUT_FILES ${INPUT_INCLUDE_FILES} ${INPUT_SRC_FILES})

set(SOURCES main.c ${UI_FILES} ${CONNECT_FILES} ${COMMON_FILES} ${INPUT_FILES})
# add the executable
add_executable(fechat ${SOURCES})

target_link_libraries(fechat pthread)