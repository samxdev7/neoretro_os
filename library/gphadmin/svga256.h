/************************************************/
/* 						                        */
/*       SuperVGA 256 BGI driver defines    	*/
/*		Copyright (c) 1991	                	*/
/*	    Jordan Hargraphix Software	        	*/
/*					                        	*/
/************************************************/

#ifndef __SVGA256_H__
#define __SVGA256_H__

extern int far _Cdecl Svga256_fdriver[];

/* These are the currently supported modes */
#define	SVGA320x200x256		0	/* 320x200x256 Standard VGA */

/* Detect SVGA 256 Mode */
int huge detectar_svga_256(void);
int huge detectar_svga_256(void)
{
    return SVGA320x200x256; /* Case SVGA 320x200x256 */
}

#endif /* __SVGA256_H__ */
