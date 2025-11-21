#ifndef DESKTOP_H
#define DESKTOP_H

/*
    DESKTOP_H
    Libreria que administra el escritorio del sistema operativo.

	- Samuel Rueda
*/

/*
    =======================================
    Importacion de Librerias
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
#include "gphadmin.h"   /* Libreria que administra el modo grafico y el sistema operativo */
#include "raster.h"     /* Libreria encargada de administrar la rasterizacion de ficheros */
#include "comp.h"       /* Libreria de componentes para la interactividad del sistema operativo */
#include "appadmin.h"   /* Libreria que administra las aplicaciones del sistema operativo */
#include "dynamic.h"    /* Libreria que ofrece funciones para la memoria dinamica */
#include "timecont.h"   /* Libreria que administra el tiempo en base a cadenas y no estructuras */
#include "config.h"     /* Aplicacion de configuraciones/ajustes del sistema operativo */

/*
    =======================================
    Declaracion de Directivas
    =========================================
*/
/* Cantidad de iconos */
#define ICONOS_ESCRITORIO 5
#define ICONOS_BARRA_TAREAS 4

/* Coordenadas */
#define INICIO_ICONOS_X_1      10
#define INICIO_ICONOS_X_2      266
#define INICIO_ICONOS_Y        5

#define POS_X_BARRA_TAREAS     105
#define POS_Y_BARRA_TAREAS     (HEIGHT - 23)

/* Tamanos */
#define TAMANO_ICONOS_ESCRITORIO    30
#define TAMANO_ICONOS_BARRA_TAREAS  20
#define ALTURA_BARRA_TAREAS         25

/* Espaciados */
#define PADDING_BARRA_TAREAS    12
#define PADDING_TIEMPO          6

#define SEPARACION_ICONOS_Y     55
#define SEPARACION_ICONOS_Y_ALT 60

/* Colores */
#define COLOR_FONDO_TEXTO      30
#define COLOR_BARRA_TAREAS     BLACK
#define COLOR_TEXTO_TIEMPO     WHITE

/*
    =======================================
    Declaracion de Variables Globales
    =========================================
*/
/* Arreglo de cadenas de los ficheros de iconos de escritorio */
static char *ficheros_iconos_escritorio[ICONOS_ESCRITORIO] = {
    "SHELLVV3.bin", "CONFVV3.bin", "CALCVV3.bin", "NOTEVV3.bin", "PAINTVV3.bin"
};

/* Arreglo de cadenas de los nombres de las apps de escritorio */
static char *nombres_apps_escritorio[ICONOS_ESCRITORIO] = {
    "Sistema", "Ajustes", "Calculadora", "Bloc de|Notas", "Paint"
};

/* Arreglo de cadenas de los ficheros de iconos de barra de tareas */
static char *ficheros_iconos_barra_tareas[ICONOS_BARRA_TAREAS] = {
    "SHUTVV.bin", "RESVV.bin", "SUSVV.bin", "CREDVV.bin"
};

/*
    =======================================
    Definicion de Enums
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
typedef enum {
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
typedef enum {
    ICONO_APAGAR,
    ICONO_REINICIAR,
    ICONO_SUSPENDER,
    ICONO_CREDITOS
} TaskBarIcons;

/*
    =======================================
    Declaracion de Prototipos
    =========================================
*/

/* Seccion 1: Funciones de renderizado */
void renderizar_fondo_de_pantalla(void);
void cargar_y_dibujar_icono(const char *nombre_archivo, short x, short y, short ancho, short alto);
void renderizar_texto_icono(short icono_x, short icono_y, char *texto);
void renderizar_escritorio(Component *barra_tareas);
void mostrar_tiempo_escritorio(char *buffer_hora, char *buffer_fecha);

/* Seccion 2: Funcion de escritorio */
BootManageOS escritorio_so(void);

/*
    =======================================
    Inicializacion de Funciones
    =========================================
*/

/* Seccion 1: Funciones de renderizado */
/*
    renderizar_fondo_de_pantalla();
    Funcion que renderiza el fondo de pantalla seleccionado (aun no implementado).
*/
void renderizar_fondo_de_pantalla(void)
{
    /* 1. Se declara un buffer de fichero y se abre el fichero del fondo como lectura binaria */
    FILE *fondo = fopen("dinosaur.bin", "rb");
    if (fondo == NULL) return; /* Tambien se verifica si se abrio correctamente */
    
    /* 2. Se dibuja el fondo con rasterizado */
    dibujar_con_rasterizado_pos(fondo, CENTER, 200, 200, WIDTH, HEIGHT);

    /* 3. Se cierra el fichero de fondo */
    fclose(fondo);
}

/* 
    renderizar_texto_icono();
    Funcion que muestra los nombres de las aplicaciones en el escritorio, la cual
    se colocan debajo de los iconos de estas aplicaciones. Este solo procesa
    las aplicaciones una por una, la cual para aplicar los textos a todas
    se debe utilizar un bucle.

    Parametros:
    short icono_x, icono_y: Posicion incial del icono
    const char *texto: Cadena pertenenciente al nombre de la app.
*/
void renderizar_texto_icono(short icono_x, short icono_y, char *texto)
{
    /* 1. Se declaran variables importantes para el renderizado */
    char *delimitador;                      /* Delimitador de separacion */
    char linea1_temp[14], linea2_temp[14];  /* Cadenas temporales */
    short centro_x1, centro_x2;             /* Posiciones de cadenas */

    /* 2. Se utiliza '|' para determinar si el texto es muy largo en algun punto
    de la cadena, se utiliza strchr para buscar esa posicion. Esto permitira
    imprimir el nombre de la aplicacion en dos lineas separadas */
    delimitador = strchr(texto, '|');

    /* 3. En el caso de que se haya detectado esta posicion se muestra en dos lineas */
    if (delimitador != NULL) {
        /* Se inicializa la primera linea hasta el delimitador */
		strncpy(linea1_temp, texto, delimitador - texto);
        linea1_temp[delimitador - texto] = '\0';

        /* Se inicializa la segunda linea desde el delimitador */
        strcpy(linea2_temp, delimitador + 1);

        /* Se calculan las posiciones de las dos lineas */
        centro_x1 = icono_x + 15 - (textwidth(linea1_temp) / 2);
        centro_x2 = icono_x + 15 - (textwidth(linea2_temp) / 2);

        /* Se dibuja barras dependiendo del tema la cual permitiran colocar el texto
        sin problema alguno, pero utiliza las coordenadas de las dos lineas */
        setfillstyle(SOLID_FILL, COLOR_FONDO_TEXTO);
        bar( /* Primera linea */
            centro_x1 - 2,
            icono_y + 35,
            centro_x1 + 2 + textwidth(linea1_temp),
            icono_y + 37 + textheight(linea1_temp)
        );
        bar( /* Segunda linea */
            centro_x2 - 2,
            icono_y + 37 + textheight(linea1_temp),
            centro_x2 + 2 + textwidth(linea2_temp),
            icono_y + 39 + textheight(linea1_temp) + textheight(linea2_temp)
        );

        /* Finalmente se imprime el nombre de la app dividido en dos lineas */
        setcolor(BLACK);
        outtextxy(centro_x1, icono_y + 35, linea1_temp); /* Primera linea */
        outtextxy(  /* Segunda linea */
            centro_x2,
            icono_y + 37 + textheight(linea1_temp),
            linea2_temp
        );
    } else { /* 4. Sino, se imprime a como esta */
        /* Se detecta la posicion final del texto */
        short texto_x = icono_x + 15 - (textwidth(texto) / 2);

        /* Se dibuja una barra para colocar el texto sin verse afectado por el fondo */
        setfillstyle(SOLID_FILL, COLOR_FONDO_TEXTO);
        bar(
            texto_x - 2,
            icono_y + 35,
            texto_x + 2 + textwidth(texto),
            icono_y + 37 + textheight(texto)
        );

        /* Finalmente se coloca el texto */
        setcolor(BLACK);
        outtextxy(texto_x, icono_y + 35, texto);
    }
}

/*
    cargar_y_dibujar_icono();
    Funcion que renderiza los iconos a traves de ficheros de imagen.

    Parametros:
    - const char *nombre_archivo: Nombre/Ruta del fichero del icono a renderizar.
    - short x, short y: Posicion inicial para dibujar el icono.
    - short ancho, short alto: Dimensiones del fichero.
*/
void cargar_y_dibujar_icono(const char *nombre_archivo, short x, short y, short ancho, short alto)
{
    /* 1. Se declara la variable apuntadora a FILE, y se abre el fichero como lectura binaria */
    FILE *fichero = fopen(nombre_archivo, "rb");
    
    /* 2. Se valida si se abrio correctamente */
    if (fichero != NULL) {
        /* Si es asi, dibuja el icono del fichero */
        dibujar_raster_png_coords(fichero, x, y, ancho, alto);
        fclose(fichero); /* Y finalmente, se cierra */
    }
}

/*
    renderizar_escritorio();
    Se encarga de renderizar el escritorio completo, renderizando incluso en base
    a las funciones anteriormente declaradas y descritas. Esta funcion renderiza:
    - Fondo de Pantalla.
    - Iconos de Escritorio.
    - Iconos de Barra de Tareas.
    - Barra de Tareas.
    - Nombres de Aplicaciones.

    Parametros:
    - Component *barra_tareas: Componente asociado a la barra de tareas.
    - Component iconos_escritorio[]: Componentes de los iconos de escritorio
    para dibujar en bases a sus coordenadas los iconos.
    - Component iconos_barra[]: Componentes de los iconos de la barra de tareas
    para dibujar en bases a sus coordenadas los iconos.
*/
void renderizar_escritorio(Component *barra_tareas) 
{
    /* 1. Se declaran las variables a utilizar */
    short icono;            /* Variable de indice de los iconos */
    short icono_x, icono_y; /* Variables de posicion de los iconos */
    
    /* 2. Se dibuja el fondo de pantalla */
    set_bg(WHITE);
    renderizar_fondo_de_pantalla();

    /* 3. Se dibuja la barra de tareas */
    renderizar_componente(barra_tareas);

    /* 4. Se colocan las configuraciones para texto */
    colocar_configuraciones();
    
    /* 5. Renderizar iconos y textos de las apps en el escritorio */
    for (icono = 0; icono < ICONOS_ESCRITORIO; icono++) {
        /* 
            Para este caso se dibujaran los iconos de escritorio en dos columnas, mostrando 
            la siguiente posicion:
            App 1               App 3
            App 2               App 4
                                App 5

            Para ello se calculan antes las posiciones para solo despues colocar los elementos
            de las aplicaciones a dibujar.
        */
        if (icono < 2) {    /* Primera Columna */
            icono_x = INICIO_ICONOS_X_1;
            icono_y = INICIO_ICONOS_Y + (icono * SEPARACION_ICONOS_Y);
        } else {            /* Segunda Columna */
            icono_x = INICIO_ICONOS_X_2;
            icono_y = INICIO_ICONOS_Y + 
                ((icono - 2) * (icono < 4 ? SEPARACION_ICONOS_Y : SEPARACION_ICONOS_Y_ALT));
        }
        
        /* Una vez calculada las posiciones se dibujan los iconos de las apps */
        cargar_y_dibujar_icono(
            ficheros_iconos_escritorio[icono], 
            icono_x, icono_y, 
            TAMANO_ICONOS_ESCRITORIO, TAMANO_ICONOS_ESCRITORIO
        );
                        
        /* Y se dibuja el texto adyacente de las aplicaciones */
        renderizar_texto_icono(icono_x, icono_y, nombres_apps_escritorio[icono]);
    }
    
    /* 6. Renderizar iconos de barra de tareas */
    for (icono = 0; icono < ICONOS_BARRA_TAREAS; icono++) {
        /* Se calcula la posicion de cada icono de la barra de tareas */
        short pos_x = POS_X_BARRA_TAREAS + ((icono * TAMANO_ICONOS_BARRA_TAREAS) + (PADDING_BARRA_TAREAS * icono));

        /* Luego se dibuja cada icono de la barra de tareas */
        cargar_y_dibujar_icono(
            ficheros_iconos_barra_tareas[icono], 
            pos_x, POS_Y_BARRA_TAREAS,
            TAMANO_ICONOS_BARRA_TAREAS, TAMANO_ICONOS_BARRA_TAREAS
        );
    }
}

/* 
    mostrar_tiempo_escritorio()
    Funcion que redibuja la fecha y hora para mantenerlo actualizado, dibujandose
    en la barra de tareas.

    Parametros:
    - char *buffer_hora: Cadena la cual se almacena la hora actual.
    - char *buffer_fecha: Cadena la cual se almacena la fecha actual.
*/
void mostrar_tiempo_escritorio(char *buffer_hora, char *buffer_fecha)
{
    /* 1. Verificar si el tiempo cambio, en ese caso se actualizan los buffers 
    automaticamente */    
    if (verificar_cambio_tiempo(buffer_hora, buffer_fecha)) {
        /* 2. Se limpia el area utilizada para mostrar el tiempo */
        setfillstyle(SOLID_FILL, COLOR_BARRA_TAREAS);
        bar(WIDTH - textwidth(buffer_fecha) - PADDING_TIEMPO,
            HEIGHT - textheight(buffer_hora) - 15,
            WIDTH - PADDING_TIEMPO,
            HEIGHT - PADDING_TIEMPO);
    }
    
    /* 3. En caso de que se haya cumplido o no la condicion anterior se muestra la hora y fecha */
    setcolor(COLOR_TEXTO_TIEMPO);
    outtextxy( /* Hora */
        WIDTH - textwidth(buffer_hora) - PADDING_TIEMPO,
        HEIGHT - textheight(buffer_hora) - 15,
        buffer_hora
    );
    outtextxy( /* Fecha */
        WIDTH - textwidth(buffer_fecha) - PADDING_TIEMPO,
        HEIGHT - textheight(buffer_fecha) - 5,
        buffer_fecha
    );
}

/* Seccion 2: Funcion de escritorio */
/* 
    escritorio_so();
    - Funcion que administra el funcionamiento del escritorio.
    - Es la funcion principal de esta libreria.

    Devuelve:
    1/APAGAR si el usuario hizo click en apagar.
    2/REINICIAR si el usuario hizo click en reiniciar.
    3/SUSPENDER si el usuario hizo click en suspender.
*/
BootManageOS escritorio_so(void)
{
    /* 1. Se declaran los siguientes componentes y variables */
    /* Componentes del escritorio */
    Component barra_tareas, iconos_escritorio[ICONOS_ESCRITORIO], iconos_barra[ICONOS_BARRA_TAREAS];

    BootManageOS accion = ACCION_NINGUNO;                        /* Bandera de accion del sistema operativo */
    short apagar_bandera = 0, i;                                 /* Bandera e indice */
    char hora_actual[FORMATO_HORA], fecha_actual[FORMATO_FECHA]; /* Formatos de hora */
    short mouse_x, mouse_y;                                      /* Posiciones del mouse */

    /* 2. Se inicializa las configuraciones de texto y del tiempo */
    colocar_configuraciones();
    obtener_fecha_hora(hora_actual, fecha_actual);

    /* 3. Inicializar componentes */
    constructor_componente( /* Barra de tareas */
        &barra_tareas, 0,
        HEIGHT - ALTURA_BARRA_TAREAS,
        WIDTH, HEIGHT,
        COLOR_BARRA_TAREAS, COLOR_BARRA_TAREAS,
        COLOR_BARRA_TAREAS, HOVER_NO_DISPONIBLE
    );

    /* Iconos de escritorio (posiciones) */
    for (i = 0; i < ICONOS_ESCRITORIO; i++) {
        /* Declaracion de variables de posicion local */
        short icono_x, icono_y;

        /* Se inicializan en funcion de la disposicion de iconos mencionada en la
        funcion renderizar_escritorio */
        if (i < 2)  /* Primera columna */
        {
            icono_x = INICIO_ICONOS_X_1;
            icono_y = INICIO_ICONOS_Y + (i * SEPARACION_ICONOS_Y);
        }
        else { /* Segunda columna */
            icono_x = INICIO_ICONOS_X_2;
            icono_y = INICIO_ICONOS_Y + ((i - 2) * (i < 4 ? SEPARACION_ICONOS_Y : SEPARACION_ICONOS_Y_ALT));
        }

        /* Se construye cada componente relacionado a los iconos de
        escritorio */
        constructor_componente(
            &iconos_escritorio[i],
            icono_x, icono_y,
            icono_x + TAMANO_ICONOS_ESCRITORIO,
            icono_y + TAMANO_ICONOS_ESCRITORIO,
            0, 0, 0, HOVER_NO_DISPONIBLE
        );
    }

    /* Iconos de barra de tareas (solo posiciones) */
    for (i = 0; i < ICONOS_BARRA_TAREAS; i++)
    {
        /* Se declara variable de posicion de los iconos y se calcula esa posicion */
        short icono_x = POS_X_BARRA_TAREAS + ((i * TAMANO_ICONOS_BARRA_TAREAS) + (PADDING_BARRA_TAREAS * i));

        /* Se construye cada posicion de los componentes de iconos de barra
        de tareas */
        constructor_componente(
            &iconos_barra[i],
            icono_x, POS_Y_BARRA_TAREAS,
			icono_x + TAMANO_ICONOS_BARRA_TAREAS,
            HEIGHT - 3, 0,
            0, 0, HOVER_NO_DISPONIBLE
        );
    }

    /* 5. Renderizar escritorio completo */
	renderizar_escritorio(&barra_tareas);
    mver(); /* Se activa el mouse */

    /* 6. Bucle principal de operaciones */
    do
    {
        /* Mostrar el tiempo en la barra de tareas */
        mostrar_tiempo_escritorio(hora_actual, fecha_actual);

        /* Se detecta si se hizo click para evaluar si se hizo click
        en alguna app del sistema operativo o accion de la barra de
        tareas */
        if (mclick() == 1)
        {
            /* Se captura la posicion del mouse actual */
            mouse_x = mxpos(); mouse_y = mypos();

            /* Se verifica si se hizo click en alguna de las aplicaciones del
            sistema operativo */
            for (i = 0; i < ICONOS_ESCRITORIO; i++) {
                /* 3. Se detecta el click a alguno de los componentes */
                if (mouse_sobre_componente(&iconos_escritorio[i], mouse_x, mouse_y))
                {
                    mocultar(); /* Se oculta el mouse */

                    /* 4. En ese caso selecciona a la app que se toco para ejecutarse */
                    switch (i) {
                        case SISTEMA:
                            break;
                        case CONFIGURACIONES:
                            app_configuraciones();
                            mocultar();
                            renderizar_escritorio(&barra_tareas);
                            break;
                        case CALCULADORA:
                            break;
                        case BLOC_DE_NOTAS:
                            break;
                        case PAINT:
                            break;
                        }

                    mver(); /* Se hailita el mouse */

                    /* Una vez realizada las operaciones, se detiene el bucle */
                    break;
                }
            }

            /* Verificar clicks en las acciones de la barra de tareas */
            for (i = 0; i < ICONOS_BARRA_TAREAS; i++) {
                if (mouse_sobre_componente(&iconos_barra[i], mouse_x, mouse_y))
                {
                    /* 3. En ese caso se realiza las siguientes acciones dependiendo de la
                    seleccionada */
                    switch (i) {
                        case ICONO_APAGAR:
                            apagar_bandera = 1;
							accion = ACCION_APAGAR;
                            break;
                        case ICONO_REINICIAR:
                            break;
                        case ICONO_SUSPENDER:
                            break;
                        case ICONO_CREDITOS:
                            break;
                    }

                    /* Una vez realizada las operaciones, se detiene el bucle */
                    break;
                }
            }
        }

        delay(100); /* Para evitar saturacion del rendimiento se implementa un retraso */
    } while (!apagar_bandera); /* Detectar cuando se detendra el escritorio */

    mocultar();
	return accion;
}
#endif