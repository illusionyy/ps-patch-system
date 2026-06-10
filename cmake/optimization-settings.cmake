add_definitions(
        -Wall
        -Werror
        -fplt # call + jmp qword ptr
        -g # debug symbols
        -O0 # no optimization
        -fno-inline # no inline unless explicitly said so
        -fvisibility-nodllstorageclass=hidden # smaller code size for global memory reads
        -fms-extensions # for inline function information
        -Wno-microsoft-string-literal-from-predefined # for above
)
