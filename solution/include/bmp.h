#ifndef BMP_H
#define BMP_H

#include "file.h"
#include "image.h"

#include <stdint.h>
#include <stdio.h>

struct bmp_header;

from_image from_bmp;
to_image to_bmp;

#endif