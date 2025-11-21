#ifndef BLOCK_H
#define BLOCK_H

/*
	BLOCK_H
    Libreria encargada de gestionar la pantalla de bloqueo del sistema
    operativo a traves de dos funciones principales de gestion.

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
#include "gphadmin.h"   /* Libreria administradora del modo grafico y del sistema operativo */
#include "raster.h"     /* Libreria que maneja el rasterizado de imagenes */
#include "comp.h"       /* Libreria de componentes, el corazon de las interfaces */
#include "timecont.h"   /* Libreria para gestionar de forma facil la fecha y hora */
#include "desktop.h"    /* Libreria que administra el escritorio del sistema operativo */

/*
    =======================================
    Declaracion de Directivas
    =========================================
*/
/* Colores */
#define FONDO_BLOQUEO_INICIAL 247
#define FONDO_BLOQUEO_SESION  274
#define PALETA_AZUL_BLOQUEO   6

/* Tamanos y cantidades */
#define TAMANO_CUADRO         6
#define MARGEN_ESQUINAS       8
#define CANTIDAD_LINEAS       5
#define ICONOS_BLOQUEO_SESION 3

/* Retardos manuales */
#define RETARDO_INICIAL 30
#define RETARDO_SESION  3000

/*
    =======================================
    Declaracion de Variables Globales
    =========================================
*/
short paleta_azul_esquinas[PALETA_AZUL_BLOQUEO] = {
    52, 53, 54, 55, 32, 33
}; /* Paleta de tonos azules para las esquinas animadas */

/*
    =======================================
    Definicion de Enums
    =========================================
*/

/* 
    enum BlockActions
    Estructura enum que indica acciones pero especificas para la
    pantalla de bloqueo.
*/
typedef enum {
    INICIAR,
    RETROCEDER
} BlockActions;

/*
    =======================================
    Declaracion de Prototipos
    =========================================
*/
/* Seccion 1: Complementos para pantalla de bloqueo inicial */
void mostrar_tiempo_bloqueo(char *buffer_hora, char *buffer_fecha, char *buffer_fecha_larga);
void degradado_esquinas(void);
void dibujar_titulo_esquinas_bloqueo(void);

/* Seccion 2: Complementos para pantalla de inicio de sesion */
void icono_iniciar_sesion(void);
void iconos_accion_sesion(Component *apagar, Component *reiniciar, Component *suspender);

/* Seccion 3: Pantallas principales */
void pantalla_bloqueo_inicial(void);
BootManageOS pantalla_bloqueo_sesion(void);

/*
    =======================================
    Inicializacion de Funciones
    =========================================
*/
/* Seccion 1: Complementos para pantalla de bloqueo inicial */
/*
    mostrar_tiempo_bloqueo();
    Funcion que muestra la hora y fecha larga en graficos para ser utilizada
    en la pantalla de bloqueo inicial.

    Parametros:
    char *buffer_hora: Buffer correspondiente a la hora actual.
    char *buffer_fecha: Buffer conrrespondiente a la fecha actual.
    char *buffer_fecha_larga: Buffer la cual es la fecha actual pero en su formato largo.
*/
void mostrar_tiempo_bloqueo(char *buffer_hora, char *buffer_fecha, char *buffer_fecha_larga)
{
    /* 1. Se verifica si el tiempo cambia, en ese caso se modifican los buffers de tiempo */
	if (verificar_cambio_tiempo(buffer_hora, buffer_fecha))
    {
        /* Pero antes se elimina el tiempo anterior de la pantalla */
        setfillstyle(SOLID_FILL, FONDO_BLOQUEO_INICIAL);
        bar(30, 160,
            30 + textwidth(buffer_fecha_larga),
            160 + textheight(buffer_fecha_larga) + textheight(buffer_hora));
    }

    /* 2. Ahora se dibuja la hora y fecha larga */
    setcolor(WHITE);
    outtextxy(30, 160, buffer_hora);
    outtextxy(30, 170, buffer_fecha_larga);
}

/*
    degradado_esquinas();
    Funcion que muestra esquinas decoradas para la pantalla de bloqueo inicial y de inicio 
    sesion.
*/
void degradado_esquinas(void)
{
    /* 1. Declaracion de variabes */
	static short pos_actual = 0; /* Variable estatica correspondiente con el indice de colores (estatico) */
    short linea, cuadro; /* Indices de cuadros  */

    /* 
        Para la implementacion de esta animacion se esta simulando lineas muy grandes que cambian
        de colores, para ello cada linea contiene un conjunto de cuadros (su cantidad varia
        del numero de linea), para ello se realizan calculos geometricos para ubicar los cuadros
        en sus respectivas lineas sin concurrir mucho a bar. Esto se logra tambien a traves de un
        bucle anidado.
    */
    /* 2. Se crea esta cantidad de lineas */
    for (linea = 1; linea <= CANTIDAD_LINEAS; linea++)
    {
        /* 3. Dependiendo del numero de linea se creara esta cantidad de cuadros en las dos esquinas */
        for (cuadro = 1; cuadro <= linea; cuadro++)
        {
            /* 4. Se ajusta el color y estilo en dependencia de la posicion y linea actual */
			setfillstyle(SOLID_FILL, paleta_azul_esquinas[(linea + pos_actual) % PALETA_AZUL_BLOQUEO]);
            mocultar(); /* Se oculta el raton para evitar problemas de dibujado */

            /* 5. Se dibuja y anima la esquina inferior izquierda */
            bar(
                TAMANO_CUADRO * (cuadro - 1),
                HEIGHT - (TAMANO_CUADRO * (linea - cuadro + 1)), 
                TAMANO_CUADRO * cuadro, 
                HEIGHT - (TAMANO_CUADRO * (linea - cuadro))
            ); 

            /* 6. Se dibuja y anima la esquina superior derecha */
            bar(
                WIDTH - TAMANO_CUADRO * (cuadro - 1), 
                (TAMANO_CUADRO * (linea - cuadro + 1)), 
                WIDTH - TAMANO_CUADRO * cuadro, 
                (TAMANO_CUADRO * (linea - cuadro))
            );
            mver();
        }
    }

    /* 7. Una vez hecha la animacion se verifica si la posicion no excede la cantidad
    de lineas para reiniciar la animacion, sino solo se incrementa */
    if (pos_actual < CANTIDAD_LINEAS) pos_actual++;
    else pos_actual = 0;
}

/*
    dibujar_titulo_esquinas_bloqueo();
    Funcion que muestra el titulo del sistema operativo NeoRetro OS y esquinas
    lineales blancas en la parte superior izquierda e inferior derecha durante
    la pantalla de bloqueo inicial.
*/
void dibujar_titulo_esquinas_bloqueo(void)
{
    /* 1. Se declaran las variables a utilizar */
    char titulo_os[] = "NeoRetro OS";   /* Cadena correspondiente al titulo del sistema operativo */
    short esquinas_origenes[4];         /* Arreglo que almacena los puntos claves de las esquinas blancas */
    short esquina;                      /* Variable de iteracion correspondiente a estas esquinas */

    /* 2. Se obtienen los puntos claves de las esquinas */
    esquinas_origenes[0] = 10;                                              /* Punto p1 (x1) */
    esquinas_origenes[1] = HEIGHT - (170 + textheight(titulo_os)) - 10;     /* Punto p1 (y1) */
    esquinas_origenes[2] = WIDTH - esquinas_origenes[0];                    /* Punto p2 (x2) */
    esquinas_origenes[3] = HEIGHT - esquinas_origenes[1];                   /* Punto p2 (y2) */

    /* 3. Se crean las dos esquinas blancas */
    for (esquina = 0; esquina < 2; esquina++)
    {
        /* 4. Se declara otra variable correspondiente con el sentido del margen */
        short margen = MARGEN_ESQUINAS * (esquina == 0 ? 1 : -1);

        /* 5. Se dibuja la primera linea adyacente */
        line(
            esquinas_origenes[esquina * 2],
            esquinas_origenes[(esquina * 2) + 1],
            esquinas_origenes[esquina * 2] + margen,
            esquinas_origenes[(esquina * 2) + 1]
        );

        /* 6. Se dibuja la segunda linea adyacente */
        line(
            esquinas_origenes[esquina * 2],
			esquinas_origenes[(esquina * 2) + 1],
            esquinas_origenes[esquina * 2],
            esquinas_origenes[(esquina * 2) + 1] + margen
        );
    }

    /* 7. Finalmente se muestra el titulo del sistema operativo en la pantalla
    de bloqueo inicial */
    outtextxy(30, HEIGHT - (170 + textheight(titulo_os)), titulo_os);
}

/* Seccion 2: Complementos para pantalla de inicio de sesion */
/*
    icono_iniciar_sesion();
    Funcion encargada de mostrar el icono de usuario en la pantalla de bloqueo 
    de inicio de sesion mas el nombre de usuario por defecto.
*/
void icono_iniciar_sesion(void)
{
    /* 1. Se declara variables para las operaciones */
	char nombre_usuario[] = "UsuarioOS"; /* Nombre de usuario predeterminado */
    short centro_icono[2];               /* Punto correspondiente al centro del icono */

    /* 2. Se obtiene el centro del icono */
    centro_icono[0] = WIDTH / 2;
    centro_icono[1] = HEIGHT / 4 + 10;

    /* 3. Dibujar borde de icono (con proporcionalidad para solucionar el problema
    de la resolucion 320x200) */
	ellipse(
        centro_icono[0],        /* Punto p (x) */
        centro_icono[1],        /* Punto p (y) */
        0, 360,                 /* Angulos del elipse que forman un circulo */
        42 * (WIDTH / HEIGHT),  /* Radio en x * Proporcionalidad para arreglar relatividad */
        42                      /* Radio en y */
    );

    /* 4. Dibujar icono (que es una silueta de una persona) */
    ellipse( /* Torso */
        centro_icono[0], 
        centro_icono[1] + 30, 
        0, 180, 
        20 * (WIDTH / HEIGHT), 
        20
    ); 
    ellipse( /* Cabeza */
        centro_icono[0], 
        centro_icono[1] - 10, 
        0, 360, 
        13 * (WIDTH / HEIGHT), 
        13
    ); 

    /* 5. Colocar nombre de usuario */
    outtextxy(
        (WIDTH - textwidth(nombre_usuario)) / 2, 
        HEIGHT / 4 + 56,
        nombre_usuario
    );
}

/*
    iconos_accion_sesion()
    Funcion que renderiza los iconos relacionado a las acciones de arranque del
    sistema operativo, siendo los siguientes:
    - Apagar
    - Reiniciar
    - Suspender

    Parametros:
    - Component apagar: Componente del boton de apagado, utilizado para obtener las
    coordenadas de este componente.
    - Component reiniciar: Componente del boton de reinicio, utilizado para obtener
    las coordenadas de este componente.
    - Component suspender: Componente del boton de suspender, utilizado para obtener
    las coordenadas de este componente.
*/
void iconos_accion_sesion(Component *apagar, Component *reiniciar, Component *suspender)
{
    /* 1. Se declaran los nombres de los ficheros de los iconos a renderizar */
    char *iconos_ficheros[] = {"SHUTVV.bin", "RESVV.bin", "SUSVV.bin"};
    TaskBarIcons icono;
    short pos_x, pos_y;

    /* 2. Se declara un apuntador a FILE para trabajar con los iconos */
    FILE *fichero;

    /* 3. Se renderiza cada uno de los iconos */
    for (icono = 0; icono < ICONOS_BLOQUEO_SESION; icono++)
    {
        /* 4. Se abre cada fichero de icono como lectura */
        fichero = fopen(iconos_ficheros[icono], "rb");

        /* 5. Se selecciona la posicion en el que va a ser dibujada */
        switch(icono)
        {
            case ICONO_APAGAR:
                pos_x = apagar->x1;
                pos_y = apagar->y1;
                break;
            case ICONO_REINICIAR:
                pos_x = reiniciar->x1;
                pos_y = reiniciar->y1;
                break;
            case ICONO_SUSPENDER:
                pos_x = suspender->x1;
                pos_y = suspender->y1;
                break;
        }

        /* 6. Se dibuja cada icono con los datos seleccionados */
        dibujar_raster_png_coords(fichero, pos_x, pos_y, 20, 20);

        /* 7. Y se cierra el fichero actual */
        fclose(fichero);
    }
}

/* Seccion 3: Pantallas principales */
/*
    pantalla_bloqueo_inicial();
    Funcion encargada de mostrar la pantalla de bloqueo inicial, una vez
    iniciado el sistema operativo.
*/
void pantalla_bloqueo_inicial(void)
{
    /* 1. Declaracion de variables */
    /* Bandera de salida */
    short salir = 0;

    /* Contador de retardo (para esquinas animadas) */
    short contador_retardo = 0;

    /* Formatos de tiempo (hora/fecha/fecha_larga) */
    char hora[FORMATO_HORA] = {0};
    char fecha[FORMATO_FECHA] = {0};
    char fecha_larga[FORMATO_FECHA_LARGA] = {0};

    /* 2. Ajustar las configuraciones por defecto, y rellenar fondo */
    colocar_configuraciones();
	set_bg(FONDO_BLOQUEO_INICIAL);
    setcolor(WHITE);

    /* 3. Se obtiene las horas actuales */
    obtener_fecha_hora(hora, fecha);
    obtener_fecha_larga(fecha_larga);

    /* 4. Se dibujan los elementos correspondientes a esta seccion
    (titulo, esquinas blancas, esquinas animadas) */
    dibujar_titulo_esquinas_bloqueo();  /* Titulo + Esquinas Blancas */
    degradado_esquinas();               /* Esquina animada */

    /* 5. Se limpia el buffer de entradas de teclado para no cerrar esta funcion */
    while (kbhit()) getch();

    /* 6. Activar raton */
    mver();

    /* ==== FUNCIONAMIENTO DE PANTALLA DE BLOQUEO INICIAL ==== */
	do {
        /* 7. Se redibuja la hora y fecha larga para actualizarse con cada minuto
        que pase */
        mostrar_tiempo_bloqueo(hora, fecha, fecha_larga);

        /* 8. Se implementa un retardo manual para la animacion de esquinas, con el objetivo
        de no redibujar muy rapido */
        if (contador_retardo == RETARDO_INICIAL)
        {
            degradado_esquinas();   /* Si llega a tal punto */
            contador_retardo = 0;   /* Y se reinicia el contador */
        }

        /* 9. Se espera si se da click izquierdo o se toca un boton
        de la tecla para pasar a la siguiente pantalla de bloqueo (inicio de sesion) */
        if (mclick() == 1 || kbhit())
			salir = 1;  /* Se activa la bandera de salida */

        /* 10. En todo caso, el retardo manual incrementa*/
        contador_retardo++;
	} while (!salir); /* Se espera hasta que el usuario haga lo anterior mencionado */

    /* 11. Se oculta el raton para no alterar el siguiente dibujado */
    mocultar();
}

/*
    pantalla_bloqueo_sesion();
    Funcion que administra la funcionalidad visual e interactiva de la pantalla
    de bloqueo de inicio de sesion. Utiliza los componentes para agregar funcionalidad
    a esta pantalla, siendo esas:
        - Boton para iniciar.
        - Boton para apagar.
        - Boton para reiniciar.
        - Boton para suspender.

    Devuelve:
        - -1 si no se presiono ninguna de las opciones.
        - 0/INICIAR si se presiona el boton/componente de iniciar.
        - 1/APAGAR si se presiona el boton/componente de apagar.
        - 2/REINICIAR si se presiona el boton/componente de reiniciar.
        - 3/SUPENDER si se presiona el boton/componente de suspender.
*/
BootManageOS pantalla_bloqueo_sesion(void)
{
    /* 1. Declaracion de componentes y variables */
    /* Componentes para pantalla de bloqueo de inicio de sesion */
    Component iniciar_sesion, apagar, reiniciar, suspender;

    /* Banderas */
    short salir = 0;    /* Bandera de salida */
    short destino = -1; /* Valor a retornar despues de presionar uno de los componentes */

    /* Contadores */
    short contador_retardo = 0; /* Retardo manual */

    /* 2. Ajustar las configuraciones por defecto, y rellenar fondo */
    colocar_configuraciones();
	set_bg(FONDO_BLOQUEO_SESION);
    setcolor(WHITE);

    /* 3. Inicializar componentes para pantalla de bloqueo de sesion */
    constructor_componente( /* Iniciar */
        &iniciar_sesion,
        (WIDTH) / 2 - 50,
		(HEIGHT) / 2 + 27,
        (WIDTH) / 2 + 50,
		(HEIGHT) / 2 + 39,
		274, WHITE, WHITE,
        HOVER_DISPONIBLE
    );
	constructor_hover_componente( /* Hover para Iniciar */
        &iniciar_sesion,
        274, 103, 103
    );

    constructor_componente( /* Apagar */
        &apagar,
        240, 170, 260, 190,
	    274, WHITE, WHITE,
        HOVER_DISPONIBLE
    );
    constructor_componente( /* Reiniciar */
        &reiniciar,
        265, 170, 285, 190,
		274, WHITE, WHITE,
        HOVER_DISPONIBLE
    );
    constructor_componente( /* Suspender */
        &suspender,
        290, 170, 310, 190,
		274, WHITE, WHITE,
        HOVER_DISPONIBLE
    );

    /* 4. Renderizar componente de iniciar sesion  */
	renderizar_componente_texto(&iniciar_sesion, "Iniciar");

    /* 5. Dibujar elementos extras (esquina animada + icono de iniciar sesion + iconos de accion) */
    degradado_esquinas();
    icono_iniciar_sesion();
    iconos_accion_sesion(&apagar, &reiniciar, &suspender);

    /* 6. Mostrar mouse */
    mver();

    /* ==== FUNCIONAMIENTO PRINCIPAL DE PANTALLA DE BLOQUEO DE INICIO DE SESION ====*/
	do {
        /* 7. Se captura las posiciones del mouse */
		short mouse_x = mxpos(), mouse_y = mypos();

        /* ==== ANIMACIONES + HOVERS ====*/
        /* 8. Se utiliza el retardo manual para la animacion de degradado */
        if (contador_retardo == RETARDO_SESION)
        {
            degradado_esquinas(); /* En ese caso se anima la esquina */
            contador_retardo = 0; /* Y se reinicia el contador */
        }

        /* 9. Se maneja el hover del componente Iniciar */
		manejar_hover_componente_texto(&iniciar_sesion, mouse_x, mouse_y, "Iniciar");

        /* ==== CALLBACKS ==== */
        /* 10. Detectar clicks en componentes */
        /* Caso: Componente iniciar sesion */
        if (detectar_click_componente(&iniciar_sesion, mouse_x, mouse_y))
        {
			salir = 1;       		   /* Se activa la bandera de salida */
			destino = INICIAR;         /* Se asigna a la variable de retorno */
        }
        /* Caso: Componente apagar */
        else if (detectar_click_componente(&apagar, mouse_x, mouse_y))
        {
			salir = 1;		           /* Se activa la bandera de salida */
			destino = ACCION_APAGAR;   /* Se asigna a la variable de retorno */
        }
        /* Caso: Componente reiniciar */
        else if (detectar_click_componente(&reiniciar, mouse_x, mouse_y))
        {
			salir = 1;       		    /* Se activa la bandera de salida */
			destino = ACCION_REINICIAR; /* Se asigna a la variable de retorno */
        }
        /* Caso: Componente suspender */
        else if (detectar_click_componente(&suspender, mouse_x, mouse_y))
        {
            salir = 1;                  /* Se activa la bandera de salida */
			destino = ACCION_SUSPENDER; /* Se asigna a la variable de retorno */
        }

        /* 11. Se incrementa el retardo manual */
        contador_retardo++;
	} while (!salir); /* Se espera alguna respuesta de parte de los componentes */

    /* 12. Se oculta el mouse y se devuelve el componente detectado */
    mocultar();
    return destino;
}
#endif