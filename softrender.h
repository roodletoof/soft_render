#ifndef SOFT_RENDER_HEADER
#define SOFT_RENDER_HEADER

#ifndef WINDOW_WIDTH
#define WINDOW_WIDTH 128
#endif
#ifndef WINDOW_HEIGHT
#define WINDOW_HEIGHT 128
#endif
#ifndef WINDOW_TITLE
#define WINDOW_TITLE "Define WINDOW_TITLE to change this."
#endif
#ifndef WINDOW_FPS
#define WINDOW_FPS 60
#endif

typedef unsigned char sr_color;

typedef struct sr_surface {
    const int width;
    const int height;
    sr_color invisible;
    sr_color * const pixels;
} sr_surface;

#define sr_input_just_pressed ((sr_input_flags) 1)
#define sr_input_just_released ((sr_input_flags) 2)
#define sr_input_down ((sr_input_flags) 4)

typedef unsigned char sr_input_flags;

typedef struct sr_input {
    int mouse_x, mouse_y;
    sr_input_flags mouse_1, mouse_2, mouse_3;
    sr_input_flags w, a, s, d, q, e, r, f, shift, space, tab, esc, ctrl;
} sr_input;

sr_color sr_get_pixel(
    sr_surface *self,
    int x,
    int y
);

void sr_pixel(
    sr_surface *self,
    sr_color color,
    int x,
    int y
);
void sr_line(
    sr_surface *self,
    sr_color color,
    int x0,
    int y0,
    int x1,
    int y1
);

void sr_rect_fill(
    sr_surface *self,
    sr_color color,
    int x,
    int y,
    int width,
    int height
);
void sr_rect_outline(
    sr_surface *self,
    sr_color color,
    int x,
    int y,
    int width,
    int height
);

void sr_circle_fill(
    sr_surface *self,
    sr_color color,
    int x,
    int y,
    int radius
);
void sr_circle_outline(
    sr_surface *self,
    sr_color color,
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
    sr_color color
);

#define sr_surface_value(WIDTH, HEIGHT) \
    (sr_surface) { \
        .width = WIDTH, \
        .height = HEIGHT, \
        .pixels = (sr_color[WIDTH*HEIGHT]) {}, \
        .invisible = 0 \
    }

#ifdef SOFT_RENDER_IMPLEMENTATION
static inline void clamp_range(sr_surface *self, int start_x, int start_y, int stop_x, int stop_y, int *start_x_out, int *start_y_out, int *stop_x_out, int *stop_y_out) {
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

sr_color sr_get_pixel(
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
    sr_color color,
    int x,
    int y
) {
    if (
        0 <= x && x < self->width &&
        0 <= y && y < self->height
    ) {
        self->pixels[y * self->width + x] = color;
    }
}

static int abs(int i) {
    return i < 0 ? -i : i;
}

void sr_line(
    sr_surface *self,
    sr_color color,
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
    sr_color color,
    int x,
    int y,
    int width,
    int height
) {
    int start_x, start_y, stop_x, stop_y;
    clamp_range(self, x, y, x+width, y+height, &start_x, &start_y, &stop_x, &stop_y);
    for (int curr_y = start_y; curr_y < stop_y; curr_y++ ) {
        for (int curr_x = start_x; curr_x < stop_x; curr_x++ ) {
            self->pixels[curr_y * self->width + curr_x] = color;
        }
    }
}
void sr_rect_outline(
    sr_surface *self,
    sr_color color,
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

static void circle_line(sr_surface *self, sr_color color, int center_x, int center_y, int x, int y) {
    sr_line(self, color, center_x+x, center_y+y, center_x+x, center_y-y);
    sr_line(self, color, center_x-x, center_y+y, center_x-x, center_y-y);
    sr_line(self, color, center_x+y, center_y+x, center_x+y, center_y-x);
    sr_line(self, color, center_x-y, center_y+x, center_x-y, center_y-x);
}

void sr_circle_fill(
    sr_surface *self,
    sr_color color,
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

static void circle_pixel(sr_surface *self, sr_color color, int center_x, int center_y, int x, int y) {
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
    sr_color color,
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
            sr_color color = sr_get_pixel(source, x-start_x, y-start_y);
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
            sr_color color = sr_get_pixel(source, s_x, s_y);
            if (color == source->invisible) { continue; }
            sr_pixel(destination, color, x, y);
        }
    }
}

void sr_fill(
    sr_surface *self,
    sr_color color
) {
    int buffer_size = self->width * self->height;
    for (int i = 0; i < buffer_size; i++) {
        self->pixels[i] = color;
    }
}
#endif /* SOFT_RENDER_IMPLEMENTATION */

#endif /* SOFT_RENDER_HEADER */
