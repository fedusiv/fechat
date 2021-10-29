cmake_minimum_required(VERSION 3.10)

# set the project name
project(fechat)

#target_include_directories(fechat PUBLIC ${PROJECT_SOURCE_DIR}/include)
set(UI_DIR ${PROJECT_SOURCE_DIR}/ui)
set(UI_INCLUE_DIR ${UI_DIR}/include)
set(UI_SRC_DIR ${UI_DIR}/src)

set(SOURCES main.c ${UI_INCLUE_DIR}/display.h ${UI_SRC_DIR}/display.c)
# add the executable
add_executable(fechat ${SOURCES})

target_link_libraries(fechat pthread)