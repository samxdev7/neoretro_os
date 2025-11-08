/*
																																						===== Guia para crear aplicaciones =====
	En este archivo C se demuestra como usar las funciones de las librerias
    appadmin.h y comp.h, importantes para la creacion de interfaces, tanto
    para las aplicaciones y como del escritorio del sistema operativo.

    Para este proceso se mantiene el enfoque de que el programador debe crear su
    entorno semipreparado con estas librerias, definiendo los componentes a usar
    en base a sus estructuras, y luego aplicar los callbacks en un bucle.

    Incluso esta funcion les puede servir como plantilla para crear sus apps.

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
#include <string.h>

/*
    =======================================
    Importacion de Librerias Personalizadas
    =========================================
*/
#include "comp.h"       /* Libreria de componentes, el corazon de las interfaces */
#include "appadmin.h"   /* Libreria de gestor de aplicaciones */
#include "dynamic.h"    /* Libreria de memoria dinamica */

/*
    =======================================
    Callbacks
    =========================================
*/

void renderizar_mostrar(Component *componente) {
    renderizar_componente_texto(componente, "Mostrar");
}

void renderizar_suma(Component *componente) {
    renderizar_componente_texto(componente, "Suma");
}

void callback_mensaje_principal() {
    outtextxy(60, 180, "Hola Mundo desde callback!");
    
}

void callback_suma() {
    static int incremento = 5;
    mocultar();
	outtextxy(100 + incremento, 190 - incremento, "2 + 5 = 7");
    incremento += 5;
	mver();
}

/* Funciones Callbacks para Menu Despegable */
void callback_guardar() {
    colocar_configuraciones();
    mocultar();
    outtextxy(60, 100, "Opcion: Guardar - Archivo guardado!");
    mver();
}

void callback_ver_historial() {
    colocar_configuraciones();
    mocultar();
    outtextxy(60, 110, "Opcion: Ver historial como reyes!");
    mver();
}

/*
    manejar_opcion_menu() - FUNCIÓN ÚNICA PARA TODAS LAS OPCIONES
    - El programador novato solo necesita modificar esta función
    - Muy fácil de entender y mantener
*/
void manejar_opcion_menu(int opcion_seleccionada)
{
    switch (opcion_seleccionada) {
        case 0:  /* Primera opción del menú */
			callback_guardar();
            break;
        case 1:  /* Segunda opción (si hubiera) */
            callback_ver_historial();
            break;
        case 2:  /* Tercera opción */
            /* callback_mas_funciones(); */
            break;
        /* Agregar más casos según las opciones del menú */
    }
}

/*
    =======================================
    Programa Principal
    =========================================
*/
int main() {
    /* 1. Declaracion de componentes y variables */
    /* Declarar componentes principales (NO TOCAR) */
    Component barra_de_ventana, cerrar, despliegue;

    /* Declarar componentes personalizados */
    /* ==================================== */
    Component mensaje, suma;
    /* ==================================== */

    /* Declarar la cantidad de opciones del menu desplegable a utilizar */
    char **opciones_desplegables; /* NO TOCAR */
    unsigned char n_opciones = 2; /* Asignar cuantas opciones habran del menu despegable */

    /* Declarar variables del funcionamiento del menu de despliegue (NO TOCAR) */
    int *desplegable_buffer = NULL;
    unsigned char despliegue_activo = 0;
    unsigned char salir = 0;

    /* 2. Inicializar modo grafico */
    if (iniciar_modo_svga_256("C:\\TC20\\BIN")) return EXIT_FAILURE;

    /* 3. Limpiar y ajustar las configuraciones por defecto, y rellenar fondo */
    colocar_configuraciones();
    set_bg(WHITE);

    /* 4. Reservar memoria para las opciones a utilizar (NO TOCAR) */
    reservar_opciones_desplegables(&opciones_desplegables, n_opciones);

    /* 5. Definir nombres de las opciones del menu de despliegue (maximo 20 caracteres) */
    /* ==================================== */
    strcpy(opciones_desplegables[0], "Guardar");
    strcpy(opciones_desplegables[1], "Ver historial");
    /* ==================================== */

    /* 6. Inicializar barra de ventana (NO TOCAR) */
    inicializar_barra_ventana(&barra_de_ventana, &cerrar, &despliegue, "Bloc de Notas");

    /* 7. Inicializar componentes personalizados */
    /* ==================================== */

    /* Inicializar componente y hover mensaje */
	constructor_componente(&mensaje, 2, 16, 220, 40, 95, BLACK, BLACK, HOVER_DISPONIBLE);
    constructor_hover_componente(&mensaje, BLACK, WHITE, 99);

    /* Inicializar componente suma */
    constructor_componente(&suma, 80, 180, 130, 187, 108, BLACK, BLACK, HOVER_NO_DISPONIBLE);

    /* ==================================== */

    /* 8. Renderizar componentes personalizados */
    /* ==================================== */

    renderizar_componente_texto(&mensaje, "Mostrar");
    renderizar_componente_texto(&suma, "Suma");

    /* ==================================== */

    /* 9. Activar raton (NO TOCAR) */
    mver();

	do {
        /* 10. Capturar la posicion actual del mouse */
        short mouse_x = mxpos(), mouse_y = mypos();
        unsigned char resultado;
        
        /* ===================================
         HOVERS Y CALLBACKS DE BARRA DE VENTANA 
        ====================================== */

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

        /* ===================================
           HOVERS Y CALLBACKS PERSONALIZADOS 
        ====================================== */
        if (!despliegue_activo) {
            manejar_hover_componente(&mensaje, mouse_x, mouse_y, renderizar_mostrar);
            manejar_hover_componente(&suma, mouse_x, mouse_y, renderizar_suma);
        }
        
        if (!despliegue_activo && detectar_click_componente(&mensaje, mouse_x, mouse_y))
            callback_mensaje_principal();
            
        if (!despliegue_activo && detectar_click_componente(&suma, mouse_x, mouse_y))
            callback_suma();
        delay(100);
    } while (!salir);

    /* 17. Liberar memoria (NO TOCAR) */
    liberar_buffer_desplegable(&desplegable_buffer);
    liberar_arreglo(opciones_desplegables, n_opciones);

    /* 18. Cerrar modo grafico */
    closegraph();
    return 0;
}