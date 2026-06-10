cmake_path(GET CMAKE_CURRENT_SOURCE_DIR FILENAME PROJECT_NAME)
string(REPLACE " " "_" ProjectId ${PROJECT_NAME})
project(${PROJECT_NAME})

include(setup-tools)
include(embed)

include_directories(../extern/reader/pugixml/pugixml/src)

add_compile_definitions(ELF_NAME="${PROJECT_NAME}.elf")

add_executable(${PROJECT_NAME}
        main.cpp
        ../patch-server/patch_web_xml.cpp
        $<$<BOOL:${PROSPERO}>:install.c>
)

embed_files(${PROJECT_NAME}
    ../extern/web/materialize/dist/js/materialize.min.js
    ../extern/web/materialize/dist/css/materialize.min.css
    assets/roboto-v51-latin-regular.woff2
    assets/roboto-v51-latin-500.woff2
    assets/roboto-mono-v31-latin-regular.woff2
    ../data/sce_sys/icon0.png
)

include(optimization-settings)

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        pugixml
        sqlite3
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
