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
    Definción de Estructras
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
void guardar_paleta(void);
void imprimir_paleta(void);

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

    /* Calcular posición según selección */
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
    guardar_paleta()
    - Funcion que captura y guarda la paleta de colores real del hardware VGA/SVGA
    - Accede directamente a los registros de la paleta VGA (puertos 0x03C7 y 0x03C9)
    - Lee los componentes RGB de los 256 colores y los escala de 0-63 a 0-255
    - Guarda los datos en un archivo binario "real_palette.bin" de 768 bytes (256 colores × 3 bytes)
    - OJO: Esta funcion requiere acceso directo al hardware y puede no funcionar en todos los entornos
*/
void guardar_paleta() {
    FILE *pal_file;
    int i;
    struct palettetype pal;

    pal_file = fopen("real_palette.bin", "wb");
    if (!pal_file) {
        printf("Error creando archivo de paleta\n");
        return;
    }

    getpalette(&pal);

    printf("Guardando paleta real del hardware...\n");

    /* En Turbo C 2.0 tenemos que usar los registros VGA directamente */
    for (i = 0; i < 256; i++) {
        unsigned char r, g, b;
        
        /* Leer registro de paleta VGA directamente */
		outp(0x03C7, i);  /* Enviar índice de color a leer */
        r = inp(0x03C9);  /* Leer componente rojo */
        g = inp(0x03C9);  /* Leer componente verde */  
        b = inp(0x03C9);  /* Leer componente azul */
        
        /* Los valores VGA van de 0-63, escalar a 0-255 */
        r = (r * 255) / 63;
        g = (g * 255) / 63;
        b = (b * 255) / 63;
        
        /* Guardar en archivo */
        fwrite(&r, 1, 1, pal_file);
        fwrite(&g, 1, 1, pal_file);
        fwrite(&b, 1, 1, pal_file);
        
        printf("Color %3d: R=%3d, G=%3d, B=%3d\n", i, r, g, b);
    }
    
    fclose(pal_file);
    printf("Paleta guardada en: real_palette.bin (768 bytes)\n");
}

/*
    imprimir_paleta()
    - Funcion que genera un archivo de texto con la paleta de colores en formato hexadecimal
    - Crea un array en sintaxis C con los valores RGB de la paleta SVGA
    - Los valores se escalan de 0-63 a 0-252 para mayor compatibilidad
    - El archivo resultante "paleta_hex.txt" puede ser incluido directamente en programas C
    - OJO: El formato de salida esta optimizado para legibilidad con 4 colores por linea
*/
void imprimir_paleta() {
    FILE *hex_file = fopen("paleta_hex.txt", "w");
    int i;
    unsigned char r, g, b;
    
    fprintf(hex_file, "Paleta SVGA Real (256 colores):\n");
    fprintf(hex_file, "const unsigned char svga_palette[768] = {\n");
    
    for (i = 0; i < 256; i++) {
        outp(0x03C7, i);
		r = inp(0x03C9) * 4;  /* 0-63 a 0-252 */
        g = inp(0x03C9) * 4;
        b = inp(0x03C9) * 4;
        
        fprintf(hex_file, "    %3d, %3d, %3d", r, g, b);
        if (i < 255) fprintf(hex_file, ",");
        if ((i + 1) % 4 == 0) fprintf(hex_file, "\n");
    }
    
    fprintf(hex_file, "};\n");
    fclose(hex_file);
    printf("Paleta en hexadecimal guardada en: paleta_hex.txt\n");
}
#endif
