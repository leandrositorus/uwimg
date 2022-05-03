#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "image.h"

float get_pixel(image im, int x, int y, int c)
{
    int column;
    int row;

    if (x < 0) {
        column = 0;
    } else if (x > im.w) {
        column = im.w;
    } else {
        column = x;
    }

    if (y < 0) {
        row = 0;
    } else if (y > im.h) {
        row = im.h;
    } else {
        row = y;
    }

    return im.data[column + row * im.w + c * im.w * im.h];
}

void set_pixel(image im, int x, int y, int c, float v)
{
    im.data[x + y * im.w + c * im.w * im.h] = v;
}

image copy_image(image im)
{
    image copy = make_image(im.w, im.h, im.c);

    int im_size = im.c * im.w * im.h;
    for (int i = 0; i < im_size; i++) {
        copy.data[i] = im.data[i];
    }

    return copy;
}

image rgb_to_grayscale(image im)
{
    assert(im.c == 3);

    int width = im.w;
    int height = im.h;

    image gray = make_image(im.w, im.h, 1);

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            float grayscale = (0.299 * get_pixel(im, i, j, 0)) 
                + (0.587 * get_pixel(im, i, j, 1)) 
                + (0.114 * get_pixel(im, i, j, 2));
            gray.data[i + width * j] = grayscale;
        }
    }

    return gray;
}

void shift_image(image im, int c, float v)
{
    int width = im.w;
    int height = im.h;

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            im.data[i + j * width + c * width * height] +=  v;
        }
    }
}

void clamp_image(image im)
{
    int width = im.w;
    int height = im.h;


    for (int c = 0; c <= im.c; c++) {
        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {
                float value = get_pixel(im, i, j, c);
                if (value < 0) set_pixel(im, i, j, c, 0);
                else if (value > 1) set_pixel(im, i, j, c, 1);
            }
        }
    }


}


// These might be handy
float three_way_max(float a, float b, float c)
{
    return (a > b) ? ( (a > c) ? a : c) : ( (b > c) ? b : c) ;
}

float three_way_min(float a, float b, float c)
{
    return (a < b) ? ( (a < c) ? a : c) : ( (b < c) ? b : c) ;
}

void rgb_to_hsv(image im)
{
    int width = im.w;
    int height = im.h;

    for (int w = 0; w <= width; w++) {
        for (int h = 0; h <= height; h++) {
            // value
            float value = three_way_max(get_pixel(im, w, h, 0), 
                get_pixel(im, w, h, 1), get_pixel(im, w, h, 2));
            if (w == 0 && h == 0) printf("value: %f\n", value);
            
            // saturation
            float min = three_way_min(get_pixel(im, w, h, 0), 
                get_pixel(im, w, h, 1), get_pixel(im, w, h, 2));
            if (w == 0 && h == 0) printf("min: %f\n", min);
            float chroma = value - min;
            float saturation = value == 0 ? 0 : chroma / value;
            
            // hue
            float hue_gamma;
            float hue;

            if (chroma == 0) {
                if (w == 0 && h == 0) printf("test\n");
                hue = 0;
            } else if (value == get_pixel(im, w, h, 0)) {
                hue_gamma = (get_pixel(im, w, h, 1) - get_pixel(im, w, h, 2)) / chroma;
                if (w == 0 && h == 0) printf("hue_gamma_0: %f\n", hue_gamma);
            } else if (value == get_pixel(im, w, h, 1)) {
                hue_gamma = ((get_pixel(im, w, h, 2) - get_pixel(im, w, h, 0)) / chroma) + 2;
                if (w == 0 && h == 0)printf("hue_gamma_1: %f\n", hue_gamma);
            } else if (value == get_pixel(im, w, h, 1)) {
                hue_gamma = ((get_pixel(im, w, h, 0) - get_pixel(im, w, h, 1)) / chroma) + 4;
                if (w == 0 && h == 0) printf("hue_gamma_2: %f\n", hue_gamma);
            }

            if (hue_gamma < 0) {
                hue = (hue_gamma / 6) + 1;
            } else {
                hue = hue_gamma / 6;
            }

            if (w == 0 && h == 1) {
                printf("hue_gamma: %f\n", hue_gamma);
                printf("hue: %f\n", hue);
            };

            set_pixel(im, w, h, 0, hue);
            set_pixel(im, w, h, 1, saturation);
            set_pixel(im, w, h, 2, value);

        }
    }

}

void hsv_to_rgb(image im)
{
    // TODO Fill this in
}
