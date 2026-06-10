cmake_minimum_required(VERSION 3.20)

project(ps-patch-system)

list(PREPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
include_directories(${CMAKE_SOURCE_DIR})

if(DEFINED ORBIS)
    add_subdirectory(extern/loader/orbis-elfldr)
elseif(DEFINED PROSPERO)
    add_subdirectory(extern/loader/prospero-elfldr)
endif()

add_subdirectory(extern/compressor/fast-lzma2)
add_subdirectory(extern/reader/pugixml)
add_subdirectory(shared)
add_subdirectory(patch)
add_subdirectory(patch-server)
add_subdirectory(patch-web)
add_subdirectory(patch-bundle)
add_subdirectory(patch-bundle-loader)
