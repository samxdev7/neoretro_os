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
char numeros_calc[] = {
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'
};

char operaciones_calc[] = {
	'+', '-', '*', '/'
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
void dibujar_fondo(void);
void dibujar_cuerpo_calc(void);
void renderizar_pantalla_calc(void);
void renderizar_numeros_pantalla(void);
void liberar_buffer_resultado(void);
void liberar_buffer_pantalla(void);
void verficar_si_0(void);
void estado_inicial_pantalla(void);
void nuevo_input_pantalla(void);
bool overflow_pantalla(void);
void mostrar_resultado_ops(void);
void append_numero_pantalla(char);
bool check_inicial(void);
bool solo_signo_presionado(void);
bool mas_de_una_operacion(void);
bool input_antes_de_signo(void);
bool dividir_por_cero(void);
void operacion(void);
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
void callback_operador(operaciones_t);
void callback_btn_igual(void);
void app_calculadora(); /* contiene la logica principal de la calc */

/* Inicio funciones */
void dibujar_fondo(void)
{
    FILE *p;
	p = fopen("back.bin", "rb");
    if (validar_archivo(p)) return;

    dibujar_con_rasterizado_coords(p, 2, 12, 316, 186); /* Dibuja el fondo de la calculadora */

    fclose(p);
}

void dibujar_cuerpo_calc(void)
{
    FILE *p;
	p = fopen("CLCBDY.bin", "rb");
    if (validar_archivo(p)) return;

    dibujar_raster_png_coords(p, 2, 12, 316, 186); /* Dibuja el cuerpo de la calculadora */

    fclose(p);
}

void renderizar_pantalla_calc(void) {
    setcolor(BLACK);
    setfillstyle(SOLID_FILL, BLACK);
    bar(102, 34, 221, 59); /* Limpia la pantalla */
}

void renderizar_numeros_pantalla(void)
{
    renderizar_pantalla_calc();
    setcolor(SCR_NUM_COL);
    outtextxy(215 - ((NUM_PX_SZ  * calc.len_pantalla) - (NUM_PX_SZ + calc.len_pantalla)), 43, calc.pantalla);
}

void liberar_buffer_resultado(void) {
    int i;
    for (i = 0; i < MAX_RESULTADO_STR; i++) {
        calc.resultado_str[i] = '\0';
    }
}

void liberar_buffer_pantalla(void) {
    int i;
    for (i = 0; i < MAX_PANTALLA; i++) {
        calc.pantalla[i] = '\0';
    }
    calc.len_pantalla = 0;
}

void verficar_si_0(void) {
    if (strcmp(calc.pantalla, "0") == 0)
        liberar_buffer_pantalla();
}

void estado_inicial_pantalla(void) {
    liberar_buffer_pantalla();
    liberar_buffer_resultado();
    renderizar_pantalla_calc();
    setcolor(SCR_NUM_COL);
    outtextxy(215, 43, "0");
}

void nuevo_input_pantalla(void) {
    decimal_usado = false;
    estado_inicial_pantalla();
}

bool overflow_pantalla(void) {
		if (calc.len_pantalla >= MAX_PANTALLA - 1) {
        calc.len_pantalla = MAX_PANTALLA - 1;
        calc.pantalla[calc.len_pantalla] = '\0';
        return true;
    }
    return false;
}

void mostrar_resultado_ops(void) {
    liberar_buffer_resultado();
    renderizar_pantalla_calc();
    setcolor(SCR_NUM_COL);

    if (calc.resultado > ((int)calc.resultado))
        sprintf(calc.resultado_str, "%.2f", calc.resultado);
    else
        sprintf(calc.resultado_str, "%.0f", calc.resultado);
    
    outtextxy(215 - ((NUM_PX_SZ  * strlen(calc.resultado_str)) - (NUM_PX_SZ + strlen(calc.resultado_str))), 43, calc.resultado_str);
    liberar_buffer_pantalla();
}

void append_numero_pantalla(char numero) {
    /*Caso del primer '.' -> "0." */
    if (numero == '.' && calc.len_pantalla == 0) {
        if (MAX_PANTALLA >= 2) {
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

bool solo_signo_presionado(void){
    if ((calc.len_pantalla == 0 && calc.resultado == 0))
        return true;
    return false;
}

bool mas_de_una_operacion(void) {
    if (calc.len_pantalla > 0 && calc.operacion != 0 && calc.resultado != 0)
        return true;
    return false;
}

bool input_antes_de_signo(void) {
    if ((calc.resultado != 0 && calc.len_pantalla == 0 && calc.operacion == 0))
        return true;
    return false;
}

bool dividir_por_cero(void) {
    if (calc.operacion == DIVI_CHAR && strcmp(calc.pantalla, "0") == 0)
        return false;
    return true;
}

void operacion(void) {
    switch (calc.operacion)
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

void renderizar_btn(Component *componente, char btn)
{
    renderizar_componente_texto(componente, btn);
}

void callback_operador(operaciones_t operador)
{
    if (solo_signo_presionado())
        return;

    if (check_inicial())
    {
        calc.resultado = atof(calc.pantalla);
        calc.operacion = operador;
        nuevo_input_pantalla();
        return;
    }
    else
    {
        calc.operacion = operador;
        nuevo_input_pantalla();
        return;
    }
}

void callback_btn_clear(void) {
    calc.resultado = 0;
    calc.operacion = 0;
    decimal_usado = false;
    liberar_buffer_resultado();
    estado_inicial_pantalla();
}

void callback_btn_decimal(void) {
    if (decimal_usado)
        return;
    if (input_antes_de_signo())
        callback_btn_clear();
        
    append_numero_pantalla('.');
    renderizar_numeros_pantalla();
}

void callback_btn_numero(char numero)
{
    if (input_antes_de_signo())
        callback_btn_clear();

    append_numero_pantalla(numero);
    renderizar_numeros_pantalla();
}

void callback_btn_igual(void) {
    mocultar();
    if (!dividir_por_cero())
    {
        renderizar_pantalla_calc();
        setcolor(ERR_MSG_COL);
        outtextxy(128, 38, "Syntax Error");
        mver();
        return;
    }
    operacion();
    mostrar_resultado_ops();
    liberar_buffer_pantalla();
    calc.operacion = 0;
	mver();
}

void manejar_opcion_menu_calc(int opcion_seleccionada, short *bandera_salir)
{
    switch (opcion_seleccionada) {
        case 0:  /* Primera opción del menú */
            *bandera_salir = 1;
            break;
    }
}

/*
    =======================================
    Programa Principal
    =========================================
*/
void app_calculadora()
{
    /* 1. Declaracion de componentes y variables */
    /* Declarar componentes principales */
    Component barra_de_ventana, cerrar, despliegue;

    /* Declarar componentes personalizados */
    /* ==================================== */
    Component btn_numero[10], btn_operadores[4], btn_igual, btn_decimal, btn_clear;

    /* ==================================== */

    /* Declarar la cantidad de opciones del menu desplegable a utilizar */
    char **opciones_desplegables; /* NO TOCAR */
    unsigned char n_opciones = 0; /* Asignar cuantas opciones habran del menu despegable */

    /* Declarar variables del funcionamiento del menu de despliegue (NO TOCAR) */
	int *desplegable_buffer = NULL;
	int i;
    unsigned char despliegue_activo = 0;
    unsigned char salir = 0;

    /* 2. Limpiar y ajustar las configuraciones por defecto, y rellenar fondo */
    colocar_configuraciones();
    set_bg(WHITE);

    /* 3. Reservar memoria para las opciones a utilizar (NO TOCAR) */
    reservar_opciones_desplegables(&opciones_desplegables, n_opciones);

    /* 4. Definir nombres de las opciones del menu de despliegue */
    strcpy(opciones_desplegables[0], "Salir");

    /* 5. Inicializar barra de ventana */
    inicializar_barra_ventana(&barra_de_ventana, &cerrar, &despliegue, "Calculadora");

    /* 6. Inicializar componentes personalizados */
    /* ==================================== */
    /* Inicializar componente y hover mensaje */
	constructor_componente(&btn_igual, 198, 150 + TRAS + TRASR2, 226, 193 + TRAS + SHRB + TRASR2, 42, BLACK, BLACK, HOVER_DISPONIBLE);
    constructor_hover_componente(&btn_igual, 43, BLACK, BLACK);

    /* Inicializar componente suma */
    constructor_componente(&btn_operadores[0], 166, 75 + TRAS, 193, 95 + TRAS + SHRB, 67, BLACK, BLACK, HOVER_DISPONIBLE);
    constructor_hover_componente(&btn_operadores[0], 92, BLACK, BLACK);

    for (i = 1; i <= 3; i++)
    {
        constructor_componente(
            &btn_operadores[i], 198, 
            (4 - (i % 3)) + TRAS, 225, 
            (4 - (i % 3)) + 20 + TRAS + SHRB, 67, 
            BLACK, BLACK, HOVER_DISPONIBLE
        );
        constructor_hover_componente(&btn_operadores[i], 92, BLACK, BLACK);
    }

    constructor_componente(&btn_decimal, 166, 174 + TRAS + TRASR2, 193, 193 + TRAS + SHRB + TRASR2, 30, BLACK, BLACK, HOVER_DISPONIBLE);
    constructor_hover_componente(&btn_decimal, 29,  BLACK, BLACK);

    /* Constructor de Botones 1 - 9 */
    for (i = 1; i <= 9; i++)
    {
        /* Constructor de componente */
        constructor_componente(
            &btn_numero[i],
            filas_calc[i % 3],
            columnas_calc[i % 3] + TRAS + TRASR2,
            filas_calc[i % 3] + 28,
            columnas_calc[i % 3] + TRAS + TRASR2 + SHRB,
            30, BLACK, BLACK, HOVER_DISPONIBLE
        );

        /* Constructor de hover */
        constructor_hover_componente(&btn_numero[i], 29, BLACK, BLACK);
    }

    /* ==================================== */

    /* 7. Renderizar componentes personalizados */
    /* ==================================== */
    dibujar_fondo();
    dibujar_cuerpo_calc();

    renderizar_componente_texto(&btn_igual, "=");

    for (i = 0; i < 4; i++)
        renderizar_componente_texto(&btn_operadores[0], operaciones_calc[i]);

    renderizar_componente_texto(&btn_clear, "C");
    renderizar_componente_texto(&btn_decimal, ".");

    for (i = 0; i <= 9; i++)
        renderizar_componente_texto(&btn_numero[i], numeros_calc[i]);

    /* ==================================== */
    
    delay(100);
    /* 8. Activar raton */
    mver();

	do {
        /* 9. Capturar la posicion actual del mouse */
        short mouse_x = mxpos(), mouse_y = mypos();
        unsigned char resultado;
        
        /* ===================================
         HOVERS Y CALLBACKS DE BARRA DE VENTANA 
        ====================================== */

        /* ===== HOVERS ===== */
        manejar_hover_componente(&cerrar, mouse_x, mouse_y, renderizar_boton_cerrar);
        manejar_hover_componente(&despliegue, mouse_x, mouse_y, renderizar_boton_despliegue);


        /* ===== Dibujar pantalla una vez ===== */
        if (estado == false)
        {
            renderizar_pantalla_calc();
            callback_btn_clear();
            estado = true;
        }

        /* ===== CALLBACKS ===== */
        /* Manejo completo de la barra de ventana */        
        resultado = manejar_barra_ventana(
            &cerrar, &despliegue, mouse_x, mouse_y,
            &despliegue_activo, n_opciones, 
            opciones_desplegables, &desplegable_buffer
        );

        /* Procesar resultados */
        if (resultado == n_opciones) salir = 1;
        else if (resultado >= 0) manejar_opcion_menu_calc(resultado, &salir);

        /* ===================================
           HOVERS Y CALLBACKS PERSONALIZADOS 
        ====================================== */
        if (!despliegue_activo) {
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
        }

        if (mclick() == 1)
        {
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

            if (mouse_sobre_componente(&btn_igual, mouse_x, mouse_y))
                callback_btn_igual();

            else if (mouse_sobre_componente(&btn_clear, mouse_x, mouse_y))
                callback_btn_clear();

            else if (mouse_sobre_componente(&btn_decimal, mouse_x, mouse_y))
                callback_btn_decimal();

            for (i = 0; i < 10; i++)
            {
                if (mouse_sobre_componente(&btn_numero[i], mouse_x, mouse_y)) 
                    callback_btn_numero(i);
            }
        }

        delay(100);
    } while (!salir);

    /* 10. Liberar memoria */
    liberar_buffer_desplegable(&desplegable_buffer);
	liberar_arreglo(opciones_desplegables, n_opciones);
}
#endif
