#include "bmp.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>


#define BMP_BF_TYPE  19778
#define BMP_BF_RESERVED  0
#define BMP_BO_OFF_BITS  54
#define BMP_BI_SIZE  40
#define BMP_FILE_HEADER_SIZE  14
#define BMP_BI_PLANES  1
#define BMP_BI_BIT_COUNT  24
#define BMP_BI_COMPRESSION  0
#define BMP_BI_X_PELS_PER_METER  2835
#define BMP_BI_Y_PELS_PER_METER  2835
#define BMP_BI_CLR_USED  0
#define BMP_BI_CLR_IMPORTANT  0

#define BMP_HEADER_SIZE  sizeof(struct bmp_header)

#pragma pack(push, 1)
struct bmp_header {
    uint16_t bfType;
    uint32_t bfileSize;
    uint32_t bfReserved;
    uint32_t bOffBits;
    uint32_t biSize;
    uint32_t biWidth;
    uint32_t biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    uint32_t biXPelsPerMeter;
    uint32_t biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
};
#pragma pack(pop)

static bool read_header(FILE* in, struct bmp_header* const header) {
    if (in == NULL) {
        fprintf(stderr, "Error opening file.\n");
        return false;
    }
    bool status = fread(header, BMP_HEADER_SIZE, 1, in);
    fseek(in, (long)0, SEEK_CUR);

    if (!status) return false;
    fseek(in, (long)header->bOffBits, SEEK_SET);
    return true;
}

static size_t get_padding(size_t row_size) {
    return ((row_size + 3) / 4) * 4 - row_size;
}

static bool read_data(FILE* in, const struct image* const img) {
    if (in == NULL) {
        fprintf(stderr, "Error opening file.\n");
        return false;
    }
    size_t byte_width = PIXEL_SIZE * (img->width);
    size_t padding = get_padding(byte_width);

    for (size_t i = 0; i < img->width * img->height; ++i) {
        size_t bytes_read = fread(&img->data[i], PIXEL_SIZE, 1, in);
        if (bytes_read != 1) {
            if (ferror(in)) {
                fprintf(stderr, "Error reading file.\n");
                return false;
            }
            else {
                return true;
            }
        }

        if (i % img->width == img->width - 1) {
            fseek(in, (long)padding, SEEK_CUR);
        }
    }

    return true;
}

enum read_status from_bmp(FILE* in, struct image* const img) {
    if (in == NULL) {
        fprintf(stderr, "Error opening file.\n");
        return READ_INVALID_HEADER;
    }
    struct bmp_header header;

    if (!read_header(in, &header)) return READ_INVALID_HEADER;

    *img = image_create(header.biWidth, header.biHeight);
    if (img->data == NULL) {
        fprintf(stderr, "Error allocating memory.\n");
        return READ_INVALID_BITS;
    }

    if (!read_data(in, img)) return READ_INVALID_BITS;

    return READ_OK;
}

static struct bmp_header create_header(const struct image* const img) {
    size_t row_size = PIXEL_SIZE * img->width;
    size_t padded_row_size = row_size + get_padding(row_size);
    size_t image_size = padded_row_size * img->height;

    return (struct bmp_header) {
        .bOffBits = BMP_BO_OFF_BITS,
            .bfReserved = BMP_BF_RESERVED,
            .bfileSize = (uint32_t)image_size + BMP_BO_OFF_BITS,
            .biBitCount = BMP_BI_BIT_COUNT,
            .biClrImportant = BMP_BI_CLR_IMPORTANT,
            .biClrUsed = BMP_BI_CLR_USED,
            .biCompression = BMP_BI_COMPRESSION,
            .biHeight = (uint32_t)img->height,
            .biPlanes = BMP_BI_PLANES,
            .biSize = BMP_HEADER_SIZE - BMP_FILE_HEADER_SIZE,
            .biSizeImage = (uint32_t)image_size,
            .biWidth = (uint32_t)img->width,
            .biXPelsPerMeter = BMP_BI_X_PELS_PER_METER,
            .biYPelsPerMeter = BMP_BI_Y_PELS_PER_METER,
            .bfType = BMP_BF_TYPE
    };
}

enum write_status to_bmp(FILE* out, const struct image img) {
    if (out == NULL) {
        fprintf(stderr, "Error opening file.\n");
        return WRITE_ERROR;
    }
    struct bmp_header header = create_header(&img);

    if (!fwrite(&header, BMP_HEADER_SIZE, 1, out)) return WRITE_ERROR;

    size_t padding = get_padding(PIXEL_SIZE * (img.width));
    for (size_t row_number = 0; row_number < img.height; ++row_number) {
        const struct pixel* current_row = img.data + row_number * img.width;
        size_t bytes_written = fwrite(current_row, PIXEL_SIZE, img.width, out);
        if (bytes_written != img.width) {
            fprintf(stderr, "Error writing file.\n");
            return WRITE_ERROR;
        }
        uint8_t ptr1[3] = { 0 };
        fwrite(&ptr1, 1, padding, out);
    }

    return WRITE_OK;
}