#!/bin/bash
set -e

compile_asm() {
    local clang="$1"
    local cpp_file="$2"
    local postfix="$3"
    local base_name="${cpp_file##*/}"
    local out_file="${base_name%.cpp}${postfix}.txt"
    local -a cflags=(
        "-fplt" # force plt so functions aren't read by pointer `call -> jmp qword ptr` instead of `call qword ptr`
        "-fno-jump-tables" # don't do it, clang-chan its gonna break our shellcode!
        "-fno-inline" # saves space on sid(s)
        "-fvisibility-nodllstorageclass=hidden" # don't know what it does, but global vars have @GOTPCREL as pointers, nyaaa
                                                # https://github.com/ps4-payload-dev/sdk/blob/14ffdcbc5bef2315ece33082be9a777ee163fd64/host/bin/orbis-clang%2B%2B#L71
                                                # https://github.com/ps5-payload-dev/sdk/blob/9e7b5f8ccae6f4bf684de32c5c425d9edbfaeea5/host/bin/prospero-clang%2B%2B#L77
        "-Wall"
        "-Werror"
        "-fms-extensions" # inline function information
        "-Wno-microsoft-string-literal-from-predefined" # above
    )

    "$clang" -S -mllvm --x86-asm-syntax=intel \
    "${cflags[@]}" $4 \
        "$cpp_file" -o - | awk '
        /^\s*\.(data|hidden|file|loc|cfi|type|size|ident|addrsig|attribute|section|bss|globl|text|intel_syntax|p2align|weak)/ { next }
        /^\s*#/ { next }
        { sub(/\s*#.*$/, "") }
        /^[[:space:]]*$/ { next }

        # Fixup static symbols
        /^\s*\.comm\s+/ {
            if (match($0, /\.comm[[:space:]]+([^,]+),([^,]+),([^,]+)/, m)) {
                label = m[1]
                total = m[2] + 0
                bpd = m[3] + 0

                # 1 = .byte, 2 = .word, 4 = .long, 8 = .quad
                if      (bpd == 1) dir = "byte"
                else if (bpd == 2) dir = "word"
                else if (bpd == 4) dir = "long"
                else if (bpd >= 8) dir = "quad"
                else               dir = "byte"
                count = int(total / bpd)
                remainder = total % bpd

                printf "%s:\n", label

                if (count > 0) {
                    printf "    .%s", dir
                    for (i = 1; i <= count; i++) {
                        printf " 0"
                        if (i < count) printf ","
                    }
                    printf "\n"
                }

                for (i = 0; i < remainder; i++) {
                    printf "    .byte 0\n"
                }
            }
            next
        }
        { print }
    ' > "$out_file"
}

o=$PWD
cd host

compile_asm $PS4_PAYLOAD_SDK/bin/orbis-clang++ shellcode/user_load_prx.cpp _ps4 -O2
compile_asm $PS4_PAYLOAD_SDK/bin/orbis-clang++ shellcode/shellcore.cpp _ps4 -O2
compile_asm $PS5_PAYLOAD_SDK/bin/prospero-clang++ shellcode/user_videoout_120hz.cpp _ps5 -O2

clang++ -std=c++17 shellcode_gen.cpp -o shellcode_gen.elf -lkeystone -lZydis && ./shellcode_gen.elf

cd $o
