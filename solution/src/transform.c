#include "transform.h"

#include "image.h"
#include <errno.h>

struct image image_copy(const struct image input) {
    struct image output = image_create(input.width, input.height);
    if (output.data == NULL) {
        fprintf(stderr, "Error creating file.\n");
        return output;
    }

    for (size_t input_row = 0; input_row < input.height; input_row++) {
        for (size_t input_column = 0; input_column < input.width; input_column++) {
            *image_pos_at(&output, input_column, input_row) = *image_pos_at(&input, input_column, input_row);
        }
    }
    return output;
}

struct image image_rotate_90deg_left(const struct image input) {
    struct image output = image_create(input.height, input.width);
    if (output.data == NULL) {
        fprintf(stderr, "Error creating file.\n");
        return output;
    }

    for (size_t input_row = 0; input_row < input.height; input_row++) {
        for (size_t input_column = 0; input_column < input.width; input_column++) {
            *image_pos_at(&output, input.height - input_row - 1, input_column) = *image_pos_at(&input, input_column, input_row);
        }
    }
    return output;
}

struct image image_rotate_90deg_right(const struct image input) {
    struct image output = image_create(input.height, input.width);
    if (output.data == NULL) {
        fprintf(stderr, "Error creating file.\n");
        return output;
    }

    for (size_t input_row = 0; input_row < input.height; input_row++) {
        for (size_t input_column = 0; input_column < input.width; input_column++) {
            *image_pos_at(&output, input_row, output.height - input_column - 1) = *image_pos_at(&input, input_column, input_row); // Используем output.height
        }
    }
    return output;
}

struct image image_flip_horizontal(const struct image input) {
    struct image output = image_create(input.width, input.height);
    if (output.data == NULL) {
        fprintf(stderr, "Error creating file.\n");
        return output;
    }

    for (size_t input_row = 0; input_row < input.height; input_row++) {
        for (size_t input_column = 0; input_column < input.width; input_column++) {
            *image_pos_at(&output, input_column, input_row) = *image_pos_at(&input, input.width - input_column - 1, input_row);
            }
    }
    return output;
}

struct image image_flip_vertical(const struct image input) {
    struct image output = image_create(input.width, input.height);
    if (output.data == NULL) {
        fprintf(stderr, "Error creating file.\n");
        return output;
    }

    for (size_t input_row = 0; input_row < input.height; input_row++) {
        for (size_t input_column = 0; input_column < input.width; input_column++) {
            *image_pos_at(&output, input_column, input_row) = *image_pos_at(&input, input_column, input.height - input_row - 1);
        }
    }
    return output;
}