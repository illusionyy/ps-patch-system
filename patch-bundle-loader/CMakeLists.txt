cmake_path(GET CMAKE_CURRENT_SOURCE_DIR FILENAME PROJECT_NAME)
string(REPLACE " " "_" ProjectId ${PROJECT_NAME})
project(${PROJECT_NAME})

include(setup-tools)
include(embed)

add_compile_definitions(ELF_NAME="${PROJECT_NAME}.elf")
add_executable(${PROJECT_NAME}
        main.cpp
)

add_dependencies(${PROJECT_NAME}
        patch-bundle
)

include_directories(
        $<$<BOOL:${ORBIS}>:${CMAKE_CURRENT_SOURCE_DIR}/../extern/loader/orbis-elfldr/orbis-elfldr>
        $<$<BOOL:${PROSPERO}>:${CMAKE_CURRENT_SOURCE_DIR}/../extern/loader/prospero-elfldr/prospero-elfldr>
        ../extern/compressor/fast-lzma2/fast-lzma2
)

embed_filesc(${PROJECT_NAME}
    ${CMAKE_BINARY_DIR}/patch-bundle/patch-bundle-${PLATFORM}.elf
)

include(optimization-settings)

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        $<$<BOOL:${ORBIS}>:orbis-elfldr>
        $<$<BOOL:${PROSPERO}>:prospero-elfldr>
        shared
        fast-lzma2
)

set(OUTPUT_ELF ${PROJECT_NAME}-${PLATFORM}.elf)
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${OUTPUT_ELF})

add_custom_target(${PROJECT_NAME}_test
        USES_TERMINAL
        COMMAND ${PROG_DEPLOY} ${OUTPUT_ELF}
        DEPENDS ${PROJECT_NAME}
)
