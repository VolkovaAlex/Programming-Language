#include "image.h"

#include <stdlib.h>
#include <errno.h>

struct image image_create(size_t width, size_t height) {
    struct image img = { 0 }; 
    img.width = width;
    img.height = height;

    img.data = malloc(width * height * sizeof(struct pixel));
    if (img.data == NULL) {
        fprintf(stderr, "Error allocating memory.\n");
        return img;
    }

    return img;
}

struct pixel* image_pos_at(const struct image* const img, size_t col, size_t row) {
    if (img->data == NULL) {
        return NULL;
    }
    return img->data + col + row * img->width;
}

void image_free(const struct image img) {
    free(img.data);
}

enum read_status read_image(const char* filename, from_image* from_image, struct image* img) {
    FILE* in = fopen(filename, "rb");
    if (in == NULL) {
        fprintf(stderr, "Error opening file.\n");
        return READ_INVALID_HEADER;
    }

    enum read_status from_image_status = from_image(in, img);
    fclose(in);

    return from_image_status;
}

enum write_status write_image(const char* filename, to_image* to_image, struct image img) {
    FILE* out = fopen(filename, "wb");
    if (out == NULL) {
        fprintf(stderr, "Error opening file.\n");
        return WRITE_ERROR;
    }

    enum write_status to_image_status = to_image(out, img);
    fclose(out);

    return to_image_status;
}