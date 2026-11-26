#include "mpeg2_crc32.h"
#include <stdint.h>
#include <stdio.h>

int main(int argc, char **argv) {

    if (argc != 2) {
    printf("Usage: %s input_file.bin \n", argv[1]);
    return 1;
    }

    // define 256 byte image
    uint8_t image[256] = {0};

    // open input binary in read byte mode
    FILE *input = fopen(argv[1], "rb");
    if (!input) {
        perror("ERROR: fopen for input file");
    return 1;
    }

    // copy input binary bytes (up to 252 bytes) into image array
    size_t n = fread(image, 1, 252, input);
    fclose(input);
    if (n > 252) {
    printf("Input file too large (max 252 bytes)");
    return 1;
    }

    // calculate CRC
    uint32_t crc = mpeg2_crc32(image, 252);

    printf("CRC=0x%08X\n", crc);
    return 0;

}
