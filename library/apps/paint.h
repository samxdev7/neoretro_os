#ifndef PAINT_C
#define PAINT_C
/*===== Aplicacion para hacer dibujos en modo grafico =====
	esta aplicacion permite al usuario dibujar en un canvas
    utilizando varias herramientas como pincel, cubeta de
    relleno, borrador, lazo y cuentagotas. El usuario puede
    seleccionar colores de una paleta y ajustar el tamaño del pincel.
    
    Creado por:
    - Johanna Duarte
	- Moises Aleman
*/
/*=======================================
    Importacion de Librerias Estandar
=========================================*/
#include <stdio.h>
#include <conio.h>
#include <graphics.h>
#include <stdlib.h>
#include <dos.h>
#include <string.h>
/*=========================================
    Importacion de Librerias Personalizadas
=========================================*/
#include "comp.h"       /* Libreria de componentes, el corazon de las interfaces */
#include "appadmin.h"   /* Libreria de gestor de aplicaciones */
#include "dynamic.h"    /* Libreria de memoria dinamica */
#include "mouse256.h"   /* Libreria de manejo de mouse en modo SVGA 256 colores */
#include "raster.h"     /* Libreria de manejo de raster grafico */
#include "boolso.h"     /* LIbreria de simulacion de booleanos */

/* Definicion de constantes */
#define MAX_COLORS 24
#define RGB 3

/*========================================
    Declaracion de Funciones
=========================================*/
void marco_canvas(void);
void guardar(void);
void cargar(void);
void rasterizar_bg(char filename[], short x1, short y1, short x2, short y2, char mode);
bool verify_mo_state(void);
void color_status(char color, char prev_color);
void edge_current_color(void);
void update_col(unsigned char nuevo_color);
void fill_area_size(void);
void bucket_callback(void);
void eraser_callback(void);
void square_callback(void);
void lazo_callback(void);
void callback_eyedropper(void);
void draw_icon(void);
void manejar_opcion_menu(int opcion_seleccionada);
void paint(void);

/* Definicion de paleta de colores */
typedef enum {
    COL1 = 0, COL2 = 92, COL3 = 43, COL4 = 6, COL5 = 186,
    COL6 = 205, COL7 = 17, COL8 = 67, COL9 = 42, COL10 = 39,
    COL11 = 111, COL12 = 180, COL13 = 178, COL14 = 92, COL15 = 70,
    COL16 = 72, COL17 = 2, COL18 = 124, COL19 = 197, COL20 = 77,
    COL21 = 52, COL22 = 53, COL23 = 126, COL24 = 127
}Paleta_colores_t;

const unsigned char paleta_colores[MAX_COLORS] = {
    COL1, COL2, COL3, COL4, COL5, COL6,
    COL7, COL8, COL9, COL10, COL11, COL12,
    COL13, COL14, COL15, COL16, COL17, COL18,
    COL19, COL20, COL21, COL22, COL23, COL24
};

/* Definicion de coordenadas Y para filas de colores */
enum Col_row_y_coords {
    Y1_ROW1 = 166, Y2_ROW1 = 176, 
    Y1_ROW2 = 178, Y2_ROW2 = 188
};

/* variables globales */
bool redibujar = true, draw_canvas = true, mouse_visible = false;
bool bucket_is_active = false, eraser_is_active = false;
bool sqr_is_active = false, brush_is_active = true;
bool lazo_is_active = false, eyedropper_is_active = false; 
short mouse_x, mouse_y;
unsigned char size_brush = 1, brush_color = BLACK, current_color = BLACK, prev_color = BLACK, clic;
unsigned char fig_clck_count = 0;
unsigned short x1, y1, x2, y2; /* variables de coords para figura y funcion square_callback */


/*========================================
    Declaracion de Funciones
=========================================*/
void marco_canvas() {
    setfillstyle(SOLID_FILL, BLACK);
    bar(46, 21, 316, 161);
}

void guardar() {
    FILE *p;
    int i, j;
    unsigned char pixel_color;
    
    p = fopen("C:\\TC20\\Ficheros\\paint_drawing.bin", "wb");
    if (!p) 
    { printf("Error al abrir el archivo para guardar.\n"); return; }

    for (i = 22; i < 161; i++) {
        for (j = 47; j < 316; j++) {
            pixel_color = getpixel(j, i);
            fwrite(&pixel_color, sizeof(unsigned char), 1, p);
        }
    }
    fclose(p);
}

void cargar() {
    FILE *p;
    int i, j;
    unsigned char pixel_color;

    p = fopen("C:\\TC20\\Ficheros\\paint_drawing.bin", "rb");
    if (!p) 
    { printf("Error al abrir el archivo para cargar.\n"); return; }

    for (i = 22; i < 161; i++) {
        for (j = 47; j < 316; j++) {
            fread(&pixel_color, sizeof(unsigned char), 1, p);
            putpixel(j, i, pixel_color);
        }
    }
    fclose(p);
}

void rasterizar_bg(char filename[], short x1, short y1, short x2, short y2, char mode) {
    FILE *p;
	p = fopen(filename, "rb");
    if (validar_archivo(p)) return;
    if (mode == 'r')
        dibujar_raster_png_coords(p, x1, y1, x2, y2); /* Dibuja el fondo de la calculadora */
    else if (mode == 'c')
        dibujar_con_rasterizado_coords(p, x1, y1, x2, y2); /* Dibuja el fondo de la calculadora */
    fclose(p);
}

bool verify_mo_state() {
    if (clic == 1 && mouse_visible) {
        mocultar();
        return true;
    }
    return false;
}

void color_status(char color, char prev_color)
{
    /* fondo */
    setfillstyle(SOLID_FILL, 28);
    bar(3, 166, 25, 188);
    setcolor(20);
    line(4, 167, 24, 167);
    line(4, 167, 4, 187);
    setcolor(27);
    line(25, 168, 25, 187);
    line(5, 187, 25, 187);

    /* color previo */
    setfillstyle(SOLID_FILL, prev_color);
    bar(12, 174, 22, 184);
    setcolor(BLACK);
    line(12, 174, 22, 174);
    line(12, 174, 12, 184);
    line(22, 174, 22, 184);
    line(12, 184, 22, 184);

    /* color activo */
    setfillstyle(SOLID_FILL, color);
    bar(7, 169, 17, 179);
    setcolor(BLACK);
    line(7, 169, 17, 169);
    line(7, 169, 7, 179);
    line(17, 169, 17, 179);
    line(7, 179, 17, 179);
}

void edge_current_color() {setcolor(prev_color); rectangle(47, 22, 315, 160);}

void update_col(unsigned char nuevo_color)
{
    if (nuevo_color == brush_color) {
        return;
    }
    prev_color  = brush_color;
    brush_color = nuevo_color;
    color_status(brush_color, prev_color);
    edge_current_color();
}
/*=======================================
    Callbacks
=========================================*/
void fill_area_size() {

    if (bucket_is_active)
        return;

    setcolor(brush_color);
    setfillstyle(SOLID_FILL, brush_color);

		if(verify_mo_state())
            mouse_visible = false;

    switch(size_brush) {
        case 10:

            if(minlimit(49, 24, 313, 158))
                putpixel(mouse_x, mouse_y, brush_color);
            break;

        case 1:
            if(minlimit(52, 27, 310, 155)) {
                sector(mouse_x, mouse_y, 0, 360, size_brush, size_brush);
            }
            break;
        case 3:

            if(minlimit(56, 31, 306, 151)) {
                sector(mouse_x, mouse_y, 0, 360, size_brush, size_brush);
            }
            break;
    }
}

 void bucket_callback() {
    if (!bucket_is_active)
        return;

    if (minlimit(50, 25, 312, 157) && mclick_M() == 1)
    {
        mocultar();
        setfillstyle(SOLID_FILL, brush_color);
        floodfill(mouse_x, mouse_y, prev_color);
        mouse_visible = false;
    }
}


void eraser_callback() {

    setfillstyle(SOLID_FILL, WHITE);

		if(verify_mo_state())
            mouse_visible = false;

    switch(size_brush) {
        case 10:

            if(minlimit(49, 24, 313, 158))
                bar(mouse_x - 1, mouse_y - 1, mouse_x + 1, mouse_y + 1);
            break;

        case 1:

            if(minlimit(52, 27, 310, 155)) {
                bar(mouse_x - 2, mouse_y - 2, mouse_x + 2, mouse_y + 2);
            }
            break;

        case 3:

            if(minlimit(56, 31, 306, 151)) {
                bar(mouse_x - 5, mouse_y - 5, mouse_x + 5, mouse_y + 5);
            }
            break;
    }
}

void square_callback() {

    do{
        clic = mclick();
        x2 = mxpos();
        y2 = mypos();
        if (fig_clck_count <= 1)
        {
            fig_clck_count++;
            x1 = mouse_x;
            y1 = mouse_y;
            mocultar();
            putpixel(x1, y1, brush_color);
            mver();
        }
        else if (!minlimit(48, 23, 314, 159)) break;

    }while (clic == 1);
    fig_clck_count = 0;
    setcolor(brush_color);
    mocultar();
    rectangle(x1, y1, x2, y2);
    mver();
}

void lazo_callback() {
    unsigned short coords[32]; /* almacenar hasta 16 puntos (x,y) */
    unsigned char point_count = 0; /* contador de puntos almacenados */
    unsigned short *p;
    unsigned short mouse_x, mouse_y, clic_m;

    do{
        clic_m = mclick();
        mouse_x = mxpos();
        mouse_y = mypos();

        if (minlimit(48, 23, 314, 159) && point_count < 15 && clic_m == 1){
            
            mocultar();
            setcolor(brush_color);
            putpixel(mouse_x, mouse_y, brush_color);
            coords[point_count * 2] = mouse_x;       /* almacenar x */
            coords[point_count * 2 + 1] = mouse_y;   /* almacenar y */
            point_count++;
            mver();
            delay(50);
        }
    }while (lazo_is_active && clic_m != 2);

    if (point_count < 2)
        return; /* no hay suficientes puntos para dibujar un lazo */

    coords[point_count * 2] = coords[0];
    coords[point_count * 2 + 1] = coords[1];   /* cerrar el lazo */
    point_count++;

    p = malloc(point_count * 2 * sizeof(unsigned short));
    if (!p) {
        printf("Error al asignar memoria para lazo...\n\r");
        free(p);
        return;
    }
    memcpy(p, coords, point_count * 2 * sizeof(unsigned short));
    setcolor(brush_color);
    mocultar();
    drawpoly(point_count, (int*)p);
    mver();
    free(p);
    lazo_is_active = false;
    return;
}

void callback_eyedropper() {
    unsigned char picked_color;
    if (!eyedropper_is_active)
        return;

    if (minlimit(47, 22, 315, 160) && mclick_M() == 1)
    {
        mocultar();
        picked_color = getpixel(mouse_x, mouse_y);
		update_col(picked_color);
        mouse_visible = false;
        eyedropper_is_active = false;
    }
}

void draw_icon()
{
    setcolor(BLACK);
    rectangle(28, 112, 37, 120); /* sqr */
    line(13, 119, 15, 117); /* lazo handle */
    line(15, 117, 11, 111); 
    line(11, 111, 13, 109);
    line(13, 109, 17, 116);
}

void manejar_opcion_menu(int opcion_seleccionada)
{
    switch (opcion_seleccionada) {
        case 0:  /* Primera opcion */
			guardar();
            break;
        case 1:  /* Segunda opcion */
            cargar();
            break;
    }
}
/*=======================================
    Programa Principal
=========================================*/
void paint(void) {
    /* 1. Declaracion de componentes y variables */
    /* Declarar componentes principales (NO TOCAR) */
    Component barra_de_ventana, cerrar, despliegue;

    /* Declarar componentes personalizados */
    /* ==================================== */
    Component col[24], size[3], tools[7], canvas;
    /* ==================================== */

    /* Declarar la cantidad de opciones del menu desplegable a utilizar */
    char **opciones_desplegables; /* NO TOCAR */
    unsigned char n_opciones = 2, i; /* Asignar cuantas opciones habran del menu despegable */

    /* Declarar variables del funcionamiento del menu de despliegue (NO TOCAR) */
    int *desplegable_buffer = NULL;
    unsigned char despliegue_activo = 0;
    unsigned char salir = 0;

    const unsigned char xcoords[12][2] = {
        {28, 38}, {40, 50}, {52, 62}, {64, 74}, {76, 86}, {88, 98},
        {100, 110}, {112, 122}, {124, 134}, {136, 146}, {148, 158}, {160, 170} /* x coords row 2 */
    };

    const unsigned char size_coords[3][4] = {
        {181, 181, 189, 190}, /* size 1 */
        {198, 181, 206, 190}, /* size 2 */
        {215, 181, 223, 190} /* size 3 */
    };

    const unsigned short tools_coords[7][4] = {
    6, 75, 20, 88, /* bucket */
    6, 45, 20, 58,  /* eraser */
    25, 45, 39, 58,  /* brush */
    6, 109, 20, 122, /* lazo */
    25, 75, 39, 88, /* eyedropper */
    254, 172, 275, 190,  /* clear */
    25, 109, 39, 122     /* square  */
    };

    /* 3. Limpiar y ajustar las configuraciones por defecto, y rellenar fondo */
    colocar_configuraciones();
	rasterizar_bg("BGPNT.bin", 2, 12, 316, 186, 'c');
    /* 4. Reservar memoria para las opciones a utilizar (NO TOCAR) */
    reservar_opciones_desplegables(&opciones_desplegables, n_opciones);

    /* 5. Definir nombres de las opciones del menu de despliegue (maximo 20 caracteres) */
    /* ==================================== */
    strcpy(opciones_desplegables[0], "Guardar");
    strcpy(opciones_desplegables[1], "Cargar");
    /* ==================================== */

    /* 6. Inicializar barra de ventana (NO TOCAR) */
    inicializar_barra_ventana(&barra_de_ventana, &cerrar, &despliegue, "Retro Paint");


    /* 7. Inicializar componentes personalizados */
    /* ==================================== */

    /* Inicializar componente y hover mensaje */

    for (i = 0; i < 12; i++) {
        constructor_componente(&col[i], xcoords[i][0], Y1_ROW1, xcoords[i][1], Y2_ROW1, paleta_colores[i], BLACK, BLACK, HOVER_NO_DISPONIBLE);

        constructor_componente(&col[i + 12], xcoords[i][0], Y1_ROW2, xcoords[i][1], Y2_ROW2, paleta_colores[i + 12], BLACK, BLACK, HOVER_NO_DISPONIBLE);
    }
    for (i = 0; i < 3; i++)
        constructor_componente(&size[i], size_coords[i][0], size_coords[i][1], size_coords[i][2], size_coords[i][3], WHITE, WHITE, WHITE, HOVER_NO_DISPONIBLE);

    for (i = 0; i < 7; i++)
        constructor_componente(&tools[i], tools_coords[i][0], tools_coords[i][1], tools_coords[i][2], tools_coords[i][3], LIGHTGRAY, BLACK, BLACK, HOVER_NO_DISPONIBLE);

    constructor_componente(&canvas, 47, 22, 315, 160, WHITE, WHITE, BLACK, HOVER_NO_DISPONIBLE);
 
    /* Renderizar componentes personalizados */
    /* ======================================================================================================================== */
    for (i = 0; i < 24; i++) {
        renderizar_componente(&col[i]);
    }
    for (i = 0; i < 3; i++)
        renderizar_componente(&size[i]);

    for (i = 0; i < 7; i++)
        renderizar_componente(&tools[i]);

    rasterizar_bg("ERASER.bin", 6, 45, 14, 13, 'c'); /* eraser */
    rasterizar_bg("EYDRPR.bin", 25, 75, 14, 13, 'c'); /* Eyedropper */
    rasterizar_bg("BUCKET.bin", 6, 75, 14, 13, 'c'); /* bucket */
	rasterizar_bg("WBERASER.bin", 254, 172, 21, 19, 'c'); /* white board eraser */
    rasterizar_bg("BRUSH.bin", 25, 45, 14, 13, 'c'); /* brush */
    draw_icon(); /* lazo icon */
    
    color_status(brush_color, prev_color);
    /* ======================================= */

    /* redibujar canvas fuera del bucle */
    marco_canvas();
    renderizar_componente(&canvas);
    mver();
    /* Bucle principal de la aplicacion */
	do{
        unsigned char resultado;

        /* Capturar la posicion actual del mouse */
        mouse_x = mxpos(); mouse_y = mypos(); clic = mclick();
        
        /* ===== HOVERS ===== */
        manejar_hover_componente(&cerrar, mouse_x, mouse_y, renderizar_boton_cerrar);
        manejar_hover_componente(&despliegue, mouse_x, mouse_y, renderizar_boton_despliegue);

        /* ===== CALLBACKS ===== */
        /* Manejo completo de la barra de ventana */        
        resultado = manejar_barra_ventana(
            &cerrar, &despliegue, mouse_x, mouse_y,
            &despliegue_activo, n_opciones, 
            opciones_desplegables, &desplegable_buffer
        );

        /* Procesar resultados */
        if (resultado == n_opciones) salir = 1;
        else if (resultado >= 0) manejar_opcion_menu(resultado);

        if (draw_canvas) {
            mocultar(); renderizar_componente(&canvas);
            mver(); draw_canvas = false; }
        if (clic == 0 && !mouse_visible) { mver(); mouse_visible = true; }
        /* ===== DETECCION DE EVENTOS ===== */
        if (clic == 1)
        {
            for (i = 0; i < 24; i++)
            {
                if (mouse_sobre_componente(&col[i], mouse_x, mouse_y)) 
                {
                    update_col(paleta_colores[i]);
                    break;
                }
            }
            for (i = 0; i < 7; i++)
            {
                if (mouse_sobre_componente(&tools[i], mouse_x, mouse_y)) 
                {
                    switch (i) {
                        case 0: /* bucket */
                            sqr_is_active = false; eraser_is_active = false;
                            brush_is_active = false; eyedropper_is_active = false;
                            lazo_is_active = false; bucket_is_active = true;
                            break;
                        case 1: /* eraser */
                            sqr_is_active = false; bucket_is_active = false;
                            lazo_is_active = false; eyedropper_is_active = false;
                            eraser_is_active = true; break;
                        case 2: /* brush */
                            sqr_is_active = false; bucket_is_active = false;
                            eraser_is_active = false; eyedropper_is_active = false;
                            lazo_is_active = false; brush_is_active = true; break;
                        case 3: /* lazo */
                            sqr_is_active = false; bucket_is_active = false;
                            eraser_is_active = false; eyedropper_is_active = false;
                            lazo_is_active = true; break;
                        case 4: /* eyedropper */
                            bucket_is_active = false; eraser_is_active = false; 
                            lazo_is_active = false; sqr_is_active = false;
                            eyedropper_is_active = true; break;
                        case 5: /* clear */
                            bucket_is_active = false; eraser_is_active = false;
                            lazo_is_active = false; eyedropper_is_active = false;
                            sqr_is_active = false; draw_canvas = true; break;
                        case 6: /* square */
                            bucket_is_active = false; eraser_is_active = false;
                            lazo_is_active = false; eyedropper_is_active = false;
                            sqr_is_active = true; break;
                    }
                   break;
                }
            }
            for (i = 0; i < 3; i++)
            {
                if (mouse_sobre_componente(&size[i], mouse_x, mouse_y) && clic == 1) {
                    switch (i) {
                        case 0: size_brush = 10;
                                mocultar();
                                setfillstyle(SOLID_FILL, BLACK);
                                bar(182, 182, 188, 189);
                                setfillstyle(SOLID_FILL, WHITE);
                                bar(199, 182, 205, 189);
                                bar(216, 182, 222, 189);
                                mver();
                                break;
                        case 1: size_brush = 1; 
                                mocultar();
                                setfillstyle(SOLID_FILL, BLACK);
                                bar(199, 182, 205, 189);
                                setfillstyle(SOLID_FILL, WHITE);
                                bar(182, 182, 188, 189);
                                bar(216, 182, 222, 189);
                                mver();
                                break;
                        case 2: size_brush = 3; 
                                mocultar();
                                setfillstyle(SOLID_FILL, BLACK);
                                bar(216, 182, 222, 189);
                                setfillstyle(SOLID_FILL, WHITE);
                                bar(182, 182, 188, 189);
                                bar(199, 182, 205, 189);
                                mver();
                                break;
                    }
                    break;
                }
            }
        }

        if (eyedropper_is_active && mouse_sobre_componente(&canvas, mouse_x, mouse_y) && clic == 1) callback_eyedropper();

        if (sqr_is_active && mouse_sobre_componente(&canvas, mouse_x, mouse_y) && clic == 1) square_callback();

        if (bucket_is_active) bucket_callback();

        if (eraser_is_active && clic == 1) eraser_callback();

        if (lazo_is_active && mouse_sobre_componente(&canvas, mouse_x, mouse_y)) lazo_callback();

		if (mouse_sobre_componente(&canvas, mouse_x, mouse_y) && clic == 1 && !eraser_is_active) fill_area_size();

	}while (!salir);

	/* 17. Liberar memoria (NO TOCAR) */
    liberar_buffer_desplegable(&desplegable_buffer);
    liberar_arreglo((void*)opciones_desplegables, n_opciones);
}
#endif
