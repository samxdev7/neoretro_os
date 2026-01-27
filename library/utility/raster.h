/* RASTER.H - Images Rasterization Tool */
#ifndef RASTER_H
#define RASTER_H

/*
 * This library responds to a necessity that would be very useful
 * for this OS. It is the possibility of putting pictures, and
 * that is possible with rasterization.
 * 
 * However, you shall convert pictures into 320x200x256 bin format
 * before you use those functions, anyway all pictures must be
 * threated as a bin file with stdio.h functions.
 */

#include <stdio.h>
#include <conio.h>
#include <graphics.h>
#include <stdlib.h>
#include <dos.h>
#include <svga256.h>

typedef enum {
    TOP_LEFT, TOP_CENTER, TOP_RIGHT,            
    CENTER_LEFT, CENTER, CENTER_RIGHT,          
    BOTTOM_LEFT, BOTTOM_CENTER, BOTTOM_RIGHT
} ImagePosition;

short int verify_file(FILE *stream);
void raster_draw_coords(FILE *stream, int pos_x, int pos_y,
                        int width, int height);
void raster_draw_pos(FILE *stream, int pos, int img_width, int img_height);
void raster_draw_png_coords(FILE *stream, int pos_x, int pos_y,
                            int width, int height);

short int verify_file(FILE *stream) {
    if (!stream) {
        printf("Error: Picture could not open.\n");
        return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

void raster_draw_coords(FILE *stream, int pos_x, int pos_y, 
                        int width, int height) {
    unsigned char color;
    int x, y;

    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
            fread(&color, 1, 1, stream);
            putpixel(x + pos_x, y + pos_y, color);
        }
    }
}

void raster_draw_pos(FILE *stream, int pos, int img_width, int img_height) {
    int px, py;

    switch(pos) {
    case TOP_LEFT:
        px = 0;
        py = 0;
        break;
    case TOP_CENTER:
        px = (WIDTH - img_width) / 2;
        py = 0;
        break;
    case TOP_RIGHT:
        px = WIDTH - img_width;
        py = 0;
        break;

    case CENTER_LEFT:
        px = 0;
        py = (HEIGHT - img_height) / 2;
        break;
    case CENTER:
        px = (WIDTH - img_width) / 2;
        py = (HEIGHT - img_height) / 2;
        break;
    case CENTER_RIGHT:
        px = WIDTH - img_width;
        py = (HEIGHT - img_height) / 2;
        break;

    case BOTTOM_LEFT:
        px = 0;
        py = HEIGHT - img_height;
        break;
    case BOTTOM_CENTER:
        px = (WIDTH - img_width) / 2;
        py = HEIGHT - img_height;
        break;
    case BOTTOM_RIGHT:
        px = WIDTH - img_width;
        py = HEIGHT - img_height;
        break;
    }

	raster_draw_coords(stream, px, py, img_width, img_height);
}

/*
 * Unlike raster_draw_coords this is used for use transparent pictures,
 * so if you want to use icons in OS, using this function is recommended.
 */
void raster_draw_png_coords(FILE *stream, int pos_x, int pos_y, 
                            int width, int height) {
    unsigned char color;
    int x, y;

    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
            fread(&color, 1, 1, stream);
            if (color != 200)
                putpixel(x + pos_x, y + pos_y, color);
        }
    }
}
#endif