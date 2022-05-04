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
            set_pixel(gray, i, j, 0, grayscale);
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
            set_pixel(im, i, j, c, get_pixel(im, i, j, c) + v);
        }
    }
}

void clamp_image(image im)
{
    int width = im.w;
    int height = im.h;

    for (int c = 0; c < im.c; c++) {
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

float set_hue(float hue_gamma) {
    return hue_gamma < 0 ? (hue_gamma / 6) + 1 : hue_gamma / 6;
}

void rgb_to_hsv(image im)
{
    int width = im.w;
    int height = im.h;
    float value, min, chroma, saturation, hue, hue_gamma;

    for (int w = 0; w < width; w++) {
        for (int h = 0; h < height; h++) {
            value = three_way_max(get_pixel(im, w, h, 0), 
                get_pixel(im, w, h, 1), get_pixel(im, w, h, 2));

            min = three_way_min(get_pixel(im, w, h, 0), 
                get_pixel(im, w, h, 1), get_pixel(im, w, h, 2));
            chroma = value - min;
            saturation = value == 0 ? 0 : chroma / value;
            
            if (chroma == 0) {
                hue = 0;
            } else if (value == get_pixel(im, w, h, 0)) {
                hue_gamma = (get_pixel(im, w, h, 1) - get_pixel(im, w, h, 2)) / chroma;
                hue = set_hue(hue_gamma);
            } else if (value == get_pixel(im, w, h, 1)) {
                hue_gamma = ((get_pixel(im, w, h, 2) - get_pixel(im, w, h, 0)) / chroma) + 2;
                hue = set_hue(hue_gamma);
            } else if (value == get_pixel(im, w, h, 2)) {
                hue_gamma = ((get_pixel(im, w, h, 0) - get_pixel(im, w, h, 1)) / chroma) + 4;
                hue = set_hue(hue_gamma);
            }

            set_pixel(im, w, h, 0, hue);
            set_pixel(im, w, h, 1, saturation);
            set_pixel(im, w, h, 2, value);
        }
    }

}

// https://en.wikipedia.org/wiki/HSL_and_HSV#HSV_to_RGB
void hsv_to_rgb(image im)
{
    int width = im.w;
    int height = im.h;

    float hue, saturation, value, chroma, 
        hue_gamma, red_temp, green_temp, blue_temp, x, m;

    for (int w = 0; w < width; w++) {
        for (int h = 0; h < height; h++) {
            hue = get_pixel(im, w, h, 0);
            saturation = get_pixel(im, w, h, 1);
            value = get_pixel(im, w, h, 2);

            chroma = saturation * value;

            hue_gamma = hue * 6;
            if (hue_gamma < 0) hue_gamma -= 1;

            x = chroma * (1 - fabs(fmod(hue_gamma, 2) - 1));
            if (w == 0 && h == 0) printf("test: %f\n", x);

            if (hue_gamma <= 0 && hue_gamma < 1) {
                red_temp = chroma;
                green_temp = x;
                blue_temp = 0;
            } else if (hue_gamma <= 1 && hue_gamma < 2) {
                red_temp = x;
                green_temp = chroma;
                blue_temp = 0;
            } else if (hue_gamma <= 2 && hue_gamma < 3) {
                red_temp = 0;
                green_temp = chroma;
                blue_temp = x;
            } else if (hue_gamma <= 3 && hue_gamma < 4) {
                red_temp = 0;
                green_temp = x;
                blue_temp = chroma;
            } else if (hue_gamma <= 4 && hue_gamma < 5) {
                red_temp = x;
                green_temp = 0;
                blue_temp = chroma;
            } else if (hue_gamma <= 5 && hue_gamma < 6) {
                red_temp = chroma;
                green_temp = 0;
                blue_temp = x;
            }

            m = value - chroma;
            set_pixel(im, w, h, 0, red_temp + m);
            set_pixel(im, w, h, 1, green_temp + m);
            set_pixel(im, w, h, 2, blue_temp + m);
        }       
    }
}
