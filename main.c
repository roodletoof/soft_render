#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdbool.h>

#define WINDOW_TITLE "Game"
#define SOFT_RENDER_IMPLEMENTATION
#define SOFT_RENDER_X11
#include "softrender.h"

#ifdef SOFT_RENDER_X11
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#endif

#define RGB(r,g,b) (b + (g<<8) + (r<<16))
const unsigned long color_map[256] = {
    RGB(0, 0, 0),
    RGB(34, 35, 35),
    RGB(67, 69, 73),
    RGB(98, 104, 113),
    RGB(130, 139, 152),
    RGB(166, 174, 186),
    RGB(205, 210, 218),
    RGB(245, 247, 250),
    RGB(98, 93, 84),
    RGB(133, 117, 101),
    RGB(158, 140, 121),
    RGB(174, 161, 137),
    RGB(187, 175, 164),
    RGB(204, 195, 177),
    RGB(234, 219, 201),
    RGB(255, 243, 214),
    RGB(88, 49, 38),
    RGB(115, 61, 59),
    RGB(136, 80, 65),
    RGB(154, 98, 76),
    RGB(173, 110, 81),
    RGB(213, 141, 107),
    RGB(251, 170, 132),
    RGB(255, 206, 127),
    RGB(0, 39, 53),
    RGB(0, 56, 80),
    RGB(0, 77, 94),
    RGB(11, 102, 127),
    RGB(0, 111, 137),
    RGB(50, 140, 167),
    RGB(36, 174, 214),
    RGB(136, 214, 255),
    RGB(102, 43, 41),
    RGB(148, 54, 58),
    RGB(182, 77, 70),
    RGB(205, 94, 70),
    RGB(227, 120, 64),
    RGB(249, 155, 78),
    RGB(255, 188, 78),
    RGB(255, 233, 73),
    RGB(40, 43, 74),
    RGB(58, 69, 104),
    RGB(97, 95, 132),
    RGB(122, 119, 153),
    RGB(134, 144, 178),
    RGB(150, 178, 217),
    RGB(199, 214, 255),
    RGB(198, 236, 255),
    RGB(0, 34, 25),
    RGB(0, 50, 33),
    RGB(23, 74, 27),
    RGB(34, 89, 24),
    RGB(47, 105, 12),
    RGB(81, 136, 34),
    RGB(125, 164, 45),
    RGB(166, 204, 52),
    RGB(24, 31, 47),
    RGB(35, 50, 77),
    RGB(37, 70, 107),
    RGB(54, 107, 138),
    RGB(49, 142, 184),
    RGB(65, 178, 227),
    RGB(82, 210, 255),
    RGB(116, 245, 253),
    RGB(26, 51, 44),
    RGB(47, 63, 56),
    RGB(56, 81, 64),
    RGB(50, 92, 64),
    RGB(65, 116, 85),
    RGB(73, 137, 96),
    RGB(85, 182, 125),
    RGB(145, 218, 161),
    RGB(94, 7, 17),
    RGB(130, 33, 29),
    RGB(182, 60, 53),
    RGB(228, 92, 95),
    RGB(255, 118, 118),
    RGB(255, 155, 168),
    RGB(255, 187, 199),
    RGB(255, 219, 255),
    RGB(45, 49, 54),
    RGB(72, 71, 77),
    RGB(91, 92, 105),
    RGB(115, 115, 127),
    RGB(132, 135, 149),
    RGB(171, 174, 190),
    RGB(186, 199, 219),
    RGB(235, 240, 246),
    RGB(59, 48, 60),
    RGB(90, 60, 69),
    RGB(138, 82, 88),
    RGB(174, 107, 96),
    RGB(199, 130, 108),
    RGB(216, 159, 117),
    RGB(236, 197, 129),
    RGB(255, 250, 171),
    RGB(49, 34, 42),
    RGB(74, 53, 60),
    RGB(94, 70, 70),
    RGB(114, 90, 81),
    RGB(126, 108, 84),
    RGB(158, 138, 110),
    RGB(192, 165, 136),
    RGB(221, 191, 154),
    RGB(46, 16, 38),
    RGB(73, 40, 61),
    RGB(102, 54, 89),
    RGB(151, 84, 117),
    RGB(185, 109, 145),
    RGB(193, 120, 170),
    RGB(219, 153, 191),
    RGB(248, 198, 218),
    RGB(0, 46, 73),
    RGB(0, 64, 81),
    RGB(0, 81, 98),
    RGB(0, 107, 109),
    RGB(0, 130, 121),
    RGB(0, 160, 135),
    RGB(0, 191, 163),
    RGB(0, 222, 218),
    RGB(69, 49, 37),
    RGB(97, 74, 60),
    RGB(126, 97, 68),
    RGB(153, 121, 81),
    RGB(178, 144, 98),
    RGB(204, 169, 110),
    RGB(232, 203, 130),
    RGB(251, 234, 163),
    RGB(95, 9, 38),
    RGB(110, 36, 52),
    RGB(144, 70, 71),
    RGB(167, 96, 87),
    RGB(189, 125, 100),
    RGB(206, 151, 112),
    RGB(237, 182, 124),
    RGB(237, 212, 147),
    RGB(50, 53, 88),
    RGB(74, 82, 128),
    RGB(100, 101, 157),
    RGB(120, 119, 193),
    RGB(142, 140, 226),
    RGB(156, 155, 239),
    RGB(184, 174, 255),
    RGB(220, 212, 255),
    RGB(67, 23, 41),
    RGB(113, 43, 59),
    RGB(159, 59, 82),
    RGB(217, 74, 105),
    RGB(248, 93, 128),
    RGB(255, 125, 175),
    RGB(255, 166, 197),
    RGB(255, 205, 255),
    RGB(73, 37, 28),
    RGB(99, 52, 50),
    RGB(124, 75, 71),
    RGB(152, 89, 90),
    RGB(172, 111, 110),
    RGB(193, 126, 122),
    RGB(210, 141, 122),
    RGB(229, 154, 124),
    RGB(32, 41, 0),
    RGB(47, 79, 8),
    RGB(73, 93, 0),
    RGB(97, 115, 8),
    RGB(124, 131, 30),
    RGB(150, 154, 38),
    RGB(180, 170, 51),
    RGB(208, 204, 50),
    RGB(98, 42, 0),
    RGB(117, 59, 9),
    RGB(133, 79, 18),
    RGB(158, 101, 32),
    RGB(186, 136, 46),
    RGB(209, 170, 57),
    RGB(232, 210, 75),
    RGB(255, 246, 79),
    RGB(38, 35, 61),
    RGB(59, 56, 85),
    RGB(86, 80, 111),
    RGB(117, 104, 110),
    RGB(145, 122, 123),
    RGB(179, 151, 131),
    RGB(207, 175, 142),
    RGB(254, 223, 177),
    RGB(29, 44, 67),
    RGB(46, 61, 71),
    RGB(57, 77, 60),
    RGB(76, 95, 51),
    RGB(88, 113, 44),
    RGB(107, 132, 45),
    RGB(120, 158, 36),
    RGB(127, 189, 57),
    RGB(55, 36, 35),
    RGB(83, 57, 58),
    RGB(120, 76, 73),
    RGB(148, 93, 79),
    RGB(169, 109, 88),
    RGB(191, 126, 99),
    RGB(215, 147, 116),
    RGB(244, 163, 128),
    RGB(45, 75, 71),
    RGB(71, 101, 90),
    RGB(91, 123, 105),
    RGB(113, 149, 125),
    RGB(135, 174, 142),
    RGB(138, 193, 150),
    RGB(169, 209, 193),
    RGB(224, 250, 235),
    RGB(0, 27, 64),
    RGB(3, 49, 95),
    RGB(7, 72, 124),
    RGB(16, 93, 162),
    RGB(20, 118, 192),
    RGB(64, 151, 234),
    RGB(85, 177, 241),
    RGB(109, 204, 255),
    RGB(85, 71, 105),
    RGB(118, 93, 115),
    RGB(151, 116, 136),
    RGB(185, 140, 147),
    RGB(213, 163, 154),
    RGB(235, 189, 157),
    RGB(255, 213, 155),
    RGB(253, 247, 134),
    RGB(29, 29, 33),
    RGB(60, 49, 81),
    RGB(88, 74, 127),
    RGB(121, 100, 186),
    RGB(149, 133, 241),
    RGB(169, 150, 236),
    RGB(186, 171, 247),
    RGB(209, 189, 254),
    RGB(38, 36, 80),
    RGB(40, 51, 93),
    RGB(45, 61, 114),
    RGB(61, 80, 131),
    RGB(81, 101, 174),
    RGB(82, 116, 197),
    RGB(108, 130, 196),
    RGB(131, 147, 195),
    RGB(73, 33, 41),
    RGB(94, 65, 74),
    RGB(119, 83, 91),
    RGB(145, 96, 106),
    RGB(173, 121, 132),
    RGB(181, 139, 148),
    RGB(212, 174, 170),
    RGB(255, 226, 207),
    RGB(114, 28, 3),
    RGB(156, 51, 39),
    RGB(191, 90, 62),
    RGB(233, 134, 39),
    RGB(255, 177, 8),
    RGB(255, 207, 5),
    RGB(255, 240, 43),
    RGB(247, 244, 191)
};
#undef RGB

int update(float dt, sr_input const * input);
void draw(sr_surface *screen, sr_input const * input);
int sr_launch(int (*update) (float dt, sr_input const * input), void (*draw) (sr_surface *screen, sr_input const * input));

int main() {
    return sr_launch(update, draw);
}

int update(float dt, sr_input const * input) {

    printf("diff from expected dt: %f\n", 1.0f/60.0f - dt);

    if (input->esc & sr_input_just_released) {
        return 1;
    }

    return 0;
}

void draw(sr_surface *screen, sr_input const * input) {
    sr_fill(screen, 25);
    sr_circle_fill(screen,7,64,64,40);
    sr_circle_fill(screen,4,64,64,20);

    sr_line(screen, 0, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    if (input->a & sr_input_just_pressed) {
        sr_rect_outline(screen, 29, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    }

    if (input->e & sr_input_down) {
        sr_circle_fill(screen,2,64,64,10);
    }
}

static void key_press_set_flags(sr_input_flags *f) {
    *f |= sr_input_down;
    if (!(*f & sr_input_just_released)) {
        *f |= sr_input_just_pressed;
    } else {
        *f &= ~sr_input_just_released;
    }
}

static void key_release_set_flags(sr_input_flags *f) {
    *f |= sr_input_just_released;
    *f &= ~sr_input_down;
}

int sr_launch(int (*update)(float dt, sr_input const * input), void (*draw)(sr_surface *screen, sr_input const * input)) {
    sr_surface *screen = &sr_surface_value(WINDOW_WIDTH,WINDOW_HEIGHT);
    sr_fill(screen, 0);

    Display *display;
    Window window;

    // Open a connection to the X server
    display = XOpenDisplay(NULL);
    if (display == NULL) {
        fprintf(stderr, "Cannot open display\n");
        return 1;
    }

    window = XCreateSimpleWindow(display, RootWindow(display, 0), 0, 0, 800, 800, 0, 0, BlackPixel(display, 0));
    XStoreName(display, window, WINDOW_TITLE);

    // Select the events to monitor and display the window
    XSelectInput(display, window, ExposureMask | KeyPressMask | KeyReleaseMask);
    XMapWindow(display, window);

    // Graphics context
    GC gc = XCreateGC(display, window, 0, NULL);

    const float target_frame_time = 1.0f / ((float) WINDOW_FPS);
    const clock_t target_frame_clocks = CLOCKS_PER_SEC / WINDOW_FPS;
    clock_t last_time = clock();

    while (1) {
        clock_t current_time = clock();
        float dt; // TODO dt calculation and (possibly) sleeping time is wrong.
        if (current_time < last_time) {
            dt = target_frame_time; // Shitty fix
        } else {
            dt = ((float) (current_time - last_time)) / ((float) CLOCKS_PER_SEC);
        }
        last_time = current_time;

        int min_scale, off_x, off_y;
        static sr_input input = {};

        const sr_input_flags mask = (~sr_input_just_pressed) & (~sr_input_just_released);
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
                    case XK_w:
                        key_press_set_flags(&input.w); break;
                    case XK_a:
                        key_press_set_flags(&input.a); break;
                    case XK_s:
                        key_press_set_flags(&input.s); break;
                    case XK_d:
                        key_press_set_flags(&input.d); break;
                    case XK_q:
                        key_press_set_flags(&input.q); break;
                    case XK_e:
                        key_press_set_flags(&input.e); break;
                    case XK_r:
                        key_press_set_flags(&input.r); break;
                    case XK_f:
                        key_press_set_flags(&input.f); break;
                    case XK_Shift_L:
                        key_press_set_flags(&input.shift); break;
                    case XK_space:
                        key_press_set_flags(&input.space); break;
                    case XK_Tab:
                        key_press_set_flags(&input.tab); break;
                    case XK_Escape:
                        key_press_set_flags(&input.esc); break;
                    case XK_Control_L:
                        key_press_set_flags(&input.ctrl); break;
                    default: break;
                }
            }
            if (event.type == KeyRelease) {
                KeySym key = XLookupKeysym(&event.xkey, 0);
                KeySym lower_case_key, _;
                XConvertCase(key, &lower_case_key, &_);

                switch (lower_case_key) {
                    case XK_w:
                        key_release_set_flags(&input.w); break;
                    case XK_a:
                        key_release_set_flags(&input.a); break;
                    case XK_s:
                        key_release_set_flags(&input.s); break;
                    case XK_d:
                        key_release_set_flags(&input.d); break;
                    case XK_q:
                        key_release_set_flags(&input.q); break;
                    case XK_e:
                        key_release_set_flags(&input.e); break;
                    case XK_r:
                        key_release_set_flags(&input.r); break;
                    case XK_f:
                        key_release_set_flags(&input.f); break;
                    case XK_Shift_L:
                        key_release_set_flags(&input.shift); break;
                    case XK_space:
                        key_release_set_flags(&input.space); break;
                    case XK_Tab:
                        key_release_set_flags(&input.tab); break;
                    case XK_Escape:
                        key_release_set_flags(&input.esc); break;
                    case XK_Control_L:
                        key_release_set_flags(&input.ctrl); break;
                    default: break;
                }
            }
        }

        if (update(dt, &input)) { break; }
        draw(screen, &input);

        for (int y = 0; y < WINDOW_HEIGHT; ++y) {
            for (int x = 0; x < WINDOW_WIDTH; ++x) {
                XSetForeground(display, gc, color_map[sr_get_pixel(screen, x, y)]);
                XFillRectangle(display, window, gc, off_x + min_scale * x, off_y + min_scale * y, min_scale, min_scale);
            }
        }

        clock_t time_passed_doing_logic = clock() - last_time;
        if (time_passed_doing_logic < 0) {
            time_passed_doing_logic = 0;
        }
        usleep(target_frame_clocks - time_passed_doing_logic);
    }

    XFreeGC(display, gc);
    XCloseDisplay(display);

    return 0;
}
