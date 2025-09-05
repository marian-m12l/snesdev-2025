#include<stdio.h>
#include<stdint.h>
#include<math.h>

#define MAX_SEPARATION (100 >> 2)

uint16_t distance(uint16_t w, uint16_t h) {
    return sqrt(w*w + h*h);
}

// Generates two binary files containing look-up tables (LUTs) with precomputed "camera offsets"
// for every vertical and horizontal distances between players (dx and dy).
// To allow for smaller LUTs, all distances are divided by 4 (so with a BG of 512*512 dimension, the LUT is 128*128).
// camera_lut_lo.bin contains values for dx from 0 to 63 (dy from 0 to 127)
// camera_lut_hi.bin contains values for dx from 64 to 127 (dy from 0 to 127)
// The 16-bit value for each combination of dx and dy represents the vertical and horizontal distance that separates
// both players' "cameras". When players are close enough (less than 100 pixels apart), they share the same
// background/camera (value is 0x0000), otherwise the backgrounds/cameras are displaced following the
// player-to-player direction (value follows the format 0xHHVV, with HH the horizontal displacement and VV the vertical
// displacement -- both displacements being in the 128*128 map dimension).
int main() {
    FILE *fp = fopen("lut.h", "w");
    FILE *fp_bin_lo = fopen("camera_lut_lo.bin", "wb");
    FILE *fp_bin_hi = fopen("camera_lut_hi.bin", "wb");

    fprintf(fp, "#pragma once\n\n");

    fprintf(fp, "const u16 camera_lut_lo[64][128] = {\n");
    for (uint16_t w=0; w<64; w++) {
        fprintf(fp, "    { ");
        for (uint16_t h=0; h<128; h++) {
            uint16_t d = distance(w, h);
            uint16_t value = 0x0000;
            if (d < MAX_SEPARATION) {
                fprintf(fp, "0x0000");
            } else {
                value = ((w * MAX_SEPARATION) / d) << 8 | ((h * MAX_SEPARATION) / d);
                fprintf(fp, "0x%04x", value);
            }
            if (h < 127) {
                fprintf(fp, ", ");
            }
            fwrite(&value, sizeof(value), 1, fp_bin_lo);
        }
        fprintf(fp, " }");
        if (w < 63) {
            fprintf(fp, ",");
        }
        fprintf(fp, "\n");
    }
    fprintf(fp, "};\n\n");

    fprintf(fp, "const u16 camera_lut_hi[64][128] = {\n");
    for (uint16_t w=64; w<128; w++) {
        fprintf(fp, "    { ");
        for (uint16_t h=0; h<128; h++) {
            uint16_t d = distance(w, h);
            uint16_t value = 0x0000;
            if (d < MAX_SEPARATION) {
                fprintf(fp, "0x0000");
            } else {
                value = ((w * MAX_SEPARATION) / d) << 8 | ((h * MAX_SEPARATION) / d);
                fprintf(fp, "0x%04x", value);
            }
            if (h < 127) {
                fprintf(fp, ", ");
            }
            fwrite(&value, sizeof(value), 1, fp_bin_hi);
        }
        fprintf(fp, " }");
        if (w < 127) {
            fprintf(fp, ",");
        }
        fprintf(fp, "\n");
    }
    fprintf(fp, "};\n");
    
    fclose(fp);
    fclose(fp_bin_lo);
    fclose(fp_bin_hi);
    printf("Done.\n");
    return 0;
}
