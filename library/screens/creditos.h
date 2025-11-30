#ifndef CREDITOS_H
#define CREDITOS_H

/*
    =======================================
                CREDITOS_H
    - Gabriela Ruiz
    =======================================
*/
/* 
    =======================================
    Importacion de Librerias Estandar
    =========================================
*/
#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include <dos.h>
#include <string.h>

/*
    =======================================
    Importacion de Librerias Personalizadas
    =========================================
*/
#include "gphadmin.h"   /* Libreria de administracion de modo grafico */
#include "raster.h"     /* Libreria de rasterizacion */

/*
    =======================================
    Declaracion de Directivas
    =========================================
*/
#define MAX_LINEAS 200
#define MAX_CHARS 200
#define NUM_TITULOS 10
#define TIEMPO_PAGINA 10000       /* 10 segundos por pagina */

/*
    =======================================
    Declaracion de Prototipos
    =========================================
*/
void creditos(void);    /* Funcion que ejecuta los creditos */

/* ============================================
                  CREDITOS
   ============================================ */
void creditos(void) 
{

    /* ============================
       VARIABLES
    ============================ */
    FILE *f;                  /* Puntero para archivo de texto */
    FILE *fondo;              /* Puntero para imagen final */
    char lineas[MAX_LINEAS][MAX_CHARS]; /* Arreglo para guardar lineas del archivo */
    int total = 0;            /* Contador de lineas leidas */
    int i, t;                 /* Variables para ciclos */
    int pagina_inicio = 0;    /* Indice de la primera linea de la pagina */
    int pagina_fin = 0;       /* Indice de la ultima linea de la pagina */
    int ventana_alto = 0;     /* Alto de la ventana */
    int texto_altura_total = 0; /* Altura total del texto en la pagina */
    int offset_y = 0;         /* Desplazamiento vertical para centrar */
    int y, x;                 /* Coordenadas para dibujar texto */
    int color_linea;          /* Color de la linea */
    int lineas_por_pagina[MAX_LINEAS]={0}; /* Arreglo con cantidad de lineas por pagina */
    int cant_lineas;          /* Cantidad de lineas de la pagina actual */

    /* Titulos y sus colores*/
    int titulos[NUM_TITULOS] = {0, 1, 2, 16, 18, 20, 22, 30, 33, 42}; /* Lineas que son titulos */
    int colores_titulos[NUM_TITULOS] = {33, 35, 37, 39, 41, 43, 45, 47, 51, 53}; /* Colores de los titulos */

    /* ============================================
       INICIAR CREDITOS
    ============================================ */
    set_bg(BLACK);  /* Se asigna fondo negro para una mejor presentacion */

    /* ============================================
       ABRIR ARCHIVO DE TEXTO
    ============================================ */
    f = fopen("C:\\TC20\\BIN\\creditos.txt", "r");   /* Abrimos el archivo de los creditos */

    if (f == NULL) {   /* Validacion en caso de que no se pueda abrir */
        outtextxy(10, 10, "ERROR: No se pudo abrir creditos.txt");
        getch_closegraph();
        return;
    }

    /* Leer lineas del archivo */
    while (fgets(lineas[total], MAX_CHARS, f) && total < MAX_LINEAS) {
        size_t len = strlen(lineas[total]);
        if (len > 0 && (lineas[total][len-1] == '\n' || lineas[total][len-1] == '\r'))
            lineas[total][len-1] = '\0';    /* Se eliminan los saltos de linea */
        total++;
    }
    fclose(f);  /* Cerramos el archivo */
    colocar_configuraciones();  /* Le asignamos el tipo de letra estandar de NeoRetro OS */

    /* ============================================
       DEFINIR LINEAS POR CADA PAGINA
            Debido a que los creditos seran presentados en forma de paginas (o diapositivas), 
            se le asigno a cada pagina un parrafo (cantidad de lineas) especifico.
            En la primera pagina encontramos solamente los titulos (NeoRetro OS y creditos),
            en la segunda y tercera pagina encontramos los agradecimientos, en la cuarta pagina
            esta la informacion sobre la universidad, carrera y docente, en la quinta pagina estan
            los nombres de todos los que somos parte del equipo de desarrolladores del sistema, en
            la sexta pagina esta el periodo de desarrollo, en la septima pagina se encuentra las
            herramientas utilizadas durante el proceso de creacion y codificacion y para culminar
            un pequeno mensaje para nuestros compañeros y usuarios. 
    =========================================== */
    /* Distribucion por pagina para que se asignen la cantidad de lineas segun la distribucion de parrafos */
    /* A traves de un bucle asignamos de que linea a que linea se van a escribir en pantalla (en esa pagina) */
    /* Pagina 1 */
    for(i = 0; i < 2; i++) lineas_por_pagina[i] = 2;

    /* Pagina 2 */
    for(i = 2; i < 8; i++) lineas_por_pagina[i] = 6;

    /* Pagina 3 */
    for(i = 8; i < 16; i++) lineas_por_pagina[i] = 8;

    /* Pagina 4 */
    for(i = 16; i < 22; i++) lineas_por_pagina[i] = 6;

    /* Pagina 5 */
    for(i = 22; i < 30; i++) lineas_por_pagina[i] = 8;

    /* Pagina 6 */
    for(i = 30; i < 33; i++) lineas_por_pagina[i] = 3;

    /* Pagina 7 */
    for(i = 33; i < 42; i++) lineas_por_pagina[i] = 9;

    /* Pagina 8 */
    for(i = 42; i < 51; i++) lineas_por_pagina[i] = 9;

    /* ============================================
          MOSTRAR POR PAGINAS
    ============================================ */
    pagina_inicio = 0;

    while (!kbhit()) { /* Mientras no se presione tecla */

        cant_lineas = lineas_por_pagina[pagina_inicio]; /* Lineas de la pagina actual */
        pagina_fin = pagina_inicio + cant_lineas;       /* Calcular fin de pagina para mostrar la imagen grupal */

        /* Si el indice final supera la cantidad total de lineas, ajustamos al total */
        if (pagina_fin > total) 
            pagina_fin = total;

        /* Limpiar pantalla antes de dibujar la pagina */
        setfillstyle(SOLID_FILL, BLACK); /* Definir estilo de relleno solido con color negro */
        bar(0, 0, getmaxx(), getmaxy()); /* Dibujar un rectangulo que cubre toda la ventana para limpiar */

        /* ========= Calcular centrado vertical del texto ========= */
        texto_altura_total = (pagina_fin - pagina_inicio) * 20; /* Altura total del bloque de texto de la pagina */
        ventana_alto = getmaxy(); /* Obtener alto de la ventana grafica */
        offset_y = (ventana_alto - texto_altura_total) / 2; /* Calcular desplazamiento vertical para centrar el texto */

        /* ========= Dibujar lineas ========= */
        for (i = pagina_inicio; i < pagina_fin; i++) {

            y = offset_y + (i - pagina_inicio) * 20; /* Posicion vertical de la linea actual */

            /* Centramos horizontalmente cada linea */
            x = (getmaxx() - textwidth(lineas[i])) / 2; /* Posicion horizontal calculada para centrar */

            /* Verificar si la linea es un titulo para cambiar color */
            color_linea = WHITE; /* Por defecto, color blanco */
            for (t = 0; t < NUM_TITULOS; t++) {
                if (i == titulos[t]) { /* Si la linea actual es un titulo */
                    color_linea = colores_titulos[t]; /* Cambiar al color del titulo correspondiente */
                    break; /* Salir del ciclo ya que encontramos el titulo */
                }
            }

            setcolor(color_linea); /* Aplicar color definido */
            outtextxy(x, y, lineas[i]); /* Dibujar la linea de texto en la pantalla */
        }

        /* Esperar 10 segundos antes de pasar a la siguiente pagina */
        delay(TIEMPO_PAGINA);

        /* Pasar a la siguiente pagina */
        pagina_inicio = pagina_fin;
        set_bg(BLACK);

        /* Al terminar creditos, mostrar imagen final */
        if (pagina_inicio >= total) {
            fondo = fopen("grupal.bin", "rb");  /* Uso de fichero binario */
            if (fondo != NULL) {
                dibujar_con_rasterizado_pos(    /* Funcion para rasterizacion de raster.h */
                    fondo, CENTER, 320, 150, WIDTH, HEIGHT
                );
                fclose(fondo); /* Cerramos el archivo */
            }
            getch();
            break;
        }
    }
}
#endif
