#ifndef C8_HEADER
#define C8_HEADER

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

#define RGB(r,g,b) (b + (g<<8) + (r<<16))

typedef struct c8_surface {
    const int width;
    const int height;
    unsigned char invisible;
    unsigned char * const pixels;
} c8_surface;

typedef unsigned char c8_input_flags;
#define c8_input_just_pressed ((c8_input_flags) 1)
#define c8_input_just_released ((c8_input_flags) 2)
#define c8_input_down ((c8_input_flags) 4)

typedef struct c8_input {
    int mouse_x, mouse_y;
    c8_input_flags mouse_1, mouse_2, mouse_3;
    c8_input_flags w, a, s, d, q, e, r, f, shift, space, tab, esc, ctrl;
} c8_input;

void c8_pal(unsigned char from, unsigned char to);
void c8_pal_reset();

unsigned char c8_get_pixel(
    c8_surface *self,
    int x,
    int y
);

void c8_pixel(
    c8_surface *self,
    unsigned char color,
    int x,
    int y
);
void c8_line(
    c8_surface *self,
    unsigned char color,
    int x0,
    int y0,
    int x1,
    int y1
);

void c8_rect_fill(
    c8_surface *self,
    unsigned char color,
    int x,
    int y,
    int width,
    int height
);
void c8_rect_outline(
    c8_surface *self,
    unsigned char color,
    int x,
    int y,
    int width,
    int height
);

void c8_circle_fill(
    c8_surface *self,
    unsigned char color,
    int x,
    int y,
    int radius
);
void c8_circle_outline(
    c8_surface *self,
    unsigned char color,
    int x,
    int y,
    int radius
);

void c8_blit(
    c8_surface *destination,
    int dest_x,
    int dest_y,
    c8_surface *source
);
void c8_blit_pro(
    c8_surface *destination,
    int dest_x,
    int dest_y,
    int dest_width,
    int dest_height,
    c8_surface *source,
    int source_x,
    int source_y,
    int source_width,
    int source_height
);

void c8_fill(
    c8_surface *self,
    unsigned char color
);

#define c8_surface_value(WIDTH, HEIGHT) \
    (c8_surface) { \
        .width = WIDTH, \
        .height = HEIGHT, \
        .pixels = (unsigned char[WIDTH*HEIGHT]) {}, \
        .invisible = 0 \
    }

int c8_launch(void (*update) (float dt, c8_input const * input), void (*draw) (c8_surface *screen, c8_input const * input));
void c8_exit(void);

#ifdef C8_IMPLEMENTATION
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>
#include <time.h>

static unsigned char color_map[16] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};

static inline void clamp_range(
        c8_surface *self,
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

unsigned char c8_get_pixel(
    c8_surface *self,
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

void c8_pixel(
    c8_surface *self,
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

void c8_line(
    c8_surface *self,
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
        c8_pixel( self, color, x0, y0 );
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

void c8_rect_fill(
    c8_surface *self,
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
void c8_rect_outline(
    c8_surface *self,
    unsigned char color,
    int x,
    int y,
    int width,
    int height
) {
    for ( int x2 = x; x2 < x + width; x2++ ) {
        c8_pixel( self, color, x2, y );
        c8_pixel( self, color, x2, y + height - 1 );
    }
    for ( int y2 = y+1; y2 < y + height - 1; y2++ ) {
        c8_pixel( self, color, x, y2 );
        c8_pixel( self, color, x + width - 1, y2 );
    }
}

static void circle_line(c8_surface *self, unsigned char color, int center_x, int center_y, int x, int y) {
    c8_line(self, color, center_x+x, center_y+y, center_x+x, center_y-y);
    c8_line(self, color, center_x-x, center_y+y, center_x-x, center_y-y);
    c8_line(self, color, center_x+y, center_y+x, center_x+y, center_y-x);
    c8_line(self, color, center_x-y, center_y+x, center_x-y, center_y-x);
}

void c8_circle_fill(
    c8_surface *self,
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

static void circle_pixel(c8_surface *self, unsigned char color, int center_x, int center_y, int x, int y) {
    c8_pixel(self, color, center_x+x, center_y+y);
    c8_pixel(self, color, center_x-x, center_y+y);
    c8_pixel(self, color, center_x+x, center_y-y);
    c8_pixel(self, color, center_x-x, center_y-y);
    c8_pixel(self, color, center_x+y, center_y+x);
    c8_pixel(self, color, center_x-y, center_y+x);
    c8_pixel(self, color, center_x+y, center_y-x);
    c8_pixel(self, color, center_x-y, center_y-x);
}

void c8_circle_outline(
    c8_surface *self,
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

void c8_blit(
    c8_surface *destination,
    int dest_x,
    int dest_y,
    c8_surface *source
) {
    int start_x, start_y, stop_x, stop_y;
    clamp_range(destination, dest_x, dest_y, dest_x+source->width, dest_y+source->height, &start_x, &start_y, &stop_x, &stop_y);
    for ( int y = start_y; y < stop_y; y++ ) {
        for ( int x = start_x; x < stop_x; x++ ) {
            unsigned char color = c8_get_pixel(source, x-start_x, y-start_y);
            if (color == source->invisible) { continue; }
            c8_pixel(destination, color, x, y);
        }
    }
}

void c8_blit_pro(
    c8_surface *destination,
    int dest_x,
    int dest_y,
    int dest_width,
    int dest_height,
    c8_surface *source,
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
            unsigned char color = c8_get_pixel(source, s_x, s_y);
            if (color == source->invisible) { continue; }
            c8_pixel(destination, color, x, y);
        }
    }
}

void c8_fill(
    c8_surface *self,
    unsigned char color
) {
    int buffer_size = self->width * self->height;
    for (int i = 0; i < buffer_size; i++) {
        self->pixels[i] = color_map[color];
    }
}

static const unsigned long colors[16] = {
        RGB(0, 0, 0),
        RGB(29, 43, 83),
        RGB(126, 37, 83),
        RGB(0, 135, 81),
        RGB(171, 82, 54),
        RGB(95, 87, 79),
        RGB(194, 195, 199),
        RGB(255, 241, 232),
        RGB(255, 0, 77),
        RGB(255, 163, 0),
        RGB(255, 236, 39),
        RGB(0, 228, 54),
        RGB(41, 173, 255),
        RGB(131, 118, 156),
        RGB(255, 119, 168),
        RGB(255, 204, 170),
};

void c8_pal(unsigned char from, unsigned char to) {
    color_map[from] = to;
}

void c8_pal_reset() {
    for (int i = 0; i < 16; ++i) {
        color_map[i] = i;
    }
}

static void key_press_set_flags(c8_input_flags *f) {
    *f |= c8_input_down;
    if (!(*f & c8_input_just_released)) {
        *f |= c8_input_just_pressed;
    } else {
        *f &= ~c8_input_just_released;
    }
}

static void key_release_set_flags(c8_input_flags *f) {
    *f |= c8_input_just_released;
    *f &= ~c8_input_down;
}

static int should_close = 0;
void c8_exit() {
    should_close = 1;
}

int c8_launch(void (*update)(float dt, c8_input const * input), void (*draw)(c8_surface *screen, c8_input const * input)) {
    // c8 init
    c8_surface *screen = &c8_surface_value(WINDOW_WIDTH,WINDOW_HEIGHT);
    c8_fill(screen, 0);

    // x11 init
    Display *display;
    Window window;
    display = XOpenDisplay(NULL);
    if (display == NULL) {
        fprintf(stderr, "Cannot open display\n");
        return 1;
    }
    window = XCreateSimpleWindow(display, RootWindow(display, 0), 0, 0, 800, 800, 0, 0, BlackPixel(display, 0));
    XStoreName(display, window, WINDOW_TITLE);
    XSelectInput(display, window, ExposureMask | KeyPressMask | KeyReleaseMask);
    XMapWindow(display, window);
    GC gc = XCreateGC(display, window, 0, NULL);

    // update timing control
    long long nanoseconds_per_frame = 1000000000 / WINDOW_FPS;
    struct timespec start_time, current_time, sleep_time;
    float dt;
    clock_gettime(CLOCK_MONOTONIC, &start_time);

    while (!should_close) {
        clock_gettime(CLOCK_MONOTONIC, &current_time);
        dt = (float)(current_time.tv_sec - start_time.tv_sec) + (float)(current_time.tv_nsec - start_time.tv_nsec) / 1e9f;
        start_time = current_time;

        static int min_scale, off_x, off_y;
        static c8_input input = {};

        const c8_input_flags mask = ~(c8_input_just_pressed | c8_input_just_released);
        input.w &= mask;
        input.a &= mask;
        input.s &= mask;
        input.d &= mask;
        input.q &= mask;
        input.e &= mask;
        input.r &= mask;
        input.f &= mask;
        input.shift &= mask;
        input.space &= mask;
        input.tab &= mask;
        input.esc &= mask;
        input.ctrl &= mask;

        while (XPending(display) > 0) {
            XEvent event;
            XNextEvent(display, &event);

            if (event.type == Expose) {
                XClearWindow(display, window);

                XWindowAttributes attrs;
                XGetWindowAttributes(display, window, &attrs);
                int scale_x = attrs.width / WINDOW_WIDTH;
                int scale_y = attrs.height / WINDOW_HEIGHT;
                min_scale = scale_x < scale_y ? scale_x : scale_y;
                off_x = (attrs.width - min_scale * WINDOW_WIDTH) / 2;
                off_y = (attrs.height - min_scale * WINDOW_HEIGHT) / 2;
            }
            if (event.type == KeyPress) {
                KeySym key = XLookupKeysym(&event.xkey, 0);
                KeySym lower_case_key, _;
                XConvertCase(key, &lower_case_key, &_);

                switch (lower_case_key) {
                    case XK_w:key_press_set_flags(&input.w); break;
                    case XK_a:key_press_set_flags(&input.a); break;
                    case XK_s:key_press_set_flags(&input.s); break;
                    case XK_d:key_press_set_flags(&input.d); break;
                    case XK_q:key_press_set_flags(&input.q); break;
                    case XK_e:key_press_set_flags(&input.e); break;
                    case XK_r:key_press_set_flags(&input.r); break;
                    case XK_f:key_press_set_flags(&input.f); break;
                    case XK_Shift_L:key_press_set_flags(&input.shift); break;
                    case XK_space:key_press_set_flags(&input.space); break;
                    case XK_Tab:key_press_set_flags(&input.tab); break;
                    case XK_Escape:key_press_set_flags(&input.esc); break;
                    case XK_Control_L:key_press_set_flags(&input.ctrl); break;
                    default: break;
                }
            }
            if (event.type == KeyRelease) {
                KeySym key = XLookupKeysym(&event.xkey, 0);
                KeySym lower_case_key, _;
                XConvertCase(key, &lower_case_key, &_);

                switch (lower_case_key) {
                    case XK_w:key_release_set_flags(&input.w); break;
                    case XK_a:key_release_set_flags(&input.a); break;
                    case XK_s:key_release_set_flags(&input.s); break;
                    case XK_d:key_release_set_flags(&input.d); break;
                    case XK_q:key_release_set_flags(&input.q); break;
                    case XK_e:key_release_set_flags(&input.e); break;
                    case XK_r:key_release_set_flags(&input.r); break;
                    case XK_f:key_release_set_flags(&input.f); break;
                    case XK_Shift_L:key_release_set_flags(&input.shift); break;
                    case XK_space:key_release_set_flags(&input.space); break;
                    case XK_Tab:key_release_set_flags(&input.tab); break;
                    case XK_Escape:key_release_set_flags(&input.esc); break;
                    case XK_Control_L:key_release_set_flags(&input.ctrl); break;
                    default: break;
                }
            }
        }

        update(dt, &input);
        draw(screen, &input);

        for (int y = 0; y < WINDOW_HEIGHT; ++y) {
            for (int x = 0; x < WINDOW_WIDTH; ++x) {
                XSetForeground(display, gc, colors[c8_get_pixel(screen, x, y)]);
                XFillRectangle(display, window, gc, off_x + min_scale * x, off_y + min_scale * y, min_scale, min_scale);
            }
        }

        long long remaining_time = nanoseconds_per_frame - (current_time.tv_nsec - start_time.tv_nsec);
        if (remaining_time > 0) {
            sleep_time.tv_sec = remaining_time / 1000000000;
            sleep_time.tv_nsec = remaining_time % 1000000000;
            nanosleep(&sleep_time, NULL);
        }
    }

    XFreeGC(display, gc);
    XCloseDisplay(display);

    return 0;
}

#endif /* C8_IMPLEMENTATION */

#endif /* C8_HEADER */
