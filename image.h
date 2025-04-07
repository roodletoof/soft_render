#ifndef IMAGE_H
#define IMAGE_H

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

typedef uint8_t color_t;

#define COLOR(R, G, B) \
    ((color_t) ((((((R) * 7) + 127) / 255) << 5) | \
               (((((G) * 7) + 127) / 255) << 2) | \
               (((((B) * 3) + 127) / 255) << 0)))

#define CHANNEL_R(c) (((c) & 0b11100000) >> 5) * 255 / 7
#define CHANNEL_G(c) (((c) & 0b00011100) >> 2) * 255 / 7
#define CHANNEL_B(c) (((c) & 0b00000011) >> 0) * 255 / 3

#define PICO_0 COLOR(0X00, 0X00, 0X00)
#define PICO_1 COLOR(0X1D, 0X2B, 0X53)
#define PICO_2 COLOR(0X7E, 0X25, 0X53)
#define PICO_3 COLOR(0X00, 0X87, 0X51)
#define PICO_4 COLOR(0XAB, 0X52, 0X36)
#define PICO_5 COLOR(0X5F, 0X57, 0X4F)
#define PICO_6 COLOR(0XC2, 0XC3, 0XC7)
#define PICO_7 COLOR(0XFF, 0XF1, 0XE8)
#define PICO_8 COLOR(0XFF, 0X00, 0X4D)
#define PICO_9 COLOR(0XFF, 0XA3, 0X00)
#define PICO_10 COLOR(0XFF, 0XEC, 0X27)
#define PICO_11 COLOR(0X00, 0XE4, 0X36)
#define PICO_12 COLOR(0X29, 0XAD, 0XFF)
#define PICO_13 COLOR(0X83, 0X76, 0X9C)
#define PICO_14 COLOR(0XFF, 0X77, 0XA8)
#define PICO_15 COLOR(0XFF, 0XCC, 0XAA)

typedef struct image {
    uint32_t width, height;
    color_t *pixels;
} image_t;

#define IMAGE_VALUE(WIDTH, HEIGHT) ((image_t) {WIDTH, HEIGHT, ((color_t[WIDTH * HEIGHT]) {0})})

bool image_save(image_t *this, FILE *file);
image_t *image_load(FILE *file);
bool image_save_path(image_t *this, const char *path);
image_t *image_load_path(const char *path);
void image_draw_circle_lines(image_t *this, int x, int y, int radius, color_t color);
void image_draw_circle_filled(image_t *this, int x, int y, int radius, color_t color);
void image_draw_rect_lines(image_t *this, int x, int y, int width, int height, color_t color);
void image_draw_rect_filled(image_t *this, int x, int y, int width, int height, color_t color);
void image_draw_line(image_t *this, int x1, int y1, int x2, int y2, color_t color);
void image_draw_image(image_t *this, int x, int y, image_t *image);
void image_draw_image_rect(image_t *this, int x, int y, image_t *image, int sx, int sy, int sw, int sh);
void image_fill(image_t *this, color_t color);

// statically test the color conversion macros
_Static_assert(COLOR(255, 0, 0) == 0b11100000, "COLOR(255, 0, 0) == 0b11100000");
_Static_assert(COLOR(0, 255, 0) == 0b00011100, "COLOR(0, 255, 0) == 0b00011100");
_Static_assert(COLOR(0, 0, 255) == 0b00000011, "COLOR(0, 0, 255) == 0b00000011");
_Static_assert(COLOR(255, 255, 0) == 0b11111100, "COLOR(255, 255, 0) == 0b11111100");
_Static_assert(COLOR(255, 255, 255) == 0b11111111, "COLOR(255, 255, 255) == 0b11111111");
_Static_assert(COLOR(0, 0, 0) == 0b00000000, "COLOR(0, 0, 0) == 0b00000000");
_Static_assert(COLOR(255, 127, 0) == 0b11101100, "COLOR(255, 127, 0) == 0b11101100");
_Static_assert(CHANNEL_R(COLOR(255, 0, 0)) == 255, "CHANNEL_R(COLOR(255, 0, 0)) == 255");
_Static_assert(CHANNEL_G(COLOR(255, 0, 0)) == 0, "CHANNEL_G(COLOR(255, 0, 0)) == 0");
_Static_assert(CHANNEL_B(COLOR(255, 0, 0)) == 0, "CHANNEL_B(COLOR(255, 0, 0)) == 0");
_Static_assert(CHANNEL_R(COLOR(0, 255, 0)) == 0, "CHANNEL_R(COLOR(0, 255, 0)) == 0");
_Static_assert(CHANNEL_G(COLOR(0, 255, 0)) == 255, "CHANNEL_G(COLOR(0, 255, 0)) == 255");
_Static_assert(CHANNEL_B(COLOR(0, 255, 0)) == 0, "CHANNEL_B(COLOR(0, 255, 0)) == 0");
_Static_assert(CHANNEL_R(COLOR(0, 0, 255)) == 0, "CHANNEL_R(COLOR(0, 0, 255)) == 0");
_Static_assert(CHANNEL_G(COLOR(0, 0, 255)) == 0, "CHANNEL_G(COLOR(0, 0, 255)) == 0");
_Static_assert(CHANNEL_B(COLOR(0, 0, 255)) == 255, "CHANNEL_B(COLOR(0, 0, 255)) == 255");

#endif /* IMAGE_H */
