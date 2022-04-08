// Program: BMP RGB-to-Grayscale Converter
// Author: Violeta DeSantis
// Date: 20 October 2021

#include "rgb_to_gray.h"

int main(int argc, char **argv) {
    if (argc != 2) { fprintf(stderr, "Usage: ./grey baboon.bmp\n"); return 1; }

    // Verify filename ends in ".bmp"
    char *src_filename = argv[1];
    if (check_ext(src_filename)) { fprintf(stderr, "Invalid file extension\n"); return 1; }
    
    // Construct filename_gs.bmp string
    char *dst_filename;
    if (!(dst_filename = build_dst_filename(src_filename))) { return 1; }

    // Open source image, and create destination image
    FILE *src, *dst;
    if ( !(src = fopen(src_filename, "rb")) ) { fprintf(stderr, "Unable to open file: %s\n",   src_filename); goto CLEANUP_1; }
    if ( !(dst = fopen(dst_filename, "wb")) ) { fprintf(stderr, "Unable to create file: %s\n", dst_filename); goto CLEANUP_2; }

    // Read file header from src, write to dst
    BitmapFileHeader file_header;
    if (read_file_header(src, &file_header))  { fprintf(stderr, "Error reading file header\n"); goto CLEANUP_3; }
    if (write_file_header(dst, &file_header)) { fprintf(stderr, "Error writing file header\n"); goto CLEANUP_3; }

    // Read info header from src, write to dst
    BitmapInfoHeader info_header;
    if (read_info_header(src, &info_header))  { fprintf(stderr, "Error reading info header\n"); goto CLEANUP_3; }
    if (write_info_header(dst, &info_header)) { fprintf(stderr, "Error writing info header\n"); goto CLEANUP_3; }

    // Write any additional bit masks / color tables / Gap1 from src to dst
    unsigned char byte[1];
    while (ftell(src) != file_header.px_arr_offset) {
        if (!fread(byte, 1, 1, src))  { fprintf(stderr, "Error reading bit masks / color tables / Gap1\n"); goto CLEANUP_3; }
        if (!fwrite(byte, 1, 1, dst)) { fprintf(stderr, "Error writing bit masks / color tables / Gap1\n"); goto CLEANUP_3; }
    }

    // RGB-to-Grayscale conversion
    int bytes_per_row = ((info_header.color_depth*info_header.width+31)/32)*4;
    unsigned char src_pixel[3], dst_pixel[3];
    
    for (int i=0; i<info_header.height; ++i) {
        int j;
        for (j=0; j<info_header.width; ++j) {
            if (!fread(src_pixel, 3, 1, src))  { fprintf(stderr, "Error reading pixel array\n"); goto CLEANUP_3; }  // read RGB pixel
            dst_pixel[0] = dst_pixel[1] = dst_pixel[2] = (src_pixel[0] + src_pixel[1] + src_pixel[2]) / 3;          // average RGB 
            if (!fwrite(dst_pixel, 3, 1, dst)) { fprintf(stderr, "Error writing pixel array\n"); goto CLEANUP_3; }  // write grayscale pixel
        }
        // Handle end-of-row padding so that each row is a multiple of 4 bytes
        if (bytes_per_row - j*3 > 0) {
            if (!fread(src_pixel, bytes_per_row - j*3, 1, src))  { fprintf(stderr, "Error reading row padding\n"); goto CLEANUP_3; }
            if (!fwrite(src_pixel, bytes_per_row - j*3, 1, dst)) { fprintf(stderr, "Error writing row padding\n"); goto CLEANUP_3; }
        }
    }

    // Write any remaining Gap2 / ICC color profile from src to dst
    while (fread(byte, 1, 1, src)) { 
        if (!fwrite(byte, 1, 1, dst)) { fprintf(stderr, "Error writing Gap2 / ICC color profile\n"); goto CLEANUP_3; }
    }
    
    fclose(dst);
    fclose(src);
    free(dst_filename);
    return 0;

CLEANUP_3:
    fclose(dst);
CLEANUP_2:
    fclose(src);
CLEANUP_1:
    free(dst_filename);
    return 1;
}
