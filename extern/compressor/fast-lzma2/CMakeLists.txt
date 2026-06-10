cmake_path(GET CMAKE_CURRENT_SOURCE_DIR FILENAME PROJECT_NAME)
string(REPLACE " " "_" ProjectId ${PROJECT_NAME})
project(${PROJECT_NAME})

set(SOURCES
    fast-lzma2/dict_buffer.c
    fast-lzma2/fl2_common.c
    fast-lzma2/fl2_compress.c
    fast-lzma2/fl2_decompress.c
    fast-lzma2/fl2_pool.c
    fast-lzma2/fl2_threading.c
    fast-lzma2/lzma2_dec.c
    fast-lzma2/lzma2_enc.c
    fast-lzma2/radix_bitpack.c
    fast-lzma2/radix_mf.c
    fast-lzma2/radix_struct.c
    fast-lzma2/range_enc.c
    fast-lzma2/util.c
    fast-lzma2/xxhash.c
)

add_library(${PROJECT_NAME} STATIC ${SOURCES})

include(optimization-settings)
