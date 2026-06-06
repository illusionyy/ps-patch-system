cmake_path(GET CMAKE_CURRENT_SOURCE_DIR FILENAME PROJECT_NAME)
string(REPLACE " " "_" ProjectId ${PROJECT_NAME})
project(${PROJECT_NAME})

include(setup-tools)

include_directories(
        ../extern/reader/pugixml/pugixml/src
        ../extern/hook/minhook/minhook/src/hde
)

add_compile_definitions(ELF_NAME="${PROJECT_NAME}.elf")
add_executable(${PROJECT_NAME}
        main.cpp
        patch_signal.cpp
        patch_reg.hpp
        patch_user_xml.cpp
        patch_web_xml.cpp
        server_patch.cpp
        server_signal.cpp
        server_thread.cpp
        server_120hz_patch.cpp
        patch_reg.cpp
)

include(optimization-settings)

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        pugixml
        SceSystemService
        hde
        shared
)

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${PROJECT_NAME}.elf)

add_custom_target(${PROJECT_NAME}_test
        USES_TERMINAL
        COMMAND ${PROG_DEPLOY} ${PROJECT_NAME}.elf
        DEPENDS ${PROJECT_NAME}
)
