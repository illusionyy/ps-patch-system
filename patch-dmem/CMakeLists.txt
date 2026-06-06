cmake_path(GET CMAKE_CURRENT_SOURCE_DIR FILENAME PROJECT_NAME)
string(REPLACE " " "_" ProjectId ${PROJECT_NAME})
project(${PROJECT_NAME})

include(setup-tools)
include(embed)

add_compile_definitions(ELF_NAME="${PROJECT_NAME}.elf")
add_executable(${PROJECT_NAME}
        dmem.c
)

include(optimization-settings)

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        shared
)

set(OUTPUT_ELF ${PROJECT_NAME}-${PLATFORM}.elf)
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${OUTPUT_ELF})

add_custom_target(${PROJECT_NAME}_test
        USES_TERMINAL
        COMMAND ${PROG_DEPLOY} ${OUTPUT_ELF}
        DEPENDS ${PROJECT_NAME}
)
