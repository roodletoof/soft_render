#include <stdio.h>
#include <stdbool.h>

#define SR_IMPLEMENTATION
#include "sr.h"

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
        printf(GREEN " SUCCESS " DEFAULT "%s" "\n", condition_text);
        tests_passed++;
        result = result_success;
    } else {
        printf(RED " FAILURE " DEFAULT "%s" " " "%s" ":%d\n", condition_text, file, line);
        result = result_failure;
    }
    fflush(stdout);
    return result;
}

#define test(condition) test(condition, #condition, __FILE__, __LINE__)

int main() {

    { // TEST
        sr_surface *screen = &sr_surface_value(128, 128);

        sr_circle_fill(screen, 1, 64, 64, 64);
        test(sr_get_pixel(screen, 64, 64) == 1);
        test(sr_get_pixel(screen, 64+63, 64) == 1);
        test(sr_get_pixel(screen, 64, 64+63) == 1);
        test(sr_get_pixel(screen, 64+64, 64) == 0);
        test(sr_get_pixel(screen, 64, 64+64) == 0);
        test(sr_get_pixel(screen, 128, 128) == 0);

        sr_fill(screen, 0);

        sr_circle_outline(screen, 1, 64, 64, 64);
        test(sr_get_pixel(screen, 64, 64) == 0);
        test(sr_get_pixel(screen, 64+63, 64) == 1);
        test(sr_get_pixel(screen, 64, 64+63) == 1);
        test(sr_get_pixel(screen, 64+64, 64) == 0);
        test(sr_get_pixel(screen, 64, 64+64) == 0);
        test(sr_get_pixel(screen, 128, 128) == 0);

    }

    // RESULT MESSAGE
    if (tests_total == tests_passed) {
        printf(GREEN "ALL TESTS PASSED" DEFAULT);
        return 0;
    } else {
        printf(RED "SOME TESTS FAILED" DEFAULT);
        return 1;
    }
}
