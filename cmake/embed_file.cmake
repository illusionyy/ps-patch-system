foreach(REQUIRED_VAR IN ITEMS INPUT_FILE OUT_C OUT_H SAFE_NAME BIN2C_EXECUTABLE)
    if(NOT DEFINED "${REQUIRED_VAR}" OR "${${REQUIRED_VAR}}" STREQUAL "")
        message(FATAL_ERROR "Required variable '${REQUIRED_VAR}' is not set.\n"
                            "This script must be invoked by embed.cmake, not directly.")
    endif()
endforeach()

if(NOT EXISTS "${BIN2C_EXECUTABLE}")
    message(FATAL_ERROR "bin2c executable not found at '${BIN2C_EXECUTABLE}'.\n")
endif()

execute_process(
    COMMAND "${BIN2C_EXECUTABLE}" "${INPUT_FILE}"
    OUTPUT_VARIABLE _bin2c_output
    RESULT_VARIABLE _bin2c_result
    ERROR_VARIABLE  _bin2c_error
)

if(NOT _bin2c_result EQUAL 0)
    message(FATAL_ERROR "bin2c failed on '${INPUT_FILE}'.\n${_bin2c_error}")
endif()

string(FIND "${_bin2c_output}" "\n" _nl)
string(SUBSTRING "${_bin2c_output}" 0 ${_nl} NUM_BYTES)
math(EXPR _body_start "${_nl} + 1")
string(SUBSTRING "${_bin2c_output}" ${_body_start} -1 ARRAY_BODY)
string(STRIP "${ARRAY_BODY}" ARRAY_BODY)

file(WRITE "${OUT_C}"
"#include \"${SAFE_NAME}.inc.h\"
#include <shared/macro.h>

// clang-format off

const unsigned char ${SAFE_NAME}_data[${NUM_BYTES}] = ${ARRAY_BODY};
const char ${SAFE_NAME}_metadata[] = \"File \" FILE_LINE \" Generated from ${INPUT_FILE}\"  \" at \" DATE_TIME;
const char ${SAFE_NAME}_path[] = FILE_LINE \":\" \"${INPUT_FILE}\";

// clang-format on
")

file(WRITE "${OUT_H}"
"#pragma once

// clang-format off

#ifdef __cplusplus
extern \"C\" {
#endif

extern const unsigned char ${SAFE_NAME}_data[${NUM_BYTES}];
extern const char ${SAFE_NAME}_metadata[];
extern const char ${SAFE_NAME}_path[];

#ifdef __cplusplus
}
#endif

// clang-format on
")

message(STATUS "Embedded '${INPUT_FILE}' (${NUM_BYTES} bytes) -> ${OUT_C}, ${OUT_H}")
