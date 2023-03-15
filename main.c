#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>

#define WIDTH 20
#define HEIGHT 20
#define PPM_SCALER 50

typedef float layer_t[HEIGHT][WIDTH];

static inline int clampi(int x, int low, int high)
{
    if (x < low) x = low;
    if (x > high) x = high;

    return x;
}

void layer_fill_rect(layer_t layer, int x, int y, int w, int h, float value)
{
    assert(w > 0);
    assert(h > 0);

    int x0 = clampi(x, 0, WIDTH - 1);
    int y0 = clampi(y, 0, HEIGHT - 1);
    int x1 = clampi(x0 + w - 1, 0, WIDTH - 1);
    int y1 = clampi(y0 + w - 1, 0, HEIGHT - 1);

    for (int y = y0; y <= y1; ++y) {
        for (int x = x0; x <= x1; ++x) {
            layer[y][x] = value;
        }
    }
}

void layer_save_as_ppm(layer_t layer, const char* file_path)
{
    FILE *f = fopen(file_path, "wb");

    if (f == NULL) {
        fprintf(stderr, "ERROR: could not open file %s: %m\n", file_path);
        exit(1);
    }

    fprintf(f, "P6\n%d %d 255\n", WIDTH * PPM_SCALER, HEIGHT * PPM_SCALER);

    for (int y = 0; y < HEIGHT * PPM_SCALER; ++y) {
        for (int x = 0; x < WIDTH * PPM_SCALER; ++x) {
            float s = layer[y / PPM_SCALER][x / PPM_SCALER];

            char pixel[3] = {
                (char) floorf(255 * s),
                0,
                0
            };

            fwrite(pixel, sizeof(pixel), 1, f);
        }
    }

    fclose(f);
}

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
    layer_fill_rect(inputs, 0, 0, WIDTH / 2, HEIGHT / 2, 1.0f);
    layer_save_as_ppm(inputs, "inputs.ppm");

    float output = feed_forward(inputs, weights);
    printf("output = %f\n", output);

    return 0;
}

