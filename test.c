#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define SR_IMPLEMENTATION
#include "sr.h"

#define WIDTH 256
#define HEIGHT 256

sr_surface *screen = &sr_surface_value(WIDTH, HEIGHT);

void ppm_save(const char* filename, sr_surface *surf) {
    FILE *f = fopen(filename, "wb");
    if (!f) {
        perror("file could not be opened");
    }
    fprintf(f, "P6\n%d\n%d\n255\n", surf->width, surf->height);
    for ( size_t y = 0; y < surf->height; y++ ) {
        for ( size_t x = 0; x < surf->width; x++ ) {
            unsigned char c = sr_get_pixel(surf, x, y);
            unsigned char color[3] = {c, c, c};
            fwrite(color, sizeof(c), 3, f);
        }
    }
    fclose(f);
}


// test definition
#define RED "\033[31m"
#define GREEN "\033[32m"
#define DEFAULT "\033[39m"

int tests_total = 0;
int tests_passed = 0;
typedef enum {result_success, result_failure} result_t;

result_t test(bool condition, char *condition_text, char *file, int line) {

    tests_total++;
    result_t result;
    if (condition) {
        // printf(GREEN " SUCCESS " DEFAULT "%s" "\n", condition_text);
        tests_passed++;
        result = result_success;
    } else {
        printf(RED " FAILURE " DEFAULT "%s" " " "%s" ":%d\n", condition_text, file, line);
        result = result_failure;

        // produce image of screen for debugging.
        const unsigned long filename_size = 256;
        char filename[filename_size];
        snprintf(filename, filename_size, "%s_%d.ppm", file, line);
        ppm_save(filename, screen);

    }
    fflush(stdout);
    return result;
}

#define test(condition) test(condition, #condition, __FILE__, __LINE__)

#define BLACK 0
#define GRAY 127
#define WHITE 255

int main() {

    screen->invisible = BLACK;


    { // TEST

        // circle
        sr_fill(screen, BLACK);
        sr_circle_fill(screen, WHITE, 64, 64, 64);
        test(sr_get_pixel(screen, 64, 64) == WHITE);
        test(sr_get_pixel(screen, 64+63, 64) == WHITE);
        test(sr_get_pixel(screen, 64, 64+63) == WHITE);
        test(sr_get_pixel(screen, 64+65, 64) == BLACK);
        test(sr_get_pixel(screen, 64, 64+65) == BLACK);
        test(sr_get_pixel(screen, 128, 128) == BLACK);
        sr_circle_fill_masked(screen, GRAY, 128, 64, 64);
        test(sr_get_pixel(screen, 64+63, 64) == GRAY);
        test(sr_get_pixel(screen, 64+65, 64) == BLACK);
        test(sr_get_pixel(screen, 57, 64) == WHITE);
        sr_fill_masked(screen, WHITE);
        test(sr_get_pixel(screen, 64, 64) == WHITE);
        test(sr_get_pixel(screen, 64+63, 64) == WHITE);
        test(sr_get_pixel(screen, 64, 64+63) == WHITE);
        test(sr_get_pixel(screen, 64+65, 64) == BLACK);
        test(sr_get_pixel(screen, 64, 64+65) == BLACK);
        test(sr_get_pixel(screen, 128, 128) == BLACK);

        sr_fill(screen, BLACK);
        sr_circle_outline(screen, WHITE, 64, 64, 64);
        test(sr_get_pixel(screen, 64, 64) == BLACK);
        test(sr_get_pixel(screen, 64+64, 64) == WHITE);
        test(sr_get_pixel(screen, 64, 64+64) == WHITE);
        test(sr_get_pixel(screen, 64+65, 64) == BLACK);
        test(sr_get_pixel(screen, 64, 64+65) == BLACK);
        test(sr_get_pixel(screen, 64+63, 64) == BLACK);
        test(sr_get_pixel(screen, 64, 64+63) == BLACK);
        test(sr_get_pixel(screen, 128, 128) == BLACK);
        sr_fill(screen, BLACK);
        sr_rect_fill(screen, GRAY, 0, 0, 70, 130);
        sr_circle_outline_masked(screen, WHITE, 64, 64, 64);
        test(sr_get_pixel(screen, 64, 64) == GRAY);
        test(sr_get_pixel(screen, 64+64, 64) == BLACK);
        test(sr_get_pixel(screen, 64, 64+64) == WHITE);
        test(sr_get_pixel(screen, 64+65, 64) == BLACK);
        test(sr_get_pixel(screen, 64, 64+65) == GRAY);
        test(sr_get_pixel(screen, 64+63, 64) == BLACK);
        test(sr_get_pixel(screen, 64, 64+63) == GRAY);
        test(sr_get_pixel(screen, 128, 128) == BLACK);

        // line
        sr_fill(screen, BLACK);
        sr_line(screen, WHITE, 0, 0, 200, 100);
        test(sr_get_pixel(screen, 0, 0) == WHITE);
        test(sr_get_pixel(screen, 200, 100) == WHITE);
        test(sr_get_pixel(screen, 200, 0) == BLACK);
        test(sr_get_pixel(screen, 0, 100) == BLACK);
        sr_fill(screen, BLACK);
        sr_rect_fill(screen, GRAY, 0, 0, 70, 130);
        sr_line_masked(screen, WHITE, 0, 0, 200, 100);
        test(sr_get_pixel(screen, 0, 0) == WHITE);
        test(sr_get_pixel(screen, 200, 100) == BLACK);
        test(sr_get_pixel(screen, 200, 0) == BLACK);
        test(sr_get_pixel(screen, 0, 100) == GRAY);

        // rect
        sr_fill(screen, BLACK);
        sr_rect_fill(screen, WHITE, 1, 1, 100, 100);
        test(sr_get_pixel(screen, 0, 0) == BLACK);
        test(sr_get_pixel(screen, 1, 1) == WHITE);
        test(sr_get_pixel(screen, 100, 100) == WHITE);
        test(sr_get_pixel(screen, 101, 100) == BLACK);
        test(sr_get_pixel(screen, 101, 101) == BLACK);
        test(sr_get_pixel(screen, 50, 50) == WHITE);
        sr_fill(screen, BLACK);
        sr_rect_fill(screen, GRAY, 0, 0, 70, 130);
        sr_rect_fill_masked(screen, WHITE, 1, 1, 100, 100);
        test(sr_get_pixel(screen, 0, 0) == GRAY);
        test(sr_get_pixel(screen, 1, 1) == WHITE);
        test(sr_get_pixel(screen, 100, 100) == BLACK);
        test(sr_get_pixel(screen, 101, 100) == BLACK);
        test(sr_get_pixel(screen, 101, 101) == BLACK);
        test(sr_get_pixel(screen, 50, 50) == WHITE);

        sr_fill(screen, BLACK);
        sr_rect_outline(screen, WHITE, 1, 1, 100, 100);
        test(sr_get_pixel(screen, 0, 0) == BLACK);
        test(sr_get_pixel(screen, 1, 1) == WHITE);
        test(sr_get_pixel(screen, 100, 100) == WHITE);
        test(sr_get_pixel(screen, 101, 100) == BLACK);
        test(sr_get_pixel(screen, 101, 101) == BLACK);
        test(sr_get_pixel(screen, 50, 50) == BLACK);
        sr_fill(screen, BLACK);
        sr_rect_fill(screen, GRAY, 0, 0, 70, 130);
        sr_rect_outline_masked(screen, WHITE, 1, 1, 100, 100);
        test(sr_get_pixel(screen, 0, 0) == GRAY);
        test(sr_get_pixel(screen, 1, 1) == WHITE);
        test(sr_get_pixel(screen, 100, 100) == BLACK);
        test(sr_get_pixel(screen, 101, 100) == BLACK);
        test(sr_get_pixel(screen, 101, 101) == BLACK);
        test(sr_get_pixel(screen, 50, 50) == GRAY);

    }


    { // TEST with other palette

        sr_pal(BLACK, WHITE);
        sr_pal(WHITE, GRAY);
        sr_pal(GRAY, BLACK);

        unsigned char map[256] = {0};
        map[BLACK] = WHITE;
        map[WHITE] = GRAY;
        map[GRAY] = BLACK;
        screen->invisible = WHITE;

        // circle
        sr_fill(screen, BLACK);
        sr_circle_fill(screen, WHITE, 64, 64, 64);
        test(sr_get_pixel(screen, 64, 64) == map[WHITE]);
        test(sr_get_pixel(screen, 64+63, 64) == map[WHITE]);
        test(sr_get_pixel(screen, 64, 64+63) == map[WHITE]);
        test(sr_get_pixel(screen, 64+65, 64) == map[BLACK]);
        test(sr_get_pixel(screen, 64, 64+65) == map[BLACK]);
        test(sr_get_pixel(screen, 128, 128) == map[BLACK]);
        sr_circle_fill_masked(screen, GRAY, 128, 64, 64);
        test(sr_get_pixel(screen, 64+63, 64) == map[GRAY]);
        test(sr_get_pixel(screen, 64+65, 64) == map[BLACK]);
        test(sr_get_pixel(screen, 57, 64) == map[WHITE]);
        sr_fill_masked(screen, WHITE);
        test(sr_get_pixel(screen, 64, 64) == map[WHITE]);
        test(sr_get_pixel(screen, 64+63, 64) == map[WHITE]);
        test(sr_get_pixel(screen, 64, 64+63) == map[WHITE]);
        test(sr_get_pixel(screen, 64+65, 64) == map[BLACK]);
        test(sr_get_pixel(screen, 64, 64+65) == map[BLACK]);
        test(sr_get_pixel(screen, 128, 128) == map[BLACK]);

        sr_fill(screen, BLACK);
        sr_circle_outline(screen, WHITE, 64, 64, 64);
        test(sr_get_pixel(screen, 64, 64) == map[BLACK]);
        test(sr_get_pixel(screen, 64+64, 64) == map[WHITE]);
        test(sr_get_pixel(screen, 64, 64+64) == map[WHITE]);
        test(sr_get_pixel(screen, 64+65, 64) == map[BLACK]);
        test(sr_get_pixel(screen, 64, 64+65) == map[BLACK]);
        test(sr_get_pixel(screen, 64+63, 64) == map[BLACK]);
        test(sr_get_pixel(screen, 64, 64+63) == map[BLACK]);
        test(sr_get_pixel(screen, 128, 128) == map[BLACK]);
        sr_fill(screen, BLACK);
        sr_rect_fill(screen, GRAY, 0, 0, 70, 130);
        sr_circle_outline_masked(screen, WHITE, 64, 64, 64);
        test(sr_get_pixel(screen, 64, 64) == map[GRAY]);
        test(sr_get_pixel(screen, 64+64, 64) == map[BLACK]);
        test(sr_get_pixel(screen, 64, 64+64) == map[WHITE]);
        test(sr_get_pixel(screen, 64+65, 64) == map[BLACK]);
        test(sr_get_pixel(screen, 64, 64+65) == map[GRAY]);
        test(sr_get_pixel(screen, 64+63, 64) == map[BLACK]);
        test(sr_get_pixel(screen, 64, 64+63) == map[GRAY]);
        test(sr_get_pixel(screen, 128, 128) == map[BLACK]);

        // line
        sr_fill(screen, BLACK);
        sr_line(screen, WHITE, 0, 0, 200, 100);
        test(sr_get_pixel(screen, 0, 0) == map[WHITE]);
        test(sr_get_pixel(screen, 200, 100) == map[WHITE]);
        test(sr_get_pixel(screen, 200, 0) == map[BLACK]);
        test(sr_get_pixel(screen, 0, 100) == map[BLACK]);
        sr_fill(screen, BLACK);
        sr_rect_fill(screen, GRAY, 0, 0, 70, 130);
        sr_line_masked(screen, WHITE, 0, 0, 200, 100);
        test(sr_get_pixel(screen, 0, 0) == map[WHITE]);
        test(sr_get_pixel(screen, 200, 100) == map[BLACK]);
        test(sr_get_pixel(screen, 200, 0) == map[BLACK]);
        test(sr_get_pixel(screen, 0, 100) == map[GRAY]);

        // rect
        sr_fill(screen, BLACK);
        sr_rect_fill(screen, WHITE, 1, 1, 100, 100);
        test(sr_get_pixel(screen, 0, 0) == map[BLACK]);
        test(sr_get_pixel(screen, 1, 1) == map[WHITE]);
        test(sr_get_pixel(screen, 100, 100) == map[WHITE]);
        test(sr_get_pixel(screen, 101, 100) == map[BLACK]);
        test(sr_get_pixel(screen, 101, 101) == map[BLACK]);
        test(sr_get_pixel(screen, 50, 50) == map[WHITE]);
        sr_fill(screen, BLACK);
        sr_rect_fill(screen, GRAY, 0, 0, 70, 130);
        sr_rect_fill_masked(screen, WHITE, 1, 1, 100, 100);
        test(sr_get_pixel(screen, 0, 0) == map[GRAY]);
        test(sr_get_pixel(screen, 1, 1) == map[WHITE]);
        test(sr_get_pixel(screen, 100, 100) == map[BLACK]);
        test(sr_get_pixel(screen, 101, 100) == map[BLACK]);
        test(sr_get_pixel(screen, 101, 101) == map[BLACK]);
        test(sr_get_pixel(screen, 50, 50) == map[WHITE]);

        sr_fill(screen, BLACK);
        sr_rect_outline(screen, WHITE, 1, 1, 100, 100);
        test(sr_get_pixel(screen, 0, 0) == map[BLACK]);
        test(sr_get_pixel(screen, 1, 1) == map[WHITE]);
        test(sr_get_pixel(screen, 100, 100) == map[WHITE]);
        test(sr_get_pixel(screen, 101, 100) == map[BLACK]);
        test(sr_get_pixel(screen, 101, 101) == map[BLACK]);
        test(sr_get_pixel(screen, 50, 50) == map[BLACK]);
        sr_fill(screen, BLACK);
        sr_rect_fill(screen, GRAY, 0, 0, 70, 130);
        sr_rect_outline_masked(screen, WHITE, 1, 1, 100, 100);
        test(sr_get_pixel(screen, 0, 0) == map[GRAY]);
        test(sr_get_pixel(screen, 1, 1) == map[WHITE]);
        test(sr_get_pixel(screen, 100, 100) == map[BLACK]);
        test(sr_get_pixel(screen, 101, 100) == map[BLACK]);
        test(sr_get_pixel(screen, 101, 101) == map[BLACK]);
        test(sr_get_pixel(screen, 50, 50) == map[GRAY]);

    }

    // RESULT MESSAGE
    if (tests_total == tests_passed) {
        printf(GREEN "ALL %d TESTS PASSED" DEFAULT, tests_total);
        return 0;
    } else {
        printf(RED "%d/%d TESTS FAILED" DEFAULT, tests_total - tests_passed, tests_total);
        return 1;
    }

}
