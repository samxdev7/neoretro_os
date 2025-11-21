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

typedef int bool;
#define true 1
#define false 0

typedef enum {
	NOTEPAD = 1,
	SHELL = 2,
	CALCULATOR = 3,
	PAINT = 4,
	CONFIG = 5,
	SHUT_DOWN = 6,
	RESTART = 7,
	LOCK = 8,
	CREDITS = 9
}iconos_t;

/* Declaracion de puntero a fichero (IMPORTANTE) */
FILE *file;

/* prototipado */
void iniciar_modo_grafico();
void notepad_icon();
void config_icon();
void calculator_icon();
void paint_icon();
void shell_icon();
void credits_icon();
void restart_icon();
void shutdown_icon();
void lock_icon();


/*
    =======================================
    Programa Principal
    =========================================
*/

int main() {
	
	bool valido = true;
	int opcion;

	do {
		clrscr();
		printf("\n");
		printf("===== Iconos =====\n");
		printf("(1) Icono notepad\n");
		printf("(2) Icono CLI\n");
		printf("(3) Icono calculator\n");
		printf("(4) Icono paint\n");
		printf("(5) Icono config\n");
		printf("(6) Icono apagar\n");
		printf("(7) Icono reiniciar\n");
		printf("(8) Icono suspender\n");
		printf("(9) Icono creditos\n");
		printf("(10) SALIR\n");
		printf("Elija una opcion para continuar: ");
		scanf("%d", &opcion);

		switch(opcion)
		{
			case NOTEPAD:
			{
				iniciar_modo_grafico();
				notepad_icon();
				getch();
				getch_closegraph();
				break;
			}
			case SHELL:
			{
				iniciar_modo_grafico();
				shell_icon();
				getch();
				getch_closegraph();
				break;
			}
			case CALCULATOR:
			{
				iniciar_modo_grafico();
				calculator_icon();
				getch();
				getch_closegraph();
				break;
			}
			case PAINT:
			{
				iniciar_modo_grafico();
				paint_icon();
				getch();
				getch_closegraph();
				break;
			}
			case CONFIG:
			{
				iniciar_modo_grafico();
				config_icon();
				getch();
				getch_closegraph();
				break;
			}
			case SHUT_DOWN:
			{
				iniciar_modo_grafico();
				setfillstyle(SOLID_FILL, BLACK);
    			bar(0, 0, WIDTH, HEIGHT);
				shutdown_icon();
				getch();
				getch_closegraph();
				break;
			}
			case RESTART:
			{
				iniciar_modo_grafico();
				setfillstyle(SOLID_FILL, BLACK);
    			bar(0, 0, WIDTH, HEIGHT);
				restart_icon();
				getch();
				getch_closegraph();
				break;
			}
			case LOCK:
			{
				iniciar_modo_grafico();
				lock_icon();
				getch();
				getch_closegraph();
				break;
			}
			case CREDITS:
			{
				iniciar_modo_grafico();
				credits_icon();
				getch();
				getch_closegraph();
				break;
			}
			case 10:
			{
				valido = false;
				break;
			}
		}
	}while(valido);
    return EXIT_SUCCESS;
}

void iniciar_modo_grafico()
{
	/* Iniciar modo grafico hacia BIN */
    if (iniciar_modo_svga_256("C:\\TC20\\BIN")) return EXIT_FAILURE;

    /* Poner fondo blanco */
	setfillstyle(SOLID_FILL, WHITE);
    bar(0, 0, WIDTH, HEIGHT);
}

void dibujar_raster_png_coords(FILE *archivo, int pos_x, int pos_y, int ancho, int alto)
{
    unsigned char color; /* Asignar el color de cada pixel */
    int x, y; /* Variables de Recorrido de Cada Pixel */

    /* Leer y dibujar pixeles */
    for (y = 0; y < alto; y++) {
        for (x = 0; x < ancho; x++) {
            fread(&color, 1, 1, archivo);
            putpixel(x + pos_x, y + pos_y, color);
        }
    }
}

void notepad_icon()
{
	file = fopen("notepad.bin", "rb");

	if (validar_archivo(file)) return EXIT_FAILURE;

	dibujar_raster_png_coords(file, CENTER, CENTER, 32, 32);

	/* Cerrar Archivo para ahorrar buffer */
	fclose(file);
}

void config_icon()
{
	file = fopen("config.bin", "rb");

	if (validar_archivo(file)) return EXIT_FAILURE;

	dibujar_raster_png_coords(file, CENTER, CENTER, 32, 32);

	/* Cerrar Archivo para ahorrar buffer */
	fclose(file);
}

void calculator_icon()
{
	file = fopen("calcIco.bin", "rb");

	if (validar_archivo(file)) return EXIT_FAILURE;

	dibujar_raster_png_coords(file, CENTER, CENTER, 64, 64);

	/* Cerrar Archivo para ahorrar buffer */
	fclose(file);
}

void paint_icon()
{
	file = fopen("paint.bin", "rb");

	if (validar_archivo(file)) return EXIT_FAILURE;

	dibujar_raster_png_coords(file, CENTER, CENTER, 32, 32);
	/* Cerrar Archivo para ahorrar buffer */
	fclose(file);
}

void shell_icon()
{
	file = fopen("shell.bin", "rb");

	if (validar_archivo(file)) return EXIT_FAILURE;

	dibujar_raster_png_coords(file, CENTER, CENTER, 32, 32);
	/* Cerrar Archivo para ahorrar buffer */
	fclose(file);
}

void credits_icon()
{
	file = fopen("creditos.bin", "rb");

	if (validar_archivo(file)) return EXIT_FAILURE;

	dibujar_raster_png_coords(file, CENTER, CENTER, 64, 64);
	/* Cerrar Archivo para ahorrar buffer */
	fclose(file);
}
void restart_icon()
{
	file = fopen("restart.bin", "rb");

	if (validar_archivo(file)) return EXIT_FAILURE;

	dibujar_raster_png_coords(file, CENTER, CENTER, 64, 64);
	/* Cerrar Archivo para ahorrar buffer */
	fclose(file);
}
void shutdown_icon()
{
	file = fopen("taskbar.bin", "rb");

	if (validar_archivo(file)) return EXIT_FAILURE;

	dibujar_raster_png_coords(file, CENTER, CENTER, 64, 64);
	/* Cerrar Archivo para ahorrar buffer */
	fclose(file);
}
void lock_icon()
{
	file = fopen("lock.bin", "rb");

	if (validar_archivo(file)) return EXIT_FAILURE;

	dibujar_raster_png_coords(file, CENTER, CENTER, 64, 64);
	/* Cerrar Archivo para ahorrar buffer */
	fclose(file);
}

