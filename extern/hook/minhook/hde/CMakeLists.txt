cmake_path(GET CMAKE_CURRENT_SOURCE_DIR FILENAME PROJECT_NAME)
string(REPLACE " " "_" ProjectId ${PROJECT_NAME})
project(${PROJECT_NAME})

set(SOURCES
    ../minhook/src/hde/hde64.c
)

add_library(${PROJECT_NAME} STATIC ${SOURCES})

include(optimization-settings)
