/*
	===== PRUEBA DE APLICACION DE RASTERIZACION =====
	Para poder tener mas nociones sobre el uso de la libreria
	raster.h se hizo este programa como ejemplo para poder aprender
	mas sobre como funciona cada funcion implementada, sirviendo de
	ayuda tambien para comprender las bases de rasterizacion y sobre
	como usar SVGA256.

	Si hay partes que no logra entender bien o quiere saber como funciona
	a profundidad una funcion o una sentencia por favor consulte desde
	diferentes fuentes o consulte de forma rapida con herramientas IA.

	- Samuel Rueda
*/

/*
    =======================================
    Importacion de Librerias Estandar
    =========================================
*/

#include <stdio.h>
#include <conio.h>
#include <graphics.h>
#include <stdlib.h>
#include <dos.h>

/*
    =======================================
    Importacion de Librerias Personalizadas
    =========================================
*/

#include "gphadmin.h"   /* Libreria que administra el modo grafico del sistema operativo */
#include "raster.h"   /* Libreria que contiene funciones para manejar la rasterizacion de imagenes */

/*
    =======================================
    Declaracion de Directivas
    =========================================
*/

#define WIDTH 320 		/* Ancho fijo de la pantalla */
#define HEIGHT 200		/* Alto fijo de la pantalla */

/*
    =======================================
    Programa Principal
    =========================================
*/

int main() {
	/* Declaracion de puntero a fichero (IMPORTANTE) */
    FILE *file;

	/* Iniciar modo grafico hacia BIN */
    if (iniciar_modo_svga_256("C:\\TC20\\BIN")) return EXIT_FAILURE;

    /* Poner fondo blanco */
	setfillstyle(SOLID_FILL, WHITE);
    bar(0, 0, WIDTH, HEIGHT);

	/* Abrir archivo binario */
	/* Parametros usados de fopen (que se usa para abrir ficheros)
	   "dinosaur.bin": Archivo binario que contiene informacion sobre cada pixel.
	   "rb": Metodo para trabajar con el fichero, rb significa leer archivo binario.

		Para hacer que esto funcione debes tener ya el archivo binario dinosaur.bin
		adjunto al programa principal y moverlo a BIN para poder trabajar de forma
		correcta con el fichero, de lo contrario no se detectara y se validara.
	*/
	file = fopen("dinosaur.bin", "rb");

	/* file contendra el puntero devuelto por fopen, si esta falla se valida
	para no jugar con la memoria */
    if (validar_archivo(file)) return EXIT_FAILURE;

	/* Dibujar con rasterizado */
	/* Para establecer el fondo de pantalla utilizamos los siguientes parametros:
		file: puntero del fichero binario a dibujar
		CENTER: constante de la posicion a ubicar en la imagen
		200, 200: Dimensiones de la imagen (ancho, alto)
		WIDTH, HEIGHT: Dimensiones de la pantalla (pantalla_ancho, pantalla_alto)
	*/
	dibujar_con_rasterizado_pos(file, CENTER, 200, 200, WIDTH, HEIGHT);

	/* Cerrar Archivo para ahorrar buffer */
    fclose(file);

    /* Cerrar modo grafico */
    getch_closegraph();
    return 0;
}