#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <float.h>

#define WIDTH 1600
#define HEIGHT 800
#define abs(value) (value<0?value*-1:value)

typedef struct Color {
    int r;
    int g;
    int b;
} Color;

typedef Color Canvas[HEIGHT][WIDTH];

static Canvas canvas;

void saveAsPPM(const char *filepath)
{
    FILE *file = fopen(filepath, "wb");
    if(!file) exit(1);

    fprintf(file, "P6\n %i %i 255\n", WIDTH, HEIGHT);

    int y; for(y = 0; y < HEIGHT; ++y) {
        int x; for(x = 0; x < WIDTH; ++x) {
            Color pixel = canvas[y][x];
            uint8_t bytes[3] = {
                pixel.r,
                pixel.g,
                pixel.b
            };
            fwrite(bytes, sizeof(bytes), 1, file);
        }
    }
}

void fill_canvas(Color color)
{
    int y; for(y = 0; y < HEIGHT; ++y) {
        int x; for(x = 0; x < WIDTH; ++x) {
            canvas[y][x] = color;
        }
    }
}

double map(int num, int s, int e, int m, int n)
{
    return (num-s)/(double)(e-s) * (n-m) + m;
}

void render_julia_set()
{
    int iterations = 1000;
    int y; for(y = 0; y < HEIGHT; ++y) {
        int x; for(x = 0; x < WIDTH; ++x) {
            double a = map(x, 0, WIDTH, -2, 2);
            double b = map(y, 0, HEIGHT, -1, 1);

            int count = 0;
            double ca = -0.8;
            double cb = 0.156;

            while(count < iterations) {
                double aa = a*a - b*b;
                double bb = 2 * a * b;

                a = aa + ca;
                b = bb + cb;

                if(a + b > 8) break;
                count++;
            }
            Color pcolor;
            if(count == iterations) pcolor = (Color){0, 0, 0};
            else pcolor = (Color){
                        (int) map(count, 0, 200, 0, 255),
                        (int) map(count, 0, 200, 0, 255),
                        (int) map(count, 0, 200, 0, 255)
                    };
            canvas[y][x] = pcolor;
        }
    }
}

int main(void)
{
    fill_canvas((Color){18, 18, 18});
    render_julia_set();
    saveAsPPM("canvas.ppm");
    return 0;
}
