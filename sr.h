#ifndef SR_HEADER
#define SR_HEADER

typedef struct sr_surface {
    const int width;
    const int height;
    unsigned char invisible;
    unsigned char * const pixels;
} sr_surface;

void sr_pal(unsigned char from, unsigned char to);
void sr_pal_reset();

unsigned char sr_get_pixel(
    sr_surface *self,
    int x,
    int y
);

void sr_pixel(
    sr_surface *self,
    unsigned char color,
    int x,
    int y
);
void sr_line(
    sr_surface *self,
    unsigned char color,
    int x0,
    int y0,
    int x1,
    int y1
);

void sr_rect_fill(
    sr_surface *self,
    unsigned char color,
    int x,
    int y,
    int width,
    int height
);
void sr_rect_outline(
    sr_surface *self,
    unsigned char color,
    int x,
    int y,
    int width,
    int height
);

void sr_circle_fill(
    sr_surface *self,
    unsigned char color,
    int x,
    int y,
    int radius
);
void sr_circle_outline(
    sr_surface *self,
    unsigned char color,
    int x,
    int y,
    int radius
);

void sr_blit(
    sr_surface *destination,
    int dest_x,
    int dest_y,
    sr_surface *source
);
void sr_blit_pro(
    sr_surface *destination,
    int dest_x,
    int dest_y,
    int dest_width,
    int dest_height,
    sr_surface *source,
    int source_x,
    int source_y,
    int source_width,
    int source_height
);

void sr_fill(
    sr_surface *self,
    unsigned char color
);

#define sr_surface_value(WIDTH, HEIGHT) \
    (sr_surface) { \
        .width = WIDTH, \
        .height = HEIGHT, \
        .pixels = (unsigned char[WIDTH*HEIGHT]) {}, \
        .invisible = 0 \
    }

#ifdef SR_IMPLEMENTATION

static unsigned char color_map[256] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
    16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
    32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,
    48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63,
    64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
    80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95,
    96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111,
    112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127,
    128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143,
    144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159,
    160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175,
    176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191,
    192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207,
    208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223,
    224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239,
    240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255,
};

static inline void clamp_range(
        sr_surface *self,
        int start_x,
        int start_y,
        int stop_x,
        int stop_y,
        int *start_x_out,
        int *start_y_out,
        int *stop_x_out,
        int *stop_y_out
) {
    if (start_x < 0) {
        *start_x_out = 0;
    } else {
        *start_x_out = start_x;
    }
    if (start_y < 0) {
        *start_y_out = 0;
    } else {
        *start_y_out = start_y;
    }
    if (stop_x > self->width) {
        *stop_x_out = self->width;
    } else {
        *stop_x_out = stop_x;
    }
    if (stop_y > self->height) {
        *stop_y_out = self->height;
    } else {
        *stop_y_out = stop_y;
    }
}

unsigned char sr_get_pixel(
    sr_surface *self,
    int x,
    int y
) {
    if (
        0 <= x && x < self->width &&
        0 <= y && y < self->height
    ) {
        return self->pixels[y * self->width + x];
    }
    return self->invisible;
}

void sr_pixel(
    sr_surface *self,
    unsigned char color,
    int x,
    int y
) {
    if (
        0 <= x && x < self->width &&
        0 <= y && y < self->height
    ) {
        self->pixels[y * self->width + x] = color_map[color];
    }
}

static int abs(int i) {
    return i < 0 ? -i : i;
}

void sr_line(
    sr_surface *self,
    unsigned char color,
    int x0,
    int y0,
    int x1,
    int y1
) {
    // https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm

    int dx = abs(x1 - x0);
    int sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0);
    int sy = y0 < y1 ? 1 : -1;
    int error = dx + dy;

    while (1) {
        sr_pixel( self, color, x0, y0 );
        if ( x0 == x1 && y0 == y1 ) {
            break;
        }
        int e2 = 2* error;
        if ( e2 >= dy ) {
            if ( x0 == x1 ) {
                break;
            }
            error += dy;
            x0 += sx;
        }
        if ( e2 <= dx ) {
            if ( y0 == y1 ) {
                break;
            }
            error += dx;
            y0 += sy;
        }
    }
}

void sr_rect_fill(
    sr_surface *self,
    unsigned char color,
    int x,
    int y,
    int width,
    int height
) {
    int start_x, start_y, stop_x, stop_y;
    clamp_range(self, x, y, x+width, y+height, &start_x, &start_y, &stop_x, &stop_y);
    for (int curr_y = start_y; curr_y < stop_y; curr_y++ ) {
        for (int curr_x = start_x; curr_x < stop_x; curr_x++ ) {
            self->pixels[curr_y * self->width + curr_x] = color_map[color];
        }
    }
}
void sr_rect_outline(
    sr_surface *self,
    unsigned char color,
    int x,
    int y,
    int width,
    int height
) {
    for ( int x2 = x; x2 < x + width; x2++ ) {
        sr_pixel( self, color, x2, y );
        sr_pixel( self, color, x2, y + height - 1 );
    }
    for ( int y2 = y+1; y2 < y + height - 1; y2++ ) {
        sr_pixel( self, color, x, y2 );
        sr_pixel( self, color, x + width - 1, y2 );
    }
}

static void circle_line(sr_surface *self, unsigned char color, int center_x, int center_y, int x, int y) {
    sr_line(self, color, center_x+x, center_y+y, center_x+x, center_y-y);
    sr_line(self, color, center_x-x, center_y+y, center_x-x, center_y-y);
    sr_line(self, color, center_x+y, center_y+x, center_x+y, center_y-x);
    sr_line(self, color, center_x-y, center_y+x, center_x-y, center_y-x);
}

void sr_circle_fill(
    sr_surface *self,
    unsigned char color,
    int x,
    int y,
    int radius
) {
    // https://www.geeksforgeeks.org/bresenhams-circle-drawing-algorithm/
    int outer_x = 0;
    int outer_y = radius;
    int d = 3 - 2 * radius;
    circle_line(self, color, x, y, outer_x, outer_y);
    while ( outer_y >= outer_x ) {
        outer_x++;
        if ( d > 0 ) {
            outer_y--;
            d = d + 4 * (outer_x - outer_y) + 10;
        } else {
            d = d + 4 * outer_x + 6;
        }
        circle_line(self, color, x, y, outer_x, outer_y);
    }
}

static void circle_pixel(sr_surface *self, unsigned char color, int center_x, int center_y, int x, int y) {
    sr_pixel(self, color, center_x+x, center_y+y);
    sr_pixel(self, color, center_x-x, center_y+y);
    sr_pixel(self, color, center_x+x, center_y-y);
    sr_pixel(self, color, center_x-x, center_y-y);
    sr_pixel(self, color, center_x+y, center_y+x);
    sr_pixel(self, color, center_x-y, center_y+x);
    sr_pixel(self, color, center_x+y, center_y-x);
    sr_pixel(self, color, center_x-y, center_y-x);
}

void sr_circle_outline(
    sr_surface *self,
    unsigned char color,
    int x,
    int y,
    int radius
) {
    // https://www.geeksforgeeks.org/bresenhams-circle-drawing-algorithm/
    int outer_x = 0;
    int outer_y = radius;
    int d = 3 - 2 * radius;
    circle_pixel(self, color, x, y, outer_x, outer_y);
    while ( outer_y >= outer_x ) {
        outer_x++;
        if ( d > 0 ) {
            outer_y--;
            d = d + 4 * (outer_x - outer_y) + 10;
        } else {
            d = d + 4 * outer_x + 6;
        }
        circle_pixel(self, color, x, y, outer_x, outer_y);
    }
}

void sr_blit(
    sr_surface *destination,
    int dest_x,
    int dest_y,
    sr_surface *source
) {
    int start_x, start_y, stop_x, stop_y;
    clamp_range(destination, dest_x, dest_y, dest_x+source->width, dest_y+source->height, &start_x, &start_y, &stop_x, &stop_y);
    for ( int y = start_y; y < stop_y; y++ ) {
        for ( int x = start_x; x < stop_x; x++ ) {
            unsigned char color = sr_get_pixel(source, x-start_x, y-start_y);
            if (color == source->invisible) { continue; }
            sr_pixel(destination, color, x, y);
        }
    }
}

void sr_blit_pro(
    sr_surface *destination,
    int dest_x,
    int dest_y,
    int dest_width,
    int dest_height,
    sr_surface *source,
    int source_x,
    int source_y,
    int source_width,
    int source_height
) {
    int start_x, start_y, stop_x, stop_y;
    clamp_range(destination, dest_x, dest_y, dest_x+dest_width, dest_y+dest_height, &start_x, &start_y, &stop_x, &stop_y);
    for (int y = start_y; y < stop_y; y++) {
        for (int x = start_x; x < stop_x; x++) {
            int s_x = source_x + (source_width) * (x-dest_x) / (dest_width);
            int s_y = source_y + (source_height) * (y-dest_y) / (dest_height);
            unsigned char color = sr_get_pixel(source, s_x, s_y);
            if (color == source->invisible) { continue; }
            sr_pixel(destination, color, x, y);
        }
    }
}

void sr_fill(
    sr_surface *self,
    unsigned char color
) {
    int buffer_size = self->width * self->height;
    for (int i = 0; i < buffer_size; i++) {
        self->pixels[i] = color_map[color];
    }
}

void sr_pal(unsigned char from, unsigned char to) {
    color_map[from] = to;
}

void sr_pal_reset() {
    for (int i = 0; i < 256; ++i) {
        color_map[i] = i;
    }
}

#endif /* SR_IMPLEMENTATION */

#endif /* SR_HEADER */
