/*

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
#include "timecont.h"   /* Libreria de manejo facil del tiempo */
#include "config.h"     /* Libreria de las configuraciones del sistema operativo */

/*
    =======================================
    Declaracion de Directivas
    =========================================
*/
#define ICONOS_ESCRITORIO 5         /* Cantidad de iconos del escritorio */
#define ICONOS_BARRA_DE_TAREAS 4    /* Cantidad de iconos de la barra de tareas */

/*
    =======================================
    Definicion de Enum
    =========================================
*/

/*
    Enum DesktopIcons
    Estructura enum que categoriza las aplicaciones del sistema operativo, es decir,
    que resalta tambien los iconos de escritorio, abarcando las siguientes aplicaciones
    en el siguiente orden:
    - Sistema.
    - Configuraciones.
    - Calculadora.
    - Bloc de Notas.
    - Paint.
*/
typedef enum
{
    SISTEMA,
    CONFIGURACIONES,
    CALCULADORA,
    BLOC_DE_NOTAS,
    PAINT
} DesktopIcons;

/*
    Enum TaskBarIcons
    Estructura enum que categoriza las acciones y/o creditos del sistema operativo, siendo
    en su mayoria acciones de arranque. Estas se categorizan en el siguiente orden:
    - Accion de apagar el sistema operativo.
    - Accion de reiniciar el sistema operativo.
    - Accion de suspender el sistema operativo.
    - Mostrar los creditos del sistema operativo.
*/
typedef enum
{
    APAGAR,
    REINICIAR,
    SUSPENDER,
    CREDITOS
} TaskBarIcons;

/*
    =======================================
    Declaracion de Prototipos
    =========================================
*/

/* Seccion 1: Callbacks de Pruebas (escritorio) */
void callback_sistema(void);
void callback_calculadora(void);
void callback_bloc_de_notas(void);
void callback_paint(void);

/* Seccion 2: Callbacks de prueba (barra de tareas) */
void callback_reiniciar(void);
void callback_suspender(void);
void callback_creditos(void);

/* Seccion 3: Funciones de renderizado */
void renderizar_escritorio(Component *barra_de_tareas, Component iconos_escritorio[], Component iconos_barra_tareas[]);
void mostrar_tiempo_escritorio(char *buffer_hora, char *buffer_fecha);

/*
    =======================================
    Inicializacion de Funciones
    =========================================
*/
/* Seccion 1: Callbacks de Pruebas (escritorio) */
void callback_sistema(void)
{
    mocultar();
    colocar_configuraciones();
    outtextxy(100, 60, "App Detectada: Sistema");
    mver();
}

void callback_calculadora(void)
{
    mocultar();
    colocar_configuraciones();
    outtextxy(100, 80, "App Detectada: Calculadora");
    mver();
}

void callback_bloc_de_notas(void)
{
    mocultar();
    colocar_configuraciones();
    outtextxy(100, 90, "App Detectada: Bloc de Notas");
    mver();
}

void callback_paint(void)
{
    mocultar();
    colocar_configuraciones();
    outtextxy(100, 100, "App Detectada: Paint");
    mver();
}

/* Seccion 2: Callbacks de prueba (barra de tareas) */
void callback_reiniciar(void)
{
    mocultar();
    colocar_configuraciones();
    outtextxy(100, 80, "Barra de Tareas: Reiniciar");
    mver();
}

void callback_suspender(void)
{
    mocultar();
    colocar_configuraciones();
    outtextxy(100, 90, "Barra de Tareas: Suspender");
    mver();
}

void callback_creditos(void)
{
    mocultar();
    colocar_configuraciones();
    outtextxy(100, 100, "Barra de Tareas: Creditos");
    mver();
}

/* Seccion 3: Funciones de renderizado */
/*
    renderizar_escritorio();
    Funcion que dibuja los iconos y la barra de tareas del sistema operativo.

    Parametros:
    Component *barra_de_tareas: Componente de la barra de tareas, la cual sirve de soporte
    para los iconos de esta barra.
    Component iconos_escritorio[]: Son los iconos pertenecientes a la ventana principal
    (o escritorio), la cual muestra las aplicaciones principales del sistema operativo.
    Component iconos_barra_tareas[]: Son los iconos pertenecientes a la barra de tareas,
    que ejecutan acciones a nivel bajo (simulado) del sistema operativo o los creditos.
*/
void renderizar_escritorio(
    Component *barra_de_tareas, 
    Component iconos_escritorio[], 
    Component iconos_barra_tareas[]
) {
    /* 1. Declarar una variable de indice */
    short icono;

	/* 2. Limpiar pantalla completa (temporal) */
    set_bg(WHITE);

    /* 3. Renderizar barra de tareas */
    renderizar_componente(barra_de_tareas);

    /* 4. Renderizar iconos de escritorio */
    for (icono = 0; icono < ICONOS_ESCRITORIO; icono++)
        renderizar_componente(&iconos_escritorio[icono]);

    /* 5. Renderizar icono de barra de tareas */
	for (icono = 0; icono < ICONOS_BARRA_DE_TAREAS; icono++)
        renderizar_componente(&iconos_barra_tareas[icono]);
}

/*
    mostrar_tiempo_escritorio();
    Funcion que muestra el tiempo en vivo a traves de la barra de tareas, realizando
    redibujado.

    Parametros:
    char *buffer_hora: Cadena que administra la hora actual.
    char *buffer_fecha: Cadena que administra la fecha actual.
*/
void mostrar_tiempo_escritorio(char *buffer_hora, char *buffer_fecha)
{
    /* 1. Declarar variable de espaciado de pixeles */
    short padding = 6;
    
    /* 2. Verificar si el tiempo cambio, en ese caso se actualizan los buffers 
    automaticamente */
    if (verificar_cambio_tiempo(buffer_hora, buffer_fecha))
    {
        /* 3. Se limpia el area utilizada para mostrar el tiempo */
        setfillstyle(SOLID_FILL, BLACK);
        bar(
            WIDTH - textwidth(buffer_fecha) - padding,
            HEIGHT - textheight(buffer_hora) - 15,
            WIDTH - padding,
            HEIGHT - padding
        );
    }

    /* 4. En caso de que se haya cumplido o no la condicion anterior se muestra la hora y fecha */
    setcolor(WHITE);
    outtextxy(  /* Hora */
        WIDTH - textwidth(buffer_hora) - padding,
        HEIGHT - textheight(buffer_hora) - 15,
        buffer_hora
    );
    outtextxy(  /* Fecha */
        WIDTH - textwidth(buffer_fecha) - padding,
        HEIGHT - textheight(buffer_fecha) - 5,
        buffer_fecha
    );
}

/*
    =======================================
    Programa Principal
    =========================================
*/
int main() {
    /* 1. Declaracion de componentes y variables */
	/* Componentes del escritorio */
    Component barra_de_tareas, iconos_escritorio[5], iconos_barra_tareas[4];

    /* Bandera de apagado (temporal) */
    short apagar_bandera = 0;

    /* Formatos de fecha y hora */
    char hora_actual[FORMATO_HORA];
    char fecha_actual[FORMATO_FECHA];

    /* Posiciones de raton */
    short mouse_x, mouse_y;

    /* Indice y espaciado */
    short i, padding = 10;

    /* 2. Inicializar modo grafico (temporal) */
    if (iniciar_modo_svga_256("C:\\TC20\\BIN")) return EXIT_FAILURE;

    /* 3. Ajustar configuraciones por defecto y limpiar pantalla */
    colocar_configuraciones();
    set_bg(WHITE);

    /* 4. Obtener fecha y hora */
    obtener_fecha_hora(hora_actual, fecha_actual);

    /* 5. Inicializar componentes principales del escritorio */
    /* Iniciar componente de barra de tareas */
    constructor_componente(&barra_de_tareas, 0, HEIGHT - 25, WIDTH, HEIGHT, BLACK, BLACK, BLACK, HOVER_NO_DISPONIBLE);

    /* Inicializar iconos de escritorio */
    for (i = 0; i < ICONOS_ESCRITORIO; i++)
    {
        constructor_componente(
            &iconos_escritorio[i], 
            16 + ((i * 40) + (padding * i)), 
            16, 
            56 + ((i * 40) + (padding * i)), 
            56, 
            WHITE, BLACK, BLACK, HOVER_NO_DISPONIBLE
        );
    }

    /* Inicializar iconos de barra de tareas */
    for (i = 0; i < ICONOS_BARRA_DE_TAREAS; i++)
    {
        constructor_componente(
            &iconos_barra_tareas[i], 
            105 + ((i * 20) + (padding * i)),
            HEIGHT - 23, 
            125 + ((i * 20) + (padding * i)), 
            HEIGHT - 3, 
            BLACK, WHITE, WHITE, HOVER_NO_DISPONIBLE
        );
    }

    /* 6. Renderizar escritorio completo en cada iteracion */
    renderizar_escritorio(&barra_de_tareas, iconos_escritorio, iconos_barra_tareas);

    /* 7. Activar raton */
    mver();

    /* 8. Bucle principal la cual se manejara la logica del escritorio del sistema operativo */
    do {
        /* 9. Se capturan las posiciones del raton */
        mouse_x = mxpos();
        mouse_y = mypos();

        /* 10. Mostrar y actualizar tiempo */
        mostrar_tiempo_escritorio(hora_actual, fecha_actual);

        /* 11. Procesar clicks en escritorio, la cual se debe e */
        if (mouse_x >= iconos_escritorio[0].x1 && 
            mouse_x <= iconos_escritorio[4].x2 && 
            mouse_y >= iconos_escritorio[0].y1 && 
            mouse_y <= iconos_escritorio[4].y2)
        {
            for (i = 0; i < 5; i++)
            {
                if (detectar_click_componente(&iconos_escritorio[i], mouse_x, mouse_y))
                {
                    switch (i)
                    {
                        case SISTEMA:
                            callback_sistema();
                            break;
                        case CONFIGURACIONES:
                            mocultar();
                            app_configuraciones();
                            break;
                        case CALCULADORA:
                            callback_calculadora();
                            break;
                        case BLOC_DE_NOTAS:
                            callback_bloc_de_notas();
                            break;
                        case PAINT:
                            callback_paint();
                            break;
                    }
                    mocultar();
                    renderizar_escritorio(&barra_de_tareas, iconos_escritorio, iconos_barra_tareas);
                    mver();
                    break;
                }
            }
        }

        /* Procesar clicks en barra de tareas */
        if (mouse_x >= iconos_barra_tareas[0].x1 && 
            mouse_x <= iconos_barra_tareas[3].x2 && 
            mouse_y >= iconos_barra_tareas[0].y1 && 
            mouse_y <= iconos_barra_tareas[3].y2)
        {
            for (i = 0; i < 4; i++)
            {
                if (detectar_click_componente(&iconos_barra_tareas[i], mouse_x, mouse_y))
                {
                    switch (i)
                    {
                        case APAGAR:
                            apagar_bandera = 1;
                            break;
                        case REINICIAR:
                            callback_reiniciar();
                            break;
                        case SUSPENDER:
                            callback_suspender();
                            break;
                        case CREDITOS:
                            callback_creditos();
                            break;
                    }
                    break;
                }
            }
        }

        /* Retardo para evitar saturacion */
        delay(100);
        
    } while (!kbhit() && !apagar_bandera);

    /* Cerrar modo grafico */
    closegraph();
    return 0;
}