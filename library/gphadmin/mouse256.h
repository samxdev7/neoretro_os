#ifndef MOUSE256_H
#define MOUSE256_H

#include <dos.h>

union REGS reg;

/*************************************************/
/* mtest()                                       */
/*************************************************/
int mtest(void)
{
    reg.x.ax = 0x0;
    int86(0x33, &reg, &reg);
    if (reg.x.ax > 0) return reg.x.bx;
    else return 0;
}

/*************************************************/
/* mver()                                        */
/*************************************************/
void mver(void)
{
    reg.x.ax = 0x1;
    int86(0x33, &reg, &reg);
}

/*************************************************/
/* mocultar()                                    */
/*************************************************/
void mocultar(void)
{
    reg.x.ax = 0x2;
    int86(0x33, &reg, &reg);
}

/*************************************************/
/* mxpos() 				                         */
/*************************************************/
int mxpos(void)
{
    reg.x.ax = 0x3;
    int86(0x33, &reg, &reg);
    return reg.x.cx / 2;  
}

/*************************************************/
/* mypos() 										 */
/*************************************************/
int mypos(void)
{
    reg.x.ax = 0x3;
    int86(0x33, &reg, &reg);
    return reg.x.dx;
}

/*************************************************/
/* msituar() 			                         */
/*************************************************/
void msituar(int x, int y)
{
    reg.x.ax = 0x4;
    reg.x.cx = x * 2;           
    reg.x.dx = y;    
    int86(0x33, &reg, &reg);
}

/*************************************************/
/* mclick()                                      */
/*************************************************/
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

/*************************************************/
/* mlimit() 					                 */
/*************************************************/
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

/*************************************************/
/* minlimit()                                    */
/*************************************************/
int minlimit(int x1, int y1, int x2, int y2)
{
    if((mxpos()>=x1) &&
        (mxpos()<=x2) &&
        (mypos()>=y1) &&
        (mypos()<=y2))
        return 1;
    return 0;
}

/*************************************************/
/* mclick() Modificado                           */
/*************************************************/
int mclick_M(void)  /* click con flanco */
{
    static int previous_state = 0;
    int actual_state;
    int r = 0;
    
    reg.x.ax = 0x5;
    reg.x.bx = 0;
    int86(0x33, &reg, &reg);
    actual_state = 0;
    
    if ((reg.x.ax) & 1) actual_state = 1;
    else if ((reg.x.ax>>1) & 1) actual_state = 2;
    
    if (actual_state == 1 && previous_state == 0) {
        r = 1;
    }
    previous_state = actual_state;
    return r;
}
#endif
