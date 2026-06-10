#include <stdio.h>
#include <stdlib.h>

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
    const long size = ftell(f);
    rewind(f);

    printf("%ld\n", size);
    printf("{");

    int col = 0;
    int byte;
    int first = 1;
    while ((byte = fgetc(f)) != EOF)
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
        printf("0x%02x", (unsigned char)byte);
        col = (col + 1) % BYTES_PER_ROW;
    }

    printf("\n}");

    fclose(f);
    return 0;
}
