#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h> 

#include "bmp.h"
#include "file.h"
#include "image.h"
#include "transform.h"

int main(int argc, char** argv) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <input-image-location> <output-image-location> <transformation>\n", argv[0]);
        return 1;
    }

    char* input_image_location = argv[1];
    char* output_image_location = argv[2];
    char* transformation = argv[3];

    struct image img;
    FILE* in = fopen(input_image_location, "rb");
    if (in == NULL) {
        fprintf(stderr, "Error opening file.\n");
        return ENOENT;
    }
    fclose(in);

    enum read_status read_status = read_image(input_image_location, from_bmp, &img);
    if (read_status != READ_OK) {
        if (read_status == READ_INVALID_HEADER) {
            fprintf(stderr, "Error reading image header.\n");
            return ENOMEM;
        }
        else if (read_status == READ_INVALID_BITS) {
            fprintf(stderr, "Error reading image data.\n");
            return ENOMEM;
        }
    }

    struct image transformed_image = image_create(img.width, img.height);
    if (transformed_image.data == NULL) {
        fprintf(stderr, "Error allocating memory.\n");
        return ENOMEM;
    }

    if (strcmp(transformation, "cw90") == 0) {
        transformed_image = image_rotate_90deg_right(img);
    }
    else if (strcmp(transformation, "ccw90") == 0) {
        transformed_image = image_rotate_90deg_left(img);
    }
    else if (strcmp(transformation, "fliph") == 0) {
        transformed_image = image_flip_horizontal(img);
    }
    else if (strcmp(transformation, "flipv") == 0) {
        transformed_image = image_flip_vertical(img);
    }
    else if (strcmp(transformation, "none") == 0) {
        transformed_image = image_copy(img);
    }

    if (transformed_image.data == NULL) {
        fprintf(stderr, "Error allocating memory.\n");
        return ENOMEM;
    }

    enum write_status write_status = write_image(output_image_location, to_bmp, transformed_image);
    if (write_status != WRITE_OK) {
        fprintf(stderr, "Error saving image.\n");
        return 1;
    }

    image_free(img);
    image_free(transformed_image);

    return 0;
}