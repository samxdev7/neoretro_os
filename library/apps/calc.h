#ifndef CALC_H
#define CALC_H

/*
    Calculadora del sitema operativo "NeoRetro OS"
    - Esta aplicacion es una calculadora simple que permite realizar
    operaciones basicas como suma, resta, multiplicacion y division.
    - La interfaz grafica se maneja a traves de la libreria de componentes,
    permitiendo crear botones y una pantalla de visualizacion.

    - El proyecto esta diseñado para funcionar en modo grafico SVGA 256 colores
    utilizando la libreria graphics.h y oTRAS librerias personalizadas como
    appadmin.h, comp.h y dynamic.h.
    - El codigo esta estructurado en funciones de renderizado y callbacks
    para manejar la logica de la calculadora.

    - Johanna Castellon
    - Moises Aleman
    - Said Bolanos

    - Version 1.0
    Nota: Ya hay parches pensados para mejorar la calculadora en futuras versiones,
    como operaciones de raiz cuadrada, modulo, etc.
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
#include "raster.h"
#include "boolso.h"

#define SCR_NUM_COL 53
#define ERR_MSG_COL 39

#define NUM_PX_SZ 7
#define MAX_PANTALLA 9
#define MAX_RESULTADO_STR 15

/* constante para mover elementos uniformemente */
#define TRAS -5
#define SHRB -3
#define TRASR2 -4

/* numeros */
char *numeros_calc[] = {
    "0", "1", "2", "3", "4", "5", "6", "7", "8", "9"
};

char *operaciones_calc[] = {
	"+", "-", "*", "/"
};

short columnas_calc[] = {150, 126, 100, 75};
short filas_calc[] = {98, 133, 166};

/* operadores */
typedef enum {
    SUM_CHAR = '+',
    RES_CHAR = '-',
    MULTI_CHAR = '*',
    DIVI_CHAR = '/'
}operaciones_t;

/* STRUCTS */
typedef struct {
    double num1;
    double num2;
    double resultado;
    char resultado_str[MAX_RESULTADO_STR];
    operaciones_t operacion;
    char pantalla[MAX_PANTALLA];
    int len_pantalla;
}calculadora_t;

/* VARIABLES GLOBALES */
bool estado = false;
bool decimal_usado = false;
calculadora_t calc = {0}; /* se inicializa con {0}, reservar el espacio en memoria para toda la estructura */

/*
    =======================================
    Prototipado de Funciones (declaraciones)
    =========================================
*/
void dibujar_fondo(void); /* Dibuja el fondo de la calculadora */
void dibujar_cuerpo_calc(void); /* Dibuja el cuerpo de la calculadora */
void renderizar_pantalla_calc(void); /* Limpia y prepara la pantalla para mostrar números */
void renderizar_numeros_pantalla(void); /* Muestra los números en la pantalla */
void liberar_buffer_resultado(void); /* Limpia el buffer de resultado */
void liberar_buffer_pantalla(void); /* Limpia el buffer de la pantalla */
void verficar_si_0(void); /* Verifica si la pantalla muestra un 0 */
void estado_inicial_pantalla(void); /* Establece el estado inicial de la pantalla */
void nuevo_input_pantalla(void); /* Prepara la pantalla para un nuevo input */
bool overflow_pantalla(void); /* Verifica si hay overflow en la pantalla */
void mostrar_resultado_ops(void); /* Muestra el resultado de las operaciones */
void append_numero_pantalla(char); /* Agrega un número a la pantalla */
bool check_inicial(void); /* Verifica el estado inicial */
bool solo_signo_presionado(void); /* Verifica si solo se ha presionado un signo */
bool mas_de_una_operacion(void); /* Verifica si hay más de una operación */
bool input_antes_de_signo(void); /* Verifica si hay input antes del signo */
bool dividir_por_cero(void); /* Verifica si se intenta dividir por cero */
void operacion(void); /* Realiza la operación matemática */
void renderizar_btn_9(Component *); 
void renderizar_btn_8(Component *);
void renderizar_btn_7(Component *);
void renderizar_btn_6(Component *);
void renderizar_btn_5(Component *);
void renderizar_btn_4(Component *);
void renderizar_btn_3(Component *);
void renderizar_btn_2(Component *);
void renderizar_btn_1(Component *);
void renderizar_btn_0(Component *);
void renderizar_btn_decimal(Component *);
void renderizar_btn_clear(Component *);
void renderizar_btn_divi(Component *);
void renderizar_btn_multi(Component *);
void renderizar_btn_resta(Component *);
void renderizar_btn_suma(Component *);
void renderizar_btn_igual(Component *);
void callback_btn_numero(char numero);
void callback_operador(operaciones_t operacion);
void callback_btn_igual(void);
void app_calculadora(void); /* contiene la logica principal de la calc */

/* Inicio funciones */
void dibujar_fondo(void)
{
    FILE *p; /* puntero a archivo */
	p = fopen("back.bin", "rb");
    if (validar_archivo(p)) return;

    dibujar_con_rasterizado_coords(p, 2, 12, 316, 186); /* Dibuja el fondo de la calculadora */

    fclose(p);
}

void dibujar_cuerpo_calc(void)
{
    FILE *p;
	p = fopen("CLCBDY.bin", "rb"); /* puntero a archivo, leer en modo binario */
    if (validar_archivo(p)) return;

    dibujar_raster_png_coords(p, 2, 12, 316, 186); /* Dibuja el cuerpo de la calculadora */

    fclose(p);
}

void renderizar_pantalla_calc(void) {
    setcolor(BLACK); /* Establece el color de dibujo a negro */
    setfillstyle(SOLID_FILL, BLACK); /* Establece el estilo de relleno sólido con color negro */
    bar(102, 34, 221, 59); /* Limpia la pantalla */
}

void renderizar_numeros_pantalla(void)
{ 
    renderizar_pantalla_calc(); /* Limpia y prepara la pantalla para mostrar números */
    setcolor(SCR_NUM_COL); /* Establece el color de dibujo para los números en la pantalla */
    outtextxy(215 - ((NUM_PX_SZ  * calc.len_pantalla) - (NUM_PX_SZ + calc.len_pantalla)), 43, calc.pantalla); /* Muestra los números en la pantalla, asegura que estén correctamente alineados */
}

void liberar_buffer_resultado(void) {
    int i;
    for (i = 0; i < MAX_RESULTADO_STR; i++) {
        calc.resultado_str[i] = '\0'; /* Limpia el buffer de resultado */
    }
}

void liberar_buffer_pantalla(void) {
    int i;
    for (i = 0; i < MAX_PANTALLA; i++) {
        calc.pantalla[i] = '\0'; /* Limpia el buffer de la pantalla */
    }
    calc.len_pantalla = 0;
}

void verficar_si_0(void) {
    if (strcmp(calc.pantalla, "0") == 0) /* Verifica si la pantalla muestra "0" */
        liberar_buffer_pantalla(); /* Limpia la pantalla si es así */
}

void estado_inicial_pantalla(void) {
    liberar_buffer_pantalla(); /* Limpia el buffer de la pantalla */
    liberar_buffer_resultado(); /* Limpia el buffer de resultado */
    renderizar_pantalla_calc(); /* Limpia y prepara la pantalla para mostrar números */
    setcolor(SCR_NUM_COL); /* Establece el color de dibujo para los números en la pantalla */
    outtextxy(215, 43, "0"); /* Muestra "0" en la pantalla */
}

void nuevo_input_pantalla(void) {
    decimal_usado = false; /* Reinicia el uso del decimal */
    estado_inicial_pantalla(); /* Prepara la pantalla para un nuevo input */
}

bool overflow_pantalla(void) { /* Verifica si la pantalla ha alcanzado su capacidad máxima */
		if (calc.len_pantalla >= MAX_PANTALLA - 1) {
        calc.len_pantalla = MAX_PANTALLA - 1;
        calc.pantalla[calc.len_pantalla] = '\0'; /* Asegura que la cadena esté correctamente terminada */
        return true; /* Indica que se ha producido un overflow */
    }
    return false; /* No hay overflow */
}

void mostrar_resultado_ops(void) { /* Muestra el resultado de las operaciones */
    liberar_buffer_resultado(); /* Limpia el buffer de resultado */
    renderizar_pantalla_calc(); /* Limpia y prepara la pantalla para mostrar números */
    setcolor(SCR_NUM_COL); /* Establece el color de dibujo para los números en la pantalla */

    if (calc.resultado > ((int)calc.resultado)) /* Verifica si el resultado tiene decimales */
        sprintf(calc.resultado_str, "%.2f", calc.resultado); /* Formatea el resultado con 2 decimales */
    else
        sprintf(calc.resultado_str, "%.0f", calc.resultado); /* Formatea el resultado sin decimales */
    
    outtextxy(215 - ((NUM_PX_SZ  * strlen(calc.resultado_str)) - (NUM_PX_SZ + strlen(calc.resultado_str))), 43, calc.resultado_str); /* Muestra el resultado en la pantalla */
    liberar_buffer_pantalla(); /* Limpia el buffer de la pantalla */
}

void append_numero_pantalla(char numero) {
    /*Caso del primer '.' -> "0." */
    if (numero == '.' && calc.len_pantalla == 0) {
        if (MAX_PANTALLA >= 2) { /* verificar si hay espacio suficiente */
            calc.pantalla[0] = '0';
            calc.pantalla[1] = '.';
            calc.len_pantalla = 2;
            calc.pantalla[2] = '\0';
            decimal_usado = true;
            return;
        }
    }

    if (numero == '0' && calc.len_pantalla == 0)
    {
        calc.pantalla[0] = '0';
        calc.pantalla[1] = '\0';
        calc.len_pantalla = 1;
        return; /* no permitir multiples ceros al inicio */
    }
    
    /* Capacidad efectiva: MAX_PANTALLA-1 (reserva 1 para '\0') */
    if (calc.len_pantalla >= MAX_PANTALLA - 1) return;

    calc.pantalla[calc.len_pantalla++] = numero;
    calc.pantalla[calc.len_pantalla]   = '\0';
}

/* verifica si es el primer numero ingresado a operar */
bool check_inicial(void) {
    if (calc.len_pantalla > 0 && calc.operacion == 0)
        return true; /* primer operador */
    return false; /* lleva mas de una operacion */
}

bool solo_signo_presionado(void){ /* verifica si solo se ha presionado un signo */
    if ((calc.len_pantalla == 0 && calc.resultado == 0))
        return true;
    return false; /* Indica que solo se ha presionado un signo */
}

bool mas_de_una_operacion(void) { /* Verifica si hay más de una operación en curso */
    if (calc.len_pantalla > 0 && calc.operacion != 0 && calc.resultado != 0)
        return true;
    return false; /* Indica que hay más de una operación en curso */
}

bool input_antes_de_signo(void) { /* Verifica si hay input antes del signo */
    if ((calc.resultado != 0 && calc.len_pantalla == 0 && calc.operacion == 0))
        return true;
    return false; /* Indica si hay input antes del signo */
}

bool dividir_por_cero(void) { /* Verifica si se intenta dividir por cero */
    if (calc.operacion == DIVI_CHAR && strcmp(calc.pantalla, "0") == 0)
        return false;
    return true; /* Indica si se intenta dividir por cero */
}

void operacion(void) {
    switch (calc.operacion) /* Realiza la operación según el operador seleccionado */
    {
        case SUM_CHAR:
            calc.resultado += atof(calc.pantalla);
            break;
        case RES_CHAR:
            calc.resultado -= atof(calc.pantalla);
            break;
        case MULTI_CHAR:
            calc.resultado *= atof(calc.pantalla);
            break;
        case DIVI_CHAR:
            calc.resultado /= atof(calc.pantalla);
            break;
    }
}

/* Funciones de renderizado de botones */
void renderizar_btn_9(Component *componente) {
    renderizar_componente_texto(componente, "9"); /* Renderiza el boton con el texto "9" */
}
void renderizar_btn_8(Component *componente) {
    renderizar_componente_texto(componente, "8"); /* Renderiza el boton con el texto "8" */
}
void renderizar_btn_7(Component *componente) {
    renderizar_componente_texto(componente, "7"); /* Renderiza el boton  con el texto "7" */
}
void renderizar_btn_6(Component *componente) {
    renderizar_componente_texto(componente, "6"); /* Renderiza el boton con el texto "6" */
}
void renderizar_btn_5(Component *componente) {
    renderizar_componente_texto(componente, "5"); /* Renderiza el boton con el texto "5" */
}
void renderizar_btn_4(Component *componente) {
    renderizar_componente_texto(componente, "4"); /* Renderiza el boton con el texto "4" */
}
void renderizar_btn_3(Component *componente) {
    renderizar_componente_texto(componente, "3"); /* Renderiza el boton con el texto "3" */
}
void renderizar_btn_2(Component *componente) {
    renderizar_componente_texto(componente, "2"); /* Renderiza el boton con el texto "2" */
}
void renderizar_btn_1(Component *componente) {
    renderizar_componente_texto(componente, "1"); /* Renderiza el boton con el texto "1" */
}
void renderizar_btn_0(Component *componente) {
    renderizar_componente_texto(componente, "0"); /* Renderiza el boton con el texto "0" */
}

void renderizar_btn_decimal(Component *componente) {
    renderizar_componente_texto(componente, "."); /* Renderiza el boton con el texto "." */
}

void renderizar_btn_clear(Component *componente) {
    renderizar_componente_texto(componente, "C"); /* Renderiza el boton con el texto "C" */
}

void renderizar_btn_divi(Component *componente) {
    renderizar_componente_texto(componente, "/"); /* Renderiza el boton con el texto "/" */
}

void renderizar_btn_multi(Component *componente) {
    renderizar_componente_texto(componente, "x"); /* Renderiza el boton con el texto "x" */
}

void renderizar_btn_resta(Component *componente) {
    renderizar_componente_texto(componente, "-"); /* Renderiza el boton con el texto "-" */
}

void renderizar_btn_suma(Component *componente) {
    renderizar_componente_texto(componente, "+"); /* Renderiza el boton  con el texto "+" */
}

void renderizar_btn_igual(Component *componente) {
    renderizar_componente_texto(componente, "="); /* Renderiza el boton con el texto "=" */
}

/* Callbacks de botones */
void callback_operador(operaciones_t operador)
{
    if (solo_signo_presionado())
        return;

    if (check_inicial())
    {
        calc.resultado = atof(calc.pantalla);
        calc.operacion = operador;
        nuevo_input_pantalla(); /* Prepara la pantalla para nuevo input */
        return;
    }
    else
    {
        calc.operacion = operador;
        nuevo_input_pantalla(); /* Prepara la pantalla para nuevo input */
        return;
    }
}

void callback_btn_clear(void) { /* Callback para el botón de limpiar */
    calc.resultado = 0;
    calc.operacion = 0;
    decimal_usado = false;
    liberar_buffer_resultado();
    estado_inicial_pantalla(); /* Establece el estado inicial de la pantalla */
}

void callback_btn_decimal(void) { /* Callback para el botón decimal */
    if (decimal_usado)
        return; /* evita múltiples decimales */
    if (input_antes_de_signo())
        callback_btn_clear(); /* limpia si hay input antes del signo */
        
    append_numero_pantalla('.'); /* Agrega el decimal a la pantalla */
    renderizar_numeros_pantalla();
}

void callback_btn_numero(char numero)
{
    if (input_antes_de_signo()) /* verifica si hay input antes del signo */
        callback_btn_clear(); /* limpia si hay input antes del signo */     

    append_numero_pantalla(numero); /* Agrega el número a la pantalla */
    renderizar_numeros_pantalla(); /* Muestra los números en la pantalla */
}

void callback_btn_igual(void) {
    mocultar(); /* Oculta la pantalla para mostrar el resultado */
    if (!dividir_por_cero()) /* verificar division por cero */
    {
        renderizar_pantalla_calc(); /* limpiar pantalla */
        setcolor(ERR_MSG_COL); /* color de mensaje de error */
        outtextxy(128, 38, "Syntax Error"); /* mostrar mensaje de error */
        mver(); /* muestra la pantalla */
        return;
    }
    operacion(); /* realiza la operación */
    mostrar_resultado_ops(); /* muestra el resultado de la operación */
    liberar_buffer_pantalla(); /* libera el buffer de la pantalla */
    calc.operacion = 0; /* reinicia la operación */
	mver(); /* muestra la pantalla */
}

/*
    =======================================
    Aplicacion de Calculadora
    =========================================
*/
void app_calculadora(void)
{
    /* 1. Declaracion de componentes y variables */
    /* Declarar componentes principales */
    Component barra_de_ventana, cerrar;

    /* Declarar componentes personalizados */
    Component btn_numero[10], btn_operadores[4], btn_igual, btn_decimal, btn_clear;

    /* Variables Miscelaneas */
	int i;                      /* Variable de Indice */

    /* 2. Limpiar y ajustar las configuraciones por defecto, y rellenar fondo */
    colocar_configuraciones();
    set_bg(WHITE);

    /* 3. Inicializar barra de ventana (sin boton de despliegue ni menu de despliegue) */
    constructor_componente(&barra_de_ventana, 0, 0, WIDTH, 11, BLACK, WHITE, BLACK, HOVER_NO_DISPONIBLE);

    constructor_componente(&cerrar, 0, 0, 11, 11, BLACK, WHITE, 40, HOVER_DISPONIBLE);
    constructor_hover_componente(&cerrar, 40, WHITE, 40);

    /* 4. Inicializar componentes personalizados */
    /* Inicializar componente de boton igual */
	constructor_componente(&btn_igual, 198, 150 + TRAS + TRASR2, 226, 193 + TRAS + SHRB + TRASR2, 42, BLACK, BLACK, HOVER_DISPONIBLE);
    constructor_hover_componente(&btn_igual, 43, BLACK, BLACK);

    /* Inicializar componente de boton de suma */
    constructor_componente(&btn_operadores[0], 166, 75 + TRAS, 193, 95 + TRAS + SHRB, 67, BLACK, BLACK, HOVER_DISPONIBLE);
    constructor_hover_componente(&btn_operadores[0], 92, BLACK, BLACK);

    /* Inicializar componentes de botones de resta (1), multiplicacion (2) y division (3) */
    for (i = 1; i <= 3; i++)
    {
        /* Se hace un calculo previo dependiendo del boton que se ubicara */
        short pos_y_actual = + ((i > 1) ? columnas_calc[(3 - ((i - 1) % 4))] + TRASR2 : 75) + TRAS;

        /* Una vez hecho eso se construye el componente */
        constructor_componente(
            &btn_operadores[i], 198, 
            pos_y_actual, 225, 
            pos_y_actual + 20 + SHRB, 67, 
            BLACK, BLACK, HOVER_DISPONIBLE
        );
        constructor_hover_componente(&btn_operadores[i], 92, BLACK, BLACK);
    }

    /* Inicializar componente de boton de decimal */
    constructor_componente(&btn_decimal, 166, 174 + TRAS + TRASR2, 193, 193 + TRAS + SHRB + TRASR2, 30, BLACK, BLACK, HOVER_DISPONIBLE);
    constructor_hover_componente(&btn_decimal, 29,  BLACK, BLACK);

    /* Inicializar componente de boton de limpiar */
    constructor_componente(&btn_clear, 98, 75 + TRAS, 162, 95 + TRAS + SHRB, 67, BLACK, BLACK, HOVER_DISPONIBLE);
    constructor_hover_componente(&btn_clear, 92, BLACK, BLACK);

    /* Inicializar botones 0 - 9 */
    constructor_componente(&btn_numero[0], 98, 174 + TRAS + TRASR2, 162, 193 + TRAS + SHRB + TRASR2, 30, BLACK, BLACK, HOVER_DISPONIBLE);
    constructor_hover_componente(&btn_numero[0], 29, BLACK, BLACK); /* Caso: Boton 0 */

    for (i = 1; i <= 9; i++) /* Caso: Botones del 1 - 9 */
    {
        /* Constructor de componente */
        constructor_componente(
            &btn_numero[i],
            filas_calc[(i - 1) % 3],
            columnas_calc[(i - 1) / 3] + TRAS + TRASR2,
            filas_calc[(i - 1) % 3] + 28,
            columnas_calc[(i - 1) / 3] + 20 + TRAS + TRASR2 + SHRB,
            30, BLACK, BLACK, HOVER_DISPONIBLE
        );

        /* Constructor de hover */
        constructor_hover_componente(&btn_numero[i], 29, BLACK, BLACK);
    }

    /* 5. Renderizar componentes y assets */
    /* Renderizar componentes de barra de ventana (con encabezado de "Calculadora") */
    renderizar_borde(&barra_de_ventana);
    renderizar_componente_texto(&barra_de_ventana, "Calculadora");
    renderizar_boton_cerrar(&cerrar);

    /* Dibujar fondo y cuerpo de calculadora */
    dibujar_fondo();
    dibujar_cuerpo_calc();

    /* Renderizar boton de igual */
    renderizar_componente_texto(&btn_igual, "=");

    /* Renderizar botones de operadores */
    for (i = 0; i < 4; i++)
        renderizar_componente_texto(&btn_operadores[i], operaciones_calc[i]);

    /* Renderizar boton de limpieza y decimal */
    renderizar_componente_texto(&btn_clear, "C");
    renderizar_componente_texto(&btn_decimal, ".");

    /* Renderizar botones de numeros */
    for (i = 0; i <= 9; i++)
        renderizar_componente_texto(&btn_numero[i], numeros_calc[i]);
    
    /* 6. Activar raton */
    mver();

	do {
        /* 7. Capturar la posicion actual del mouse */
        short mouse_x = mxpos(), mouse_y = mypos();
        
        /* 8. === HOVER BOTON DE CERRRAR === */
        manejar_hover_componente(&cerrar, mouse_x, mouse_y, renderizar_boton_cerrar);     

        /* 9. Hover de botones de calculadora */
        manejar_hover_componente(&btn_igual, mouse_x, mouse_y, renderizar_btn_igual);
        manejar_hover_componente(&btn_operadores[0], mouse_x, mouse_y, renderizar_btn_suma);
        manejar_hover_componente(&btn_operadores[1], mouse_x, mouse_y, renderizar_btn_resta);
        manejar_hover_componente(&btn_operadores[2], mouse_x, mouse_y, renderizar_btn_multi);
        manejar_hover_componente(&btn_operadores[3], mouse_x, mouse_y, renderizar_btn_divi);
        manejar_hover_componente(&btn_clear, mouse_x, mouse_y, renderizar_btn_clear);
        manejar_hover_componente(&btn_decimal, mouse_x, mouse_y, renderizar_btn_decimal);
        manejar_hover_componente(&btn_numero[0], mouse_x, mouse_y, renderizar_btn_0);
        manejar_hover_componente(&btn_numero[1], mouse_x, mouse_y, renderizar_btn_1);
        manejar_hover_componente(&btn_numero[2], mouse_x, mouse_y, renderizar_btn_2);
        manejar_hover_componente(&btn_numero[3], mouse_x, mouse_y, renderizar_btn_3);
        manejar_hover_componente(&btn_numero[4], mouse_x, mouse_y, renderizar_btn_4);
        manejar_hover_componente(&btn_numero[5], mouse_x, mouse_y, renderizar_btn_5);
        manejar_hover_componente(&btn_numero[6], mouse_x, mouse_y, renderizar_btn_6);
        manejar_hover_componente(&btn_numero[7], mouse_x, mouse_y, renderizar_btn_7);
        manejar_hover_componente(&btn_numero[8], mouse_x, mouse_y, renderizar_btn_8);
        manejar_hover_componente(&btn_numero[9], mouse_x, mouse_y, renderizar_btn_9);

        /* 10. === MANEJO DE CALLBACKS === */
        if (mclick() == 1) /* Si se da click se evalua en donde se dio para realizar las siguientes acciones */
        {
            /* Caso: Boton de cerrar (detiene el programa) */
            if (mouse_sobre_componente(&cerrar, mouse_x, mouse_y))
                break;

            /* Caso: Botones de Operadores +, -, *, y / */
            for (i = 0; i < 4; i++)
            {
                if (mouse_sobre_componente(&btn_operadores[i], mouse_x, mouse_y)) 
                {
                    switch (i)
                    {
                        case 0: callback_operador(SUM_CHAR); break;
                        case 1: callback_operador(RES_CHAR); break;
                        case 2: callback_operador(MULTI_CHAR); break;
                        case 3: callback_operador(DIVI_CHAR); break;
                    }
                }
            }

            /* Caso: Boton de Igual */
            if (mouse_sobre_componente(&btn_igual, mouse_x, mouse_y))
                callback_btn_igual();

            /* Caso: Boton de Limpieza */
            else if (mouse_sobre_componente(&btn_clear, mouse_x, mouse_y))
                callback_btn_clear();

            /* Caso: Boton de decimal */
            else if (mouse_sobre_componente(&btn_decimal, mouse_x, mouse_y))
                callback_btn_decimal();

            /* Caso: Botones de numeros del 0 al 9 */
            for (i = 0; i < 10; i++)
            {
                if (mouse_sobre_componente(&btn_numero[i], mouse_x, mouse_y)) 
                    callback_btn_numero(numeros_calc[i][0]);
            }
        }
        delay(100); /* Retraso para evitar saturacion */
    } while (1);
}
#endif
