#ifndef BOOTANIM_H
#define BOOTANIM_H

/*
	BOOTANIM_H

    Libreria que simula animaciones del arranque del sistema operativo. Estas animaciones
    se basan en la carga inicial y en el apagado del equipo. Las animaciones disponibles
    son las siguientes:
    - Animacion de arranque.
    - Animacion de bienvenida.
    - Animacion de apagado.

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
#include <math.h>

/*
    =======================================
    Importacion de Librerias Personalizadas
    =========================================
*/
#include "gphadmin.h" /* Libreria que administra el modo grafico y el sistema operativo */
#include "raster.h"   /* Libreria que contiene funciones para manejar la rasterizacion de imagenes */
#include "dynamic.h"  /* Libreria que ofrece funciones para controlar memoria dinamica */

/*
    =======================================
    Declaracion de Directivas
    =========================================
*/
/* Animacion de arranque */
#define POSICION_Y_ARRANQUE  160    /* Posicion en Y de "Iniciando NeoRetro OS..." */ 
#define LETRAS_ARRANQUE      25     /* Letras de "Iniciando NeoRetro OS..." */
#define ITERACIONES_ARRANQUE 2      /* Iteraciones de "Iniciando NeoRetro OS..." */

/* Animacion de bienvenida */
#define CANTIDAD_PASTELES_FLASH 4   /* Cantidad de colores pasteles para flash de bienvenida */
#define CANTIDAD_TONOS_AZUL     6   /* Cantidad de tonos azules para fondo de bienvenida */
#define CANTIDAD_LETRAS         11  /* Cantidad de letras del titulo "NeoRetro OS" */
#define MAXIMO_ITERACIONES      20  /* Iteraciones pertenecientes a la animacion del titulo */

/* Animacion de apagado */
#define LETRAS_APAGANDO      24     /* Cantidad de letras de "Apagando NeoRetro OS..." */

/* Retrasos */
#define RETRASO_LARGO 1000          /* Retraso correspondiente con el muestreo largo del titulo */
#define RETRASO_CORTO 200           /* Retraso correspondiente para animaciones fluidas */

/*
    =======================================
    Declaracion de Prototipos
    =========================================
*/

/* Seccion 1: Complementos */
void dibujar_fondo_gradiente(void);

/* Seccion 2: Animaciones */
void animacion_arranque(void);
void pantalla_bienvenida(void);
void apagar_sistema_operativo(void);

/*
    =======================================
    Inicializacion de Funciones
    =========================================
*/

/* Seccion 1: Complementos */
/*
    dibujar_fondo_gradiente();
    Funcion que dibuja un fondo de gradiente azul la cual sera utilizado
    para la bienvenida y el apagado.
*/
void dibujar_fondo_gradiente(void)
{
	/* Paleta de tonos azules */
    short tonos_azul[CANTIDAD_TONOS_AZUL] = {176, 176, 176, 105, 105, 1};

    /* Variable indice */
    short i;

    /* A traves del bucle y los tonos azules se crea el fondo con gradiente azul */
    for (i = 0; i < CANTIDAD_TONOS_AZUL; i++)
    {
        /* Se configura cada uno de los 6 tonos de azules */
        setcolor(tonos_azul[i]);
        setfillstyle(SOLID_FILL, tonos_azul[i]);

        /* Luego, son dibujados en 6 barras verticales congruentes de
        izquierda a derecha en la pantalla */
        bar(
            i * (WIDTH / CANTIDAD_TONOS_AZUL),
            0,
            (i + 1) * (WIDTH / CANTIDAD_TONOS_AZUL + 2),
            HEIGHT
        );
    }
}

/* Seccion 2: Animaciones */
/*
    animacion_arranque();
    Funcion que anima el arranque del sistema operativo, mostrando el logo
    de la UNI y un titulo que indica la inicializacion del sistema operativo.
*/
void animacion_arranque(void)
{
    /* 1. Se declaran las variables a utilizar */
    /* Buffers */
    FILE *file; /* Buffer de fichero */

    /* Titulos y letras */
    char titulo_iniciando[LETRAS_ARRANQUE] = "Iniciando NeoRetro OS...";
    char titulo_progresivo[LETRAS_ARRANQUE] = {0};
    
    /* Indexacion */    
    short i, iteracion;
    
    /* 2. Se dibuja el logo de la UNI a traves de ficheros */
    set_bg(BLACK);
    file = fopen("unilogo.bin", "rb");
    if (validar_archivo(file)) return;
    
    dibujar_con_rasterizado_pos(file, CENTER, 60, 36, WIDTH, HEIGHT);
    fclose(file);
    delay(RETRASO_LARGO * 4); /* Retardo para esperar */
    
    /* 3. Animacion: Construir titulo */
    /* Colocar configuraciones originales */
    colocar_configuraciones();

    for (iteracion = 0; iteracion < ITERACIONES_ARRANQUE; iteracion++)
    {
        for (i = 0; i < LETRAS_ARRANQUE; i++)
        {
            /* Poner color para texto en blanco */
            setcolor(WHITE);
            
            /* Ubicar letra por letra el titulo de inicializacion */
            titulo_progresivo[i] = titulo_iniciando[i];
            
            /* Luego dibujarla */
            outtextxy((WIDTH - textwidth(titulo_progresivo)) / 2, POSICION_Y_ARRANQUE, titulo_progresivo);
            
            /* Esperar para luego redibujar */
            delay(RETRASO_CORTO - 50);
            
            /* Borrar titulo */
            setcolor(BLACK);
            outtextxy((WIDTH - textwidth(titulo_progresivo)) / 2, POSICION_Y_ARRANQUE, titulo_progresivo);
        }

        /* Reiniciar cadena progresiva */
        for (i = 0; i < LETRAS_ARRANQUE; i++)
            titulo_progresivo[i] = 0;
    }

    /* 4. Colocar titulo original de arranque + retardo */
    setcolor(WHITE);
    outtextxy((WIDTH - textwidth(titulo_iniciando)) / 2, POSICION_Y_ARRANQUE, titulo_iniciando);
    delay(RETRASO_LARGO);
}

/*
    pantalla_bienvenida();
    Funcion que anima la pantalla de bienvenida, mostrando una animacion
    del titulo del sistema operativo "NeoRetro OS".
*/
void pantalla_bienvenida(void)
{
    /* 1. Se declaran las variables a utilizar */
    /* Titulo y cadenas */
    char titulo_os[] = "NeoRetro OS";
    char letra_actual[2];
    
    /* Paletas de colores */
    short pasteles_flash[CANTIDAD_PASTELES_FLASH] = {97, 92, 89, 101};
    short arcoiris_titulo[CANTIDAD_LETRAS] = {40, 44, 45, 50, 55, 34, 37, 39, 40, 44, 48};
    
    /* Buffers y tamanos */
    short **buffers_fondo;
    short tamano_buffer;
    short ancho_letra, alto_letra;
    
    /* Posiciones */
    short posicion_x, posicion_y;
    short posicion_x_actual;
    short desplazamiento_y[CANTIDAD_LETRAS] = {
        0, 1, 2, 1, 0, -1, -2, -1, 0, 1, 2
    };
    
    /* Indexacion e interaciones */
    short i, iteracion;

    /* Incrementos */
    short incrementos_y[CANTIDAD_LETRAS] = {
        1, 1, -1, -1, -1, -1, 1, 1, 1, 1, -1
    };

    /* 2. Inicializar cadena para mostrar el titulo por letra */
    letra_actual[1] = '\0';

	/* 3. Configurar y mostrar fondo de flasheo */
	srand((unsigned) time(NULL)); /* Antes se genera un numero aleatorio para elegir aleatoriamente el color del flash */
	set_bg(pasteles_flash[rand() % CANTIDAD_PASTELES_FLASH]);
    delay(RETRASO_CORTO);

    /* 4. Crear fondo con gradiente azul */
    dibujar_fondo_gradiente();

    /* 5. Configurar texto para ser animado */
    setcolor(WHITE);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);

    /* 6. Calcular posicion central del titulo */
    /* Esto se hace con el objetivo de mostrar el titulo por el centro y ubicar
    las letras correctamente en esta posicion, la cual se divide la diferencia
    entre el tamano total y la dimension del texto */
    posicion_x = (WIDTH - textwidth(titulo_os)) / 2;
    posicion_y = (HEIGHT - textheight(titulo_os)) / 2;

    /* 7. Reservar memoria para los buffers de fondo */
    buffers_fondo = (short **) malloc(sizeof(short *) * CANTIDAD_LETRAS);
    if (buffers_fondo == NULL) return; /* Validar antes de usar */
    
    /* 8. Se obtiene las dimensiones de la letra actual */
    letra_actual[0] = 'N'; /* Se puede usar cualquier letra como referencia por ser un mapa de bits 8x8 */
    ancho_letra = textwidth(letra_actual);
    alto_letra = textheight(letra_actual);
    
    /* 9. Calcular tamano del buffer para cada letra */
    tamano_buffer = imagesize(0, 0, ancho_letra, alto_letra);
    
    /* 10. Inicializar buffers... */
    for (i = 0; i < CANTIDAD_LETRAS; i++)
    {
        /* ... a traves de malloc para guardar las partes de los fondos
        que seran tapados por las letras */
        buffers_fondo[i] = (short *) malloc(tamano_buffer);

        /* Validar si la reserva se hizo correctamente */
        if (buffers_fondo[i] == NULL) 
        {
            liberar_arreglo((void *) buffers_fondo, CANTIDAD_LETRAS);
            return;
        }
    }

    /* 11. Se captura las posiciones en donde se dibujaran las letras */
    posicion_x_actual = posicion_x;
    for (i = 0; i < CANTIDAD_LETRAS; i++)
    {
        /* Capturar el fondo en la posicion donde estara la letra */
        getimage(
            posicion_x_actual, 
            posicion_y, 
            posicion_x_actual + ancho_letra, 
            posicion_y + alto_letra,
            buffers_fondo[i]
        );
        
        /* Guardar la letra actual para despues medir su ancho */
        letra_actual[0] = titulo_os[i];
        
        /* Medir ancho y avanzar posicion para la siguiente letra */
        posicion_x_actual += textwidth(letra_actual);
    }

    /* 12. Animacion: agitar letras */
    for (iteracion = 1; iteracion <= MAXIMO_ITERACIONES; iteracion++)
    {
        /* Se captura la posicion en x de la letra actual */
        posicion_x_actual = posicion_x;

        /* Dibujar cada letra en nueva posicion */
        for (i = 0; i < CANTIDAD_LETRAS; i++)
        {
            /* Restaurar el fondo original */
            putimage(
                posicion_x_actual,
                posicion_y + desplazamiento_y[i],
                buffers_fondo[i],
                COPY_PUT
            );
            
            /* Cambiar sentido de incremento en y (responsable de dirigir el agite de las letras) */
            if ((desplazamiento_y[i] + incrementos_y[i]) > 1
                || (desplazamiento_y[i] + incrementos_y[i]) < -1)
                incrementos_y[i] *= -1;

            /* Desplazar posicion y en base a nuevo incremento */
            desplazamiento_y[i] += incrementos_y[i];

            /* Dibujar letra en nueva posicion */
            letra_actual[0] = titulo_os[i];
            setcolor(arcoiris_titulo[(i + iteracion) % CANTIDAD_LETRAS]); /* Ajustar carrusel de colores */
            outtextxy(posicion_x_actual, posicion_y + desplazamiento_y[i], letra_actual);
            
            /* Avanzar posicion para la siguiente letra */
            posicion_x_actual += textwidth(letra_actual);
        }

        /* Retraso corto para producir animacion fluida */
        delay(RETRASO_CORTO / 2);
    }

    /* 13. Recuperar fondo y ubicar texto  */
    setcolor(WHITE);
    posicion_x_actual = posicion_x;
    for (i = 0; i < CANTIDAD_LETRAS; i++)
    {
        /* Restaurar el fondo original */
        putimage(
            posicion_x_actual,
            posicion_y + desplazamiento_y[i],
            buffers_fondo[i],
            COPY_PUT
        );
        
        /* Avanzar posicion para la siguiente letra */
        posicion_x_actual += textwidth(letra_actual);
    }
    outtextxy(posicion_x, posicion_y, titulo_os); /* Colocar titulo en blanco */

    /* 14. Animacion: Animar letras con cambio de colores de izquierda a derecha */
    posicion_x_actual = posicion_x;
    for (i = 0; i < CANTIDAD_LETRAS; i++)
    { 
        /* Dibujar la letra actual */
        letra_actual[0] = titulo_os[i];
        setcolor(i < 8 ? arcoiris_titulo[i] : WHITE); /* El 8 es la posicion de la ultima letra de "NeoRetro" */
        outtextxy(posicion_x_actual, posicion_y, letra_actual);
        
        /* Avanzar posicion para la siguiente letra */
        posicion_x_actual += textwidth(letra_actual);
        delay(RETRASO_CORTO);
    }

    /* 15. Poner texto original en blanco */
    setcolor(WHITE);
    outtextxy(posicion_x, posicion_y, titulo_os);
    delay(RETRASO_LARGO + 2000); /* Retraso de espera */

    /* 16. Liberar memoria de buffers de fondo */
    liberar_arreglo((void *) buffers_fondo, CANTIDAD_LETRAS);
}

/*
    apagar_sistema_operativo();
    Funcion que simula la animacion de apagado del sistema operativo.
    Se muestra un titulo que indica que se esta apagando el sistema operativo,
    luego se muestra una pantalla negra durante dos segundos la cual es 
    el ultimo paso del cierre.
*/
void apagar_sistema_operativo(void)
{
    /* 1. Declaracion de variables */
    /* Titulos */
    char titulo_apagando[LETRAS_APAGANDO] = "Apagando NeoRetro OS...";

    /* Buffers y tamanos */
    short *buffer_fondo;
    short tamano_buffer;
    short ancho_titulo, alto_titulo;

    /* Posiciones */
    short pos_x_actual;

    /* Indexacion e iteraciones */
    short i, iteracion;

    /* Dibujar fondo con gradiente azul */
    dibujar_fondo_gradiente();

    /* Configurar texto para ser animado */
    setcolor(WHITE);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);

    /* Encontrar dimensiones */
    ancho_titulo = textwidth(titulo_apagando);
    alto_titulo = textheight(titulo_apagando);

    /* Reservar memoria */
    tamano_buffer = imagesize(0, 0, ancho_titulo, alto_titulo);
	buffer_fondo = (short *) malloc(tamano_buffer);
    if (buffer_fondo == NULL) return;

    /* Capturar zona del titulo a ubicar para redibujar */
    getimage(
        (WIDTH - ancho_titulo) / 2,
        (HEIGHT - alto_titulo) / 2,
        (WIDTH - ancho_titulo) / 2 + ancho_titulo,
        (HEIGHT - alto_titulo) / 2 + alto_titulo,
        buffer_fondo
    );

    /* Animacion: Dibujar animacion de titulo apagandose con tres puntos suspensivos */
    for (iteracion = 0; iteracion < 5; iteracion++)
    {
        /* Quitar puntos suspensivos */
        titulo_apagando[LETRAS_APAGANDO - 4] = 0;
        titulo_apagando[LETRAS_APAGANDO - 3] = 0;
        titulo_apagando[LETRAS_APAGANDO - 2] = 0;
         
        for (i = 4; i >= 1; i--)
        {
            /* Capturar ancho de titulo */
            pos_x_actual = textwidth(titulo_apagando);

            /* Mostrar titulo apagado + retardo corto */
            outtextxy((WIDTH - pos_x_actual) / 2, (HEIGHT - alto_titulo) / 2, titulo_apagando);
            delay(RETRASO_CORTO * 2);
            
            /* Redibujar zona de fondo afectada */
            putimage(
                (WIDTH - ancho_titulo) / 2,
                (HEIGHT - alto_titulo) / 2,
                buffer_fondo,
                COPY_PUT
            );
            
            /* Agregar punto suspensivo */
            if(i > 1) titulo_apagando[LETRAS_APAGANDO - i] = '.';
        }
    }
    
    /* Liberar memoria */
    liberar_elemento(buffer_fondo);

    /* Poner fondo negro durante unos segundos */
    set_bg(BLACK); delay(2000);
}
#endif