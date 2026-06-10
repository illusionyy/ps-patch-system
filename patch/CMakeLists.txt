cmake_path(GET CMAKE_CURRENT_SOURCE_DIR FILENAME PROJECT_NAME)
string(REPLACE " " "_" ProjectId ${PROJECT_NAME})
project(${PROJECT_NAME})

include(setup-tools)

add_compile_definitions(ELF_NAME="${PROJECT_NAME}.elf")
add_executable(${PROJECT_NAME}
        hide_app_sql.c
        patch_shellcore.c
        main.cpp
)

include(optimization-settings)

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        $<$<BOOL:${ORBIS}>:sqlite3>
        kernel_sys
        SceSystemService
        SceUserService
        shared
)

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${PROJECT_NAME}.elf)

add_custom_target(${PROJECT_NAME}_test
        USES_TERMINAL
        COMMAND ${PROG_DEPLOY} ${PROJECT_NAME}.elf
        DEPENDS ${PROJECT_NAME}
)
