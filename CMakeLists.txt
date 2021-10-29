cmake_minimum_required(VERSION 3.10)

# set the project name
project(fechat)

# Ui files
set(UI_DIR ${PROJECT_SOURCE_DIR}/ui)
set(UI_INCLUDE_DIR ${UI_DIR}/include)
set(UI_SRC_DIR ${UI_DIR}/src)
set(UI_FILES ${UI_INCLUDE_DIR}/display.h ${UI_SRC_DIR}/display.c)

# connect files
set(CONNECT_DIR ${PROJECT_SOURCE_DIR}/connection)
set(CONNECT_INCLUDE_DIR ${CONNECT_DIR}/include)
set(CONNECT_SRC_DIR ${CONNECT_DIR}/src)
set(CONNECT_SRC_FILES ${CONNECT_SRC_DIR}/connect.c)
set(CONNECT_INCLUDE_FILES ${CONNECT_INCLUDE_DIR}/connect.h ${CONNECT_INCLUDE_DIR}/connection_defines.h)
set(CONNECT_FILES ${CONNECT_SRC_FILES} ${CONNECT_INCLUDE_FILES})

# Common files
set(COMMON_DIR ${PROJECT_SOURCE_DIR}/common)
set(COMMON_INCLUDE_DIR ${COMMON_DIR}/include)
set(COMMON_SRC_DIR ${COMMON_DIR}/src)
set(COMMON_SRC_FILES ${COMMON_SRC_DIR}/common.c)
set(COMMON_INCLUDE_FILES ${COMMON_INCLUDE_DIR}/common.h)
set(COMMON_FILES ${COMMON_SRC_FILES} ${COMMON_INCLUDE_FILES})

set(SOURCES main.c ${UI_FILES} ${CONNECT_FILES} ${COMMON_FILES})
# add the executable
add_executable(fechat ${SOURCES})

target_link_libraries(fechat pthread)