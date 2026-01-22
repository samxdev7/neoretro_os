/* GPHAdmin - Handle Graphic Settings */
#ifndef GPHADMIN_H
#define GPHADMIN_H


#include <graphics.h>
#include <conio.h>


#include "svga256.h"


#define WIDTH  320		
#define HEIGHT 200


/*
    enum BootManageOS
    Estructura enum encargada de categorizar las diferentes acciones de arranque 
    simuladas del sistema operativo, utilizada en la carga inicial y
    en el escritorio.
*/
typedef enum {
    ACCION_NINGUNO,
    ACCION_APAGAR,
    ACCION_REINICIAR,
    ACCION_SUSPENDER
} BootManageOS;


short iniciar_modo_svga_256(char *dir);
void getch_closegraph(void);
void colocar_configuraciones(void);
void set_bg(unsigned char color);


short iniciar_modo_svga_256(char *dir)
{
	int gd = DETECT, gm;
	installuserdriver("Svga256", detectar_svga_256);
    initgraph(&gd, &gm, dir);
	cleardevice(); 		 
	return EXIT_SUCCESS;
}

/*
    colocar_configuraciones()
    Esta funcion coloca las configuraciones del color de texto y el estilo de texto
    actual para ser utilizado por las funciones de graphics.h.
*/
void colocar_configuraciones(void)
{
    setcolor(BLACK);
    settextstyle(SMALL_FONT, HORIZ_DIR, 4);
}

/* 
    set_bg()
    - Funcion que cambia el color de fondo de la pantalla, resolviendo la
    problematica de setbkcolor de graphics.h.
*/
void set_bg(unsigned char color)
{
    /* Definir color y fuente de letra */
    setfillstyle(SOLID_FILL, color);
    bar(0, 0, WIDTH, HEIGHT);
}
#endif
