#include <stdio.h>

#define WIDTH 20
#define HEIGHT 20

typedef float layer_t[HEIGHT][WIDTH];

float feed_forward(layer_t inputs, layer_t weights)
{
    float output = 0.0f;

    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            output += inputs[y][x] * weights[y][x];
        }
    }

    return output;
}

static layer_t inputs;
static layer_t weights;

int main(void)
{
    float output = feed_forward(inputs, weights);
    printf("output = %f\n", output);

    return 0;
}

