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
    image gray = make_image(im.w, im.h, 1);

    int width = im.w;
    int height = im.h;

    for (int i = 0; i <= width; i++) {
        for (int j = 0; j <= height; j++) {
            float grayscale = (0.299 * get_pixel(im, i, j, 0)) 
                + (0.587 * get_pixel(im, i, j, 1)) 
                + (0.114 * get_pixel(im, i, j, 2));
            gray.data[i + j * width] = grayscale;
        }
    }

    return gray;
}

void shift_image(image im, int c, float v)
{
    
}

void clamp_image(image im)
{
    // TODO Fill this in
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
    // TODO Fill this in
}

void hsv_to_rgb(image im)
{
    // TODO Fill this in
}
