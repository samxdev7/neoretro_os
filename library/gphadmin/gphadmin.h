#ifndef GPHADMIN_H
#define GPHADMIN_H

/*
	GHPADMIN_H
	Libreria usada para administar el modo grafico de TC20, esta vez
	para el modo SVGA 320x200x256.
	
	- Moises Aleman
	- Samuel Rueda
*/

/*
    =======================================
    Importacion de Librerias Estandar
    =========================================
*/
#include <graphics.h>
#include <conio.h>

/*
    =======================================
    Importacion de Librerias Personalizadas
    =========================================
*/
#include "svga256.h" 	/* Libreria de constantes de resoluciones con 256 colores */
#include "validgph.h"   /* Libreria validadora del modo grafico */

/*
    =======================================
    Declaracion de Directivas
    =========================================
*/
#define WIDTH  320 		/* Ancho fijo de la pantalla */
#define HEIGHT 200		/* Alto fijo de la pantalla */

/*
    =======================================
    Definicion de Enums
    =========================================
*/
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

/*
    =======================================
    Declaracion de Prototipos
    =========================================
*/
short iniciar_modo_svga_256(char *dir);
void getch_closegraph(void);
void colocar_configuraciones(void);
void set_bg(unsigned char color);

/*
    =======================================
    Inicializacion de Funciones
    =========================================
*/

/* 
    iniciar_modo_svga_256()
    - Funcion que inicia el modo grafico SVGA256.
    
    Parametros:
    - char *dir: Cadena que indica el directorio donde esta guardado el driver
    svga256.bgi para usar el modo 320x200x256.
*/
short iniciar_modo_svga_256(char *dir)
{
	/* Declaracion e instalacion de modo 320x200x256 */
	int gd = DETECT, gm;

	/* A traves de la sentencia installuserdriver se esta importando un
	driver que no esta definido en la libreria estandar graphics.h, para
	este caso usaremos SVGA 256, ingresamos los siguientes parametros para
	importar este driver:
		- "Svga256": Nombre del modo grafico a usar
		- detectar_svga_256: Puntero a funcion que devuelve 320x200x256
	*/
	int driver = installuserdriver("Svga256", detectar_svga_256);

    /* Validar driver instalado */
    if (verificar_instalacion_driver(driver)) return EXIT_FAILURE;

    /* Inicializar modo grafico */
    initgraph(&gd, &gm, dir);

    /* Verificar si se inicializo correctamente el modo grafico */
	if (verificar_inicializacion_driver()) return EXIT_FAILURE;

    /* Verificar soporte de 256 colores */
	if (verificar_soporte_colores()) return EXIT_FAILURE;

	cleardevice(); 		 /* Limpieza de pantalla antes de usar el modo grafico */
	return EXIT_SUCCESS; /* Si todo sale correctamente, se puede trabajar adecuadamente con el modo grafico */
}

/* 
    iniciar_modo_svga_256()
    - Funcion que cierra el modo grafico con getch + closegraph.
*/
void getch_closegraph(void)
{ getch(); closegraph(); }

/* 
    colocar_configuraciones()
    - Funcion que cambia las configuraciones del sistema operativo, ya sea
    el color de la letra, y la fuente.
*/
void colocar_configuraciones(void)
{
    /* Definir color y fuente de letra */
    setcolor(BLACK);
    settextstyle(SMALL_FONT, HORIZ_DIR, 4);
}

/* 
    set_bg()
    - Funcion que cambia el color de fondo de la pantalla, resolviendo la
    problematica de setbkcolor de graphics.h.

    Parametros:
    - unsigned char color: Indica el color a cambiar del fondo de la pantalla.
*/
void set_bg(unsigned char color)
{
    /* Definir color y fuente de letra */
    setfillstyle(SOLID_FILL, color);
    bar(0, 0, WIDTH, HEIGHT);
}
#endif
