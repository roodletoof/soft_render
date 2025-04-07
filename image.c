#include "image.h"

#include <stdio.h>
#include <stdint.h>
#include <math.h>

void store_uint32(uint32_t value, FILE *file) {
    // Store a 32-bit unsigned integer in little-endian format
    fputc(value & 0xFF, file);
    fputc((value >> 8) & 0xFF, file);
    fputc((value >> 16) & 0xFF, file);
    fputc((value >> 24) & 0xFF, file);
}
void load_uint32(uint32_t *value, FILE *file) {
    // Load a 32-bit unsigned integer from little-endian format
    *value = fgetc(file);
    *value |= fgetc(file) << 8;
    *value |= fgetc(file) << 16;
    *value |= fgetc(file) << 24;
}

// image_save saves the image to a file using a custom format.
// Uses run-length encoding to compress the image data.
bool image_save(image_t *this, FILE *file) {
    store_uint32(this->width, file);
    store_uint32(this->height, file);

    uint32_t size = this->width * this->height;

    for (uint32_t i = 0; i < size; i++) {
        color_t color = this->pixels[i];
        int count = 1;
        while (i + 1 < size && this->pixels[i + 1] == color && count < 255) {
            count++;
            i++;
        }
        fputc(color, file);
        fputc((uint8_t) count, file);
    }
    return true;
}

image_t *image_load(FILE *file) {
    uint32_t width, height;
    load_uint32(&width, file);
    load_uint32(&height, file);
    uint32_t size = width * height;
    // allocate all the memory in one go
    image_t *image = malloc(sizeof(image_t) + size * sizeof(color_t));
    if (!image) { return NULL; }
    image->width = width;
    image->height = height;
    image->pixels = (color_t*) (image+1);
    for (uint32_t i = 0; i < size; i++) {
        color_t color = fgetc(file);
        uint8_t count = fgetc(file);
        for (uint8_t j = 0; j < count; j++) {
            image->pixels[i + j] = color;
        }
        i += count - 1;
    }
    return image;
}

bool image_save_path(image_t *this, const char *path) {
    FILE *file = fopen(path, "wb");
    if (!file) {
        return false;
    }
    bool result = image_save(this, file);
    fclose(file);
    return result;
}

image_t *image_load_path(const char *path) {
    FILE *file = fopen(path, "rb");
    if (!file) {
        return NULL;
    }
    image_t *image = image_load(file);
    fclose(file);
    return image;
}

void image_draw_circle_lines(image_t *this, int x, int y, int radius, color_t color) {
    int x0 = 0;
    int y0 = radius;
    int d = 1 - radius;

    while (x0 <= y0) {
        this->pixels[(y + y0) * this->width + (x + x0)] = color;
        this->pixels[(y + y0) * this->width + (x - x0)] = color;
        this->pixels[(y - y0) * this->width + (x + x0)] = color;
        this->pixels[(y - y0) * this->width + (x - x0)] = color;
        this->pixels[(y + x0) * this->width + (x + y0)] = color;
        this->pixels[(y + x0) * this->width + (x - y0)] = color;
        this->pixels[(y - x0) * this->width + (x + y0)] = color;
        this->pixels[(y - x0) * this->width + (x - y0)] = color;

        if (d < 0) {
            d += 2 * x0 + 3;
        } else {
            d += 2 * (x0 - y0) + 5;
            y0--;
        }
        x0++;
    }
}

// use integer math to draw a filled circle
void image_draw_circle_filled(image_t *this, int x, int y, int radius, color_t color) {
    int x0 = 0;
    int y0 = radius;
    int d = 1 - radius;

    while (x0 <= y0) {
        for (int i = -x0; i <= x0; i++) {
            this->pixels[(y + y0) * this->width + (x + i)] = color;
            this->pixels[(y - y0) * this->width + (x + i)] = color;
        }
        for (int i = -y0; i <= y0; i++) {
            this->pixels[(y + x0) * this->width + (x + i)] = color;
            this->pixels[(y - x0) * this->width + (x + i)] = color;
        }

        if (d < 0) {
            d += 2 * x0 + 3;
        } else {
            d += 2 * (x0 - y0) + 5;
            y0--;
        }
        x0++;
    }
}

void image_draw_image(image_t *this, int x, int y, image_t *image) {
    for (uint32_t i = 0; i < image->width; i++) {
        for (uint32_t j = 0; j < image->height; j++) {
            int pixel_x = x + i;
            int pixel_y = y + j;
            if (pixel_x >= 0 && pixel_x < this->width && pixel_y >= 0 && pixel_y < this->height) {
                this->pixels[pixel_y * this->width + pixel_x] = image->pixels[j * image->width + i];
            }
        }
    }
}
void image_draw_image_rect( image_t *this, int x, int y, image_t *image, int sx, int sy, int sw, int sh) {
    for (int i = 0; i < sw; i++) {
        for (int j = 0; j < sh; j++) {
            int pixel_x = x + i;
            int pixel_y = y + j;
            if (pixel_x >= 0 && pixel_x < this->width && pixel_y >= 0 && pixel_y < this->height) {
                this->pixels[pixel_y * this->width + pixel_x] = image->pixels[(sy + j) * image->width + (sx + i)];
            }
        }
    }
}

void image_fill(image_t *this, color_t color) {
    for (uint32_t i = 0; i < this->width * this->height; i++) {
        this->pixels[i] = color;
    }
}
