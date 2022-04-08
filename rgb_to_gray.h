#ifndef _RGB_TO_GRAY_H_
#define _RGB_TO_GRAY_H_

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define u16 uint16_t
#define u32 uint32_t
#define s32 int32_t

typedef struct BitmapFileHeader {
    u16 magic_number;
    u32 file_size;
    u16 reserved0;
    u16 reserved1;
    u32 px_arr_offset;
} BitmapFileHeader;

typedef struct BitmapInfoHeader { // Windows BITMAPINFOHEADER
    u32 header_size;
    s32 width;
    s32 height;
    u16 n_color_planes;
    u16 color_depth;
    u32 compression;
    u32 bitmap_size;
    s32 horz_res;
    s32 vert_res;
    u32 n_colors;
    u32 n_imp_colors;
} BitmapInfoHeader;

/* returns 0 if filename ends in .bmp */
int check_ext(char *filename) {
    int len = strlen(filename);
    if (filename[len-4]=='.' && filename[len-3]=='b' && filename[len-2]=='m' && filename[len-1]=='p') {
        return 0;
    }
    return 1;
}

/* mallocs and returns a string in the form filename_gs.bmp */
char *build_dst_filename(char *filename) {
    char *filename_gs;
    if (!(filename_gs = malloc(strlen(filename)+3+1))) { // baboon.bmp + _gs + \0
        fprintf(stderr, "malloc() failed\n");
        return NULL;
    }                         
    memcpy(filename_gs, filename, strlen(filename)-4);  // baboon
    memcpy(filename_gs+strlen(filename)-4, "_gs",  3);  // baboon_gs
    memcpy(filename_gs+strlen(filename)-1, ".bmp", 4);  // baboon_gs.bmp
    filename_gs[strlen(filename)+3] = '\0';             // baboon_gs.bmp\0

    return filename_gs;
}

/* reads file header and checks the magic number - returns 1 in case of failure */
int read_file_header(FILE *fp, BitmapFileHeader *header) {
    if (!fread(&header->magic_number,  2, 1, fp)) { return 1; }
    if (header->magic_number != 0x4D42) {
        fprintf(stderr, "Invalid file signature for a BMP file.\n");
        return 1;
    }

    if (!fread(&header->file_size,     4, 1, fp)) { return 1; }
    if (!fread(&header->reserved0,     2, 1, fp)) { return 1; }
    if (!fread(&header->reserved1,     2, 1, fp)) { return 1; }
    if (!fread(&header->px_arr_offset, 4, 1, fp)) { return 1; }

    return 0;
}

/* writes file header to destination file - returns 1 in case of failure */
int write_file_header(FILE *fp_gs, BitmapFileHeader *header) {
    if (!fwrite(&header->magic_number,  2, 1, fp_gs)) { return 1; }
    if (!fwrite(&header->file_size,     4, 1, fp_gs)) { return 1; }
    if (!fwrite(&header->reserved0,     2, 1, fp_gs)) { return 1; }
    if (!fwrite(&header->reserved1,     2, 1, fp_gs)) { return 1; }
    if (!fwrite(&header->px_arr_offset, 4, 1, fp_gs)) { return 1; }

    return 0;
}

/* reads info header and checks header type, color depth and compression - returns 1 in case of failure */
int read_info_header(FILE *fp, BitmapInfoHeader *header) {
    if (!fread(&header->header_size,    4, 1, fp)) { return 1; }
    if (header->header_size != 40) { 
        fprintf(stderr, "File does not have header type: BITMAPINFOHEADER\n");
        return 1;
    }

    if (!fread(&header->width,          4, 1, fp)) { return 1; }
    if (!fread(&header->height,         4, 1, fp)) { return 1; }
    if (!fread(&header->n_color_planes, 2, 1, fp)) { return 1; }
    if (!fread(&header->color_depth,    2, 1, fp)) { return 1; }
    if (header->color_depth != 24) {
        fprintf(stderr, "Support for color depths other than 24-bit not implemented\n");
        return 1;
    }

    if (!fread(&header->compression,    4, 1, fp)) { return 1; }
    if (header->compression != 0) {
        fprintf(stderr, "Support for compressed files not implemented\n");
        return 1;
    }

    if (!fread(&header->bitmap_size,    4, 1, fp)) { return 1; }
    if (!fread(&header->horz_res,       4, 1, fp)) { return 1; }
    if (!fread(&header->vert_res,       4, 1, fp)) { return 1; }
    if (!fread(&header->n_colors,       4, 1, fp)) { return 1; }
    if (!fread(&header->n_imp_colors,   4, 1, fp)) { return 1; }

    return 0;
}

/* writes info header to destination file - returns 1 in case of failure */
int write_info_header(FILE *fp_gs, BitmapInfoHeader *header) {
    if (!fwrite(&header->header_size,    4, 1, fp_gs)) { return 1; }
    if (!fwrite(&header->width,          4, 1, fp_gs)) { return 1; }
    if (!fwrite(&header->height,         4, 1, fp_gs)) { return 1; }
    if (!fwrite(&header->n_color_planes, 2, 1, fp_gs)) { return 1; }
    if (!fwrite(&header->color_depth,    2, 1, fp_gs)) { return 1; }
    if (!fwrite(&header->compression,    4, 1, fp_gs)) { return 1; }
    if (!fwrite(&header->bitmap_size,    4, 1, fp_gs)) { return 1; }
    if (!fwrite(&header->horz_res,       4, 1, fp_gs)) { return 1; }
    if (!fwrite(&header->vert_res,       4, 1, fp_gs)) { return 1; }
    if (!fwrite(&header->n_colors,       4, 1, fp_gs)) { return 1; }
    if (!fwrite(&header->n_imp_colors,   4, 1, fp_gs)) { return 1; }

    return 0;
}

#endif