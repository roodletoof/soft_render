#define WINDOW_TITLE "Game"
#define WINDOW_WIDTH 256
#define WINDOW_HEIGHT 256
#define C8_IMPLEMENTATION
#include "c8.h"

#include <math.h>

static struct {
    struct {
        float x, y;
        const float speed;
    } Player;
} State = {
        .Player = {
                .speed = 128
        }
};

void init() {
    State.Player.x = 0.0f;
    State.Player.y = 0.0f;
}

static float t = 0.0f;

void update(float dt, c8_input const * input) {

    t += dt;
    printf("time %f\n", t);

    float dir_x = 0.0f;
    float dir_y = 0.0f;
    if (input->d & c8_input_down) dir_x += 1.0f;
    if (input->a & c8_input_down) dir_x -= 1.0f;
    if (input->s & c8_input_down) dir_y += 1.0f;
    if (input->w & c8_input_down) dir_y -= 1.0f;

    float f = 1.0f/ sqrtf(dir_x * dir_x + dir_y * dir_y);
    if (f != INFINITY) {
        dir_x *= f;
        dir_y *= f;
    }

    State.Player.x += dir_x * State.Player.speed * dt;
    State.Player.y += dir_y * State.Player.speed * dt;

    if (input->e & c8_input_just_released) {
        c8_exit();
    }
}

void draw(c8_surface *screen, c8_input const * input) {
    c8_pal(0, 1);
    c8_pal(7,0);
    c8_fill(screen, 0);
    c8_pal_reset();
    c8_circle_fill(screen, 7, (int) State.Player.x, (int) State.Player.y, 20);
}

int main() {
    init();
    return c8_launch(update, draw);
}
