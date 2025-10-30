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
    Declaracion de Prototipos
    =========================================
*/
short int iniciar_modo_svga_256(char *dir);
void getch_closegraph(void);

/*
    =======================================
    Inicializacion de Funciones
    =========================================
*/

/* Funcion iniciar modo grafico  */
short int iniciar_modo_svga_256(char *dir)
{
	/* Declaracion e instalacion de modo 320x200x256 */
	int gd = DETECT, gm;

	/* A traves de la sentencia installuserdriver se esta importando un
	driver que no esta definido en la libreria estandar graphics.h, para
	este caso usaremos SVGA 256, ingresamos los siguientes parametros para
	importar este driver:
		- "Svga256": Nombre del modo grafico a usar
		- detectar_vga_256: Puntero a funcion que devuelve 320x200x256
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

/* Funcion cerrar modo grafico */
void getch_closegraph(void)
{
	getch();
	closegraph();
}
#endif
