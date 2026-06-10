#include <stdio.h>
#include <stdlib.h>
#include <fast-lzma2.h>

#define BYTES_PER_ROW 8

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: bin2c <input_file>\n");
        return 1;
    }

    FILE* f = fopen(argv[1], "rb");
    if (!f)
    {
        fprintf(stderr, "Error: cannot open '%s'\n", argv[1]);
        return 1;
    }

    fseek(f, 0, SEEK_END);
    const long src_size = ftell(f);
    rewind(f);

    unsigned char* src = (unsigned char*)malloc(src_size);
    if (!src)
    {
        fprintf(stderr, "Error: out of memory\n");
        fclose(f);
        return 1;
    }

    if ((long)fread(src, 1, src_size, f) != src_size)
    {
        fprintf(stderr, "Error: read failed\n");
        free(src);
        fclose(f);
        return 1;
    }
    fclose(f);

    const size_t dst_capacity = FL2_compressBound(src_size);
    unsigned char* dst = (unsigned char*)malloc(dst_capacity);
    if (!dst)
    {
        fprintf(stderr, "Error: out of memory for compression buffer\n");
        free(src);
        return 1;
    }

    const int compressionLevel = 9;
    const size_t compressed_size = FL2_compress(dst, dst_capacity, src, src_size, compressionLevel);
    free(src);

    if (FL2_isError(compressed_size))
    {
        fprintf(stderr, "Error: compression failed: %s\n", FL2_getErrorName(compressed_size));
        free(dst);
        return 1;
    }

    printf("%lu\n", compressed_size);
    printf("{");

    int col = 0;
    int first = 1;
    for (size_t i = 0; i < compressed_size; i++)
    {
        if (first)
        {
            printf("\n  ");
            first = 0;
        }
        else if (col == 0)
        {
            printf(",\n  ");
        }
        else
        {
            printf(", ");
        }
        printf("0x%02x", dst[i]);
        col = (col + 1) % BYTES_PER_ROW;
    }

    printf("\n}");

    free(dst);
    return 0;
}
