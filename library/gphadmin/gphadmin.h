/* GPHAdmin - Handle Graphic Settings */
#ifndef GPHADMIN_H
#define GPHADMIN_H

#include <graphics.h>
#include <conio.h>
#include <svga256.h>

#define WIDTH  320		
#define HEIGHT 200

/*
 * This enum structure is used for categorize basic booting actions for SO
 * (well, are simulated), used mainly at initial loading and desktop.
 */
typedef enum {
    NONE_ACTION,
    SHUT_DOWN_ACTION,
    RESTART_ACTION,
    SUSPEND_ACTION
} BootManageOS;

short start_svga_256_mode(char *dir);
void set_text_config_so(unsigned char color);
void set_bg(unsigned char color);

short start_svga_256_mode(char *dir)
{
	int gd = DETECT, gm;
	installuserdriver("Svga256", detect_svga_256);
    initgraph(&gd, &gm, dir);
	cleardevice(); 		 
	return EXIT_SUCCESS;
}

void set_text_config_so(unsigned char color)
{
    setcolor(color);
    settextstyle(SMALL_FONT, HORIZ_DIR, 4);
}

/* 
 * It is used this function by fixing setbkcolor main problem.
 * This consists of not giving black color correctly with that function.
 */
void set_bg(unsigned char color)
{
    setfillstyle(SOLID_FILL, color);
    bar(0, 0, WIDTH, HEIGHT);
}
#endif /* GPHAdmin.h */