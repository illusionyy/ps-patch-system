cmake_path(GET CMAKE_CURRENT_SOURCE_DIR FILENAME PROJECT_NAME)
string(REPLACE " " "_" ProjectId ${PROJECT_NAME})
project(${PROJECT_NAME})

include(c-standard)

file(GLOB SOURCES "*.c" "*.cpp")

add_library(${PROJECT_NAME} STATIC ${SOURCES})

include(optimization-settings)
