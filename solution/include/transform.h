#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "image.h"

struct image image_copy(const struct image input);
struct image image_rotate_90deg_left(const struct image input);
struct image image_rotate_90deg_right(const struct image input);
struct image image_flip_horizontal(const struct image input);
struct image image_flip_vertical(const struct image input);

#endif