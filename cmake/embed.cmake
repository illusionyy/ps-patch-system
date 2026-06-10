get_filename_component(_EMBED_DIR "${CMAKE_CURRENT_LIST_FILE}" DIRECTORY)

macro(_embed_script_path OUT_VAR)
    get_filename_component(${OUT_VAR}
        "${_EMBED_DIR}/embed_file.cmake" ABSOLUTE)
    if(NOT EXISTS "${${OUT_VAR}}")
        message(FATAL_ERROR "embed.cmake and embed_file.cmake must live in the same directory. OUT_VAR: '${${OUT_VAR}}'")
    endif()
endmacro()

function(_embed_ensure_tool)
    cmake_parse_arguments(ARG "" "PROPERTY_PREFIX;TARGET_NAME;EXECUTABLE;INCLUDE_DIR" "SOURCES" ${ARGN})

    set(_prop "${ARG_PROPERTY_PREFIX}_TARGET_DEFINED")
    get_property(_already GLOBAL PROPERTY "${_prop}")
    if(_already)
        return()
    endif()

    foreach(_src IN LISTS ARG_SOURCES)
        if(NOT EXISTS "${_src}")
            message(FATAL_ERROR "source file not found: '${_src}'")
        endif()
    endforeach()

    set(_inc_flag "")
    if(ARG_INCLUDE_DIR)
        set(_inc_flag "-I" "${ARG_INCLUDE_DIR}")
    endif()

    set_property(GLOBAL PROPERTY "${ARG_PROPERTY_PREFIX}_EXECUTABLE" "${ARG_EXECUTABLE}")

    add_custom_command(
        OUTPUT  "${ARG_EXECUTABLE}"
        COMMAND clang -Os -Wall -Werror
                ${_inc_flag}
                -o "${ARG_EXECUTABLE}"
                ${ARG_SOURCES}
        DEPENDS ${ARG_SOURCES}
        COMMENT "Building ${ARG_TARGET_NAME}"
        VERBATIM
    )

    add_custom_target("${ARG_TARGET_NAME}" DEPENDS "${ARG_EXECUTABLE}")
    set_property(GLOBAL PROPERTY "${_prop}" TRUE)
endfunction()

function(_embed_add_files TARGET TOOL_TARGET TOOL_EXECUTABLE_PROP)
    _embed_script_path(_EMBED_SCRIPT)
    get_property(BIN2C_EXECUTABLE GLOBAL PROPERTY "${TOOL_EXECUTABLE_PROP}")

    foreach(INPUT_FILE IN LISTS ARGN)
        get_filename_component(ABS_FILE "${INPUT_FILE}" ABSOLUTE)
        get_filename_component(FNAME    "${ABS_FILE}"   NAME)

        string(REGEX REPLACE "[^a-zA-Z0-9]" "_" SAFE_NAME "${FNAME}")

        set(BASE_DIR "${CMAKE_CURRENT_BINARY_DIR}/.data")
        set(CURR_DIR "${BASE_DIR}/generated")
        file(MAKE_DIRECTORY ${CURR_DIR})
        set(OUT_C "${CURR_DIR}/${SAFE_NAME}.inc.c")
        set(OUT_H "${CURR_DIR}/${SAFE_NAME}.inc.h")

        add_custom_command(
            OUTPUT "${OUT_C}" "${OUT_H}"
            COMMAND ${CMAKE_COMMAND}
                "-DINPUT_FILE=${ABS_FILE}"
                "-DOUT_C=${OUT_C}"
                "-DOUT_H=${OUT_H}"
                "-DSAFE_NAME=${SAFE_NAME}"
                "-DBIN2C_EXECUTABLE=${BIN2C_EXECUTABLE}"
                -P "${_EMBED_SCRIPT}"
            DEPENDS "${ABS_FILE}" "${TOOL_TARGET}"
            COMMENT "Embedding ${FNAME}"
            VERBATIM
        )

        # add this binary dir to include list
        include_directories(${BASE_DIR})
        target_sources(${TARGET} PRIVATE "${OUT_C}" "${OUT_H}")
    endforeach()
endfunction()

function(embed_files TARGET)
    if(NOT DEFINED BIN2C_SOURCE)
        get_filename_component(BIN2C_SOURCE
            "${CMAKE_SOURCE_DIR}/host/bin2c.c" ABSOLUTE)
    endif()
    if(NOT DEFINED BIN2C_EXECUTABLE)
        set(BIN2C_EXECUTABLE "${CMAKE_SOURCE_DIR}/host/bin2c.elf")
    endif()

    _embed_ensure_tool(
        PROPERTY_PREFIX _EMBED_BIN2C
        TARGET_NAME     bin2c_tool
        EXECUTABLE      "${BIN2C_EXECUTABLE}"
        SOURCES         "${BIN2C_SOURCE}"
    )
    _embed_add_files(${TARGET} bin2c_tool _EMBED_BIN2C_EXECUTABLE ${ARGN})
endfunction()

function(embed_filesc TARGET)
    set(_lzma_inc "${CMAKE_SOURCE_DIR}/extern/compressor/fast-lzma2/fast-lzma2")
    set(_lzma_sources
        "${CMAKE_SOURCE_DIR}/host/lzma.c"
        "${_lzma_inc}/dict_buffer.c"
        "${_lzma_inc}/fl2_common.c"
        "${_lzma_inc}/fl2_compress.c"
        "${_lzma_inc}/fl2_decompress.c"
        "${_lzma_inc}/fl2_pool.c"
        "${_lzma_inc}/fl2_threading.c"
        "${_lzma_inc}/lzma2_dec.c"
        "${_lzma_inc}/lzma2_enc.c"
        "${_lzma_inc}/radix_bitpack.c"
        "${_lzma_inc}/radix_mf.c"
        "${_lzma_inc}/radix_struct.c"
        "${_lzma_inc}/range_enc.c"
        "${_lzma_inc}/util.c"
        "${_lzma_inc}/xxhash.c"
    )
    set(_lzma_exe "${CMAKE_SOURCE_DIR}/host/bin2c_lzma.elf")

    _embed_ensure_tool(
        PROPERTY_PREFIX _EMBED_BIN2C_LZMA
        TARGET_NAME     bin2c_lzma_tool
        EXECUTABLE      "${_lzma_exe}"
        SOURCES         ${_lzma_sources}
        INCLUDE_DIR     "${_lzma_inc}"
    )

    _embed_add_files(${TARGET} bin2c_lzma_tool _EMBED_BIN2C_LZMA_EXECUTABLE ${ARGN})
endfunction()
