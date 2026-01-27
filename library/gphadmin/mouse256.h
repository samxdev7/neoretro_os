/* Mouse256.h - Mouse for 300x200x256 resolution */
#ifndef MOUSE256_H
#define MOUSE256_H

#include <dos.h>

union REGS reg;

int mtest(void)
{
    reg.x.ax = 0x0;
    int86(0x33, &reg, &reg);
    if (reg.x.ax > 0) return reg.x.bx;
    else return 0;
}

void mshow(void)
{
    reg.x.ax = 0x1;
    int86(0x33, &reg, &reg);
}

void mhide(void)
{
    reg.x.ax = 0x2;
    int86(0x33, &reg, &reg);
}

int mxpos(void)
{
    reg.x.ax = 0x3;
    int86(0x33, &reg, &reg);
    return reg.x.cx / 2;  
}

int mypos(void)
{
    reg.x.ax = 0x3;
    int86(0x33, &reg, &reg);
    return reg.x.dx;
}

void mplace(int x, int y)
{
    reg.x.ax = 0x4;
    reg.x.cx = x * 2;           
    reg.x.dx = y;    
    int86(0x33, &reg, &reg);
}

int mclick(void)
{
	int r = 0;
	reg.x.ax = 0x5;
	reg.x.bx = 0;
	int86(0x33, &reg, &reg);
	if ((reg.x.ax) & 1) r = 1;
	else if ((reg.x.ax>>1) & 1) r = 2;
	return r;
}

/*
 * This function has a different behavior than mclick, because
 * its created by response a left/right click once, after
 * that, the mouse stays unfunctional at least you let go the mouse
 * or do other click.
 */
int mclick_p(void) {
    static int prev_state = 0;
    int curr_state = 0;
    int r = 0;
    
    reg.x.ax = 0x5;
    reg.x.bx = 0;
    int86(0x33, &reg, &reg);
    
    if ((reg.x.ax) & 1) curr_state = 1;
    else if ((reg.x.ax>>1) & 1) curr_state = 2;
    
    if (curr_state != prev_state) r = curr_state;
    prev_state = curr_state;

    return r;
}

void mlimit(int x1, int y1, int x2, int y2)
{
    reg.x.ax = 0x7;
    reg.x.cx = x1 * 2;         
    reg.x.dx = x2 * 2;
    int86(0x33, &reg, &reg);
    
    reg.x.ax = 0x8;
    reg.x.cx = y1;  
    reg.x.dx = y2;
    int86(0x33, &reg, &reg);
}

int minlimit(int x1, int y1, int x2, int y2) {
    int mx = mxpos(), my = mypos();
    return (mx>=x1) && (mx<=x2) && (my>=y1) && (my<=y2);
}
#endif