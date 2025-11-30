#ifndef RASTER_H
#define RASTER_H

/*
	RASTER_H
	Libreria encargada de trabajar con imagenes rasterizadas a traves
	de archivos binarios, util para poder mostrar en pantalla imagenes
	detallas. Esta libreria destaca por lo siguiente:

	- Manejo de ficheros
	- Generador de paleta
	- Lectura de archivos binarios
	- Posiciones para rasterizar
	- EFICIENCIA EXTREMA DE DIBUJADO!!!!!

    - Johanna Castellon
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

#include "svga256.h" /* Libreria de constantes de resoluciones con 256 colores */

/*
    =======================================
    Definci�n de Estructras
    =========================================
*/

/*
    Enum ImagePosition
    - Estructura enum que contiene el indice de las diferentes posiciones de una imagen
    en pantalla por predeterminado
*/
typedef enum {
    TOP_LEFT, TOP_CENTER, TOP_RIGHT,            /* Posiciones superiores 0 - 2 */
    CENTER_LEFT, CENTER, CENTER_RIGHT,          /* Posiciones centro     3 - 5 */
    BOTTOM_LEFT, BOTTOM_CENTER, BOTTOM_RIGHT    /* Posiciones inferiores 6 - 8 */
} ImagePosition;

/*
    =======================================
    Declaracion de Prototipos
    =========================================
*/

short int validar_archivo(FILE *archivo);
void dibujar_con_rasterizado_coords(FILE *archivo, int pos_x, int pos_y, int ancho, int alto);
void dibujar_con_rasterizado_pos(FILE *archivo, int pos, int img_ancho, int img_alto, int scr_ancho, int scr_alto);
void dibujar_raster_png_coords(FILE *archivo, int pos_x, int pos_y, int ancho, int alto);

/*
    =======================================
    Inicializacion de Funciones
    =========================================
*/

/*
    validar_archivo()
    - Funcion que verifica si un archivo fue abierto correctamente
    - Si el archivo es NULL, muestra un mensaje de error y retorna EXIT_FAILURE
    - OJO: Esta validacion previene errores al intentar operar con archivos no existentes
*/
short int validar_archivo(FILE *archivo)
{
    if (!archivo) {
        printf("Error: No se pudo abrir el archivo.\n");
        getch();
        return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

/*
    dibujar_con_rasterizado_coords()
    - Funcion que lee un archivo binario y dibuja pixel por pixel en modo grafico
    - Lee secuencialmente cada byte del archivo que representa un color de pixel
    - Utiliza putpixel() para dibujar cada pixel en las coordenadas correspondientes
    - Este realiza los dibujados a traves de un vector que conecta con el origen y el primer
    pixel del dibujo rasterizado. ( Si quieres usar posiciones predeterminadas use
    dibujar_con_rasterizado_pos() )
    - OJO: El archivo debe contener exactamente ancho*alto bytes para una representacion correcta
*/
void dibujar_con_rasterizado_coords(FILE *archivo, int pos_x, int pos_y, int ancho, int alto)
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

/*
    dibujar_con_rasterizado_pos()
    - Funcion que lee un archivo binario y dibuja pixel por pixel en modo grafico
    - Lee secuencialmente cada byte del archivo que representa un color de pixel
    - Utiliza putpixel() para dibujar cada pixel en las coordenadas correspondientes
    - Este realiza los dibujados a traves de posiciones predeterminadas, la cual
    consisten en 9 posiciones diferentes que conforman las esquinas y centros
    de la pantalla. ( Si quieres usar un punto en especifico use
    dibujar_con_rasterizado_coords() )
    - OJO: El archivo debe contener exactamente ancho*alto bytes para una representacion correcta
*/
void dibujar_con_rasterizado_pos(FILE *archivo, int pos, int img_ancho, int img_alto, int scr_ancho, int scr_alto)
{
    int punto_x, punto_y;

    /* Calcular posicion segun seleccion */
    switch(pos) {
        /* PARTE SUPERIOR */
        case TOP_LEFT: /* Esquina superior izquierda */
        {
            punto_x = 0;
            punto_y = 0;
            break;
        }
        case TOP_CENTER: /* Esquina superior centro */
        {
            punto_x = (scr_ancho - img_ancho) / 2;
            punto_y = 0;
            break;
        }
        case TOP_RIGHT: /* Esquina superior derecha */
        {
            punto_x = scr_ancho - img_ancho;
            punto_y = 0;
            break;
        }

        /* PARTE CENTRO */
        case CENTER_LEFT: /* Esquina centro izquierda */
        {
            punto_x = 0;
            punto_y = (scr_alto - img_alto) / 2;
            break;
        }
        case CENTER: /* Centro de la pantalla */
        {
            punto_x = (scr_ancho - img_ancho) / 2;
            punto_y = (scr_alto - img_alto) / 2;
            break;
        }
        case CENTER_RIGHT: /* Esquina centro derecha */
        {
            punto_x = scr_ancho - img_ancho;
            punto_y = (scr_alto - img_alto) / 2;
            break;
        }

        /* PARTE INFERIOR */
        case BOTTOM_LEFT: /* Esquina inferior izquierda */
        {
            punto_x = 0;
            punto_y = scr_alto - img_alto;
            break;
        }
        case BOTTOM_CENTER: /* Esquina inferior centro */
        {
            punto_x = (scr_ancho - img_ancho) / 2;
            punto_y = scr_alto - img_alto;
            break;
        }
        case BOTTOM_RIGHT: /* Esquina inferior derecha */
        {
            punto_x = scr_ancho - img_ancho;
            punto_y = scr_alto - img_alto;
            break;
        }
    }

    /* Dibujar imagen */
	dibujar_con_rasterizado_coords(archivo, punto_x, punto_y, img_ancho, img_alto);
}

/*
    dibujar_raster_png_coords()
    - Funcion que lee un archivo binario y dibuja pixel por pixel en modo grafico
    - A diferencia de las funciones anteriores de rasterizacion esta se hizo con el proposito de dibujar
    iconos para el sistema operativo, por lo cual se dibuja con un proceso de transparencia la cual
    el fondo de detras debe tener exactamente el color 200 (o un azul demasiado oscuro).
    - OJO: El archivo debe contener exactamente ancho*alto bytes para una representacion correcta.
*/
void dibujar_raster_png_coords(FILE *archivo, int pos_x, int pos_y, int ancho, int alto)
{
    unsigned char color; /* Asignar el color de cada pixel */
    int x, y; /* Variables de Recorrido de Cada Pixel */

    /* Leer y dibujar pixeles */
    for (y = 0; y < alto; y++) {
        for (x = 0; x < ancho; x++) {
            fread(&color, 1, 1, archivo);
            if (color != 200) /* Color de fondo a eliminar */
                putpixel(x + pos_x, y + pos_y, color);
        }
    }
}
#endif

