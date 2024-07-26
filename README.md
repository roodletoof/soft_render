# What is this?
A single-header software rendering library.
You create sr_surface types which you can draw bytes to.
The simple circle- line- and rectangle-drawing function's assume a single pixel to be represented by a single byte.
How that byte is visually interpreted is up to you to decide and implement.
A common approach might be to have the byte represent an index into a palette.

# Using the library
Add the header to your project, and include it wherever it is needed.
In exactly one of the source files; define SR_IMPLEMENTATION

Example:
```c
#define SR_IMPLEMENTATION
#include "sr.h"

#define WIDTH 256
#define HEIGHT 256

#define WHITE 255
#define BLACK 0

int main() {
    sr_surface *screen = &sr_surface_value(WIDTH, HEIGHT);
    sr_fill(screen, BLACK);

    int x = 127;
    int y = 127;
    int radius = 64;

    sr_circle_fill(screen, WHITE, x, y, radius);
}
```

![example_image](example.ppm)
