#ifndef VALID_H
#define VALID_H
/********************************************************************
        LIBRERIA DE VALIDACIONES

Creada por Samuel Rueda

aportaciones por:
- Moises Aleman

Versión: 1.2
Fecha: 02/11/2025
Descripción:
    Esta librería contiene funciones para la validación de entradas
    de datos en C, incluyendo validaciones para enteros, flotantes,
    y buffers vacíos, así como funciones auxiliares para la
     limpieza del buffer y el manejo de errores.
*******************************************************************/

#include <stdio.h>
#include <ctype.h>

/*******************************
    Estructuras y Definiciones
*******************************/
typedef enum {
    VALID_NUMBER_OK = 0,
    VALID_NUMBER_INVALID_TYPE = 1,
    VALID_NUMBER_BELOW_MIN = 2,
    VALID_NUMBER_ABOVE_MAX = 3
} number_isvalid_t;

typedef enum {
    BUF_ERR_NULL = -1,
    BUF_EMPTY = 1,
    BUF_NOT_EMPTY = 0
}buff_err_t;

/*******************************
    PROTOTIPADO DE FUNCIONES
*******************************/
void limpiar_buffer(void); /* Limpieza del buffer */
void funcion_error(void);   /* Control de errores */
number_isvalid_t validar_entero (int*, int, int); /* validar una entrada de un int */
number_isvalid_t validar_float (float*, float, float); /* validar una entrada de un float */
buff_err_t buffer_isempty(const char*); /* Verifica si una cadena/string esta vacia o solo tiene espacios */

/*********************************
    INICIALIZACION DE FUNCIONES
**********************************/

void limpiar_buffer(void) /* Limpieza del buffer */
{
    /* Esta se encarga de limpiar el buffer (que es una memoria con el que trabaja con las entradas del teclado),
    se toma cada caracter del buffer con un bucle while hasta que se encuentra con el caracter de nueva linea.
    Al tomarlo, deja el buffer limpio para que se puedan leer otras entradas
    con scanf o getchar. */
    int buffer_c;
    while ((buffer_c = getchar()) != EOF && buffer_c != '\n');
}

void funcion_error(void) /* Control de errores */
{
    /* Se implementa en este programa para las siguientes funciones en caso de que no se ingrese el valor
    correcto: se hace limpieza del buffer con una función previamente declarada, se limpia la pantalla y
    se imprime un mensaje de error. */
    limpiar_buffer(); /* Limpiar el buffer */
}

/* Validacion de entradas enteras
    Devuelve:
    VALID_NUMBER_OK = 0             -> entrada correcta
    VALID_NUMBER_INVALID_TYPE = 1   -> tipo de dato incorrecto
    VALID_NUMBER_BELOW_MIN = 2      -> menor que min
    VALID_NUMBER_ABOVE_MAX = 3      -> mayor que max
 */
number_isvalid_t validar_entero (int *variable_entrada, int min, int max)
{
    int entrada_valida; /* Señal de tipo de dato correcto */
    char extra_buffer;  /* Señal de restos en el buffer */

    /* Captura el buffer una vez que se ingresen los datos
     *  Busca la concordancia entre un entero y un caracter
     *  Si se ingreso correctamente los datos el buffer queda limpio
     *  Si se ingresa un flotante el caracter captura el punto decimal
     *  Si se ingresa una cadena o caracter habra problemas en la compilacion y ejecucion
     *  NOTA: La funcion no admite ningun espacio en blanco despues del numero
     */
    entrada_valida = scanf("%d%c", variable_entrada, &extra_buffer);

    /* Si el valor es incorrecto devuelve 1 */
    if (entrada_valida != 2 || extra_buffer != '\n')
    {
        limpiar_buffer();
        return VALID_NUMBER_INVALID_TYPE;
    } 

    /* Si el valor se sale del rango establecido devuelve 2 o 3 */
    if (*variable_entrada < min) return VALID_NUMBER_BELOW_MIN;
    if (*variable_entrada > max) return VALID_NUMBER_ABOVE_MAX;

    /* Entrada permitida */
    return VALID_NUMBER_OK;
}

/* Validacion de entradas flotantes
    Devuelve:
    VALID_NUMBER_OK = 0             -> entrada correcta
    VALID_NUMBER_INVALID_TYPE = 1   -> tipo de dato incorrecto
    VALID_NUMBER_BELOW_MIN = 2      -> menor que min
    VALID_NUMBER_ABOVE_MAX = 3      -> mayor que max
 */
number_isvalid_t validar_float (float *variable_entrada, float min, float max)
{
    int entrada_valida; /* Señal de tipo de dato correcto */
    char extra_buffer;  /* Señal de restos en el buffer */

    /* Captura el buffer una vez que se ingresen los datos
     *  captura el numero flotante incluido el punto decimal
     *  verifica si hay algun caracter extra en el buffer
     *  Si se ingreso correctamente los datos el buffer queda limpio
     *  Si se ingresa una cadena o caracter despues del numero habra problemas en la compilacion y ejecucion
     *  NOTA: La funcion no admite ningun espacio en blanco despues del numero
     */
    entrada_valida = scanf("%f%c", variable_entrada, &extra_buffer);

    /* Si el valor es incorrecto devuelve 1 */
    if (entrada_valida != 2 || extra_buffer != '\n'){
        limpiar_buffer();
        return VALID_NUMBER_INVALID_TYPE;
    } 

    /* Si el valor se sale del rango establecido devuelve 2 o 3 */
    if (*variable_entrada < min) return VALID_NUMBER_BELOW_MIN;
    if (*variable_entrada > max) return VALID_NUMBER_ABOVE_MAX;

    /* Entrada permitida */
    return VALID_NUMBER_OK;
}

/* devuelve 1 si s está vacío o solo contiene espacios/tabuladores */
/*
BUF_ERR_NULL = -1 // s es NULL
BUF_EMPTY = 1   // s está vacío o solo contiene espacios/tabuladores
BUF_NOT_EMPTY = 0 // s contiene al menos un caracter no espacio
*/
buff_err_t buffer_isempty(const char *s) {

    /* primero verificamos que es puntero no sea NULL para evitar comportamiento indefinido */
    /* devuelve -1 si el valor es NULL */
    if (s == NULL) return BUF_ERR_NULL;

    /* recorre el array de caracteres hasta '\0' */
    while (*s) {
        if (!isspace((unsigned char)*s)) return BUF_NOT_EMPTY; /* se encontró un caracter devuelve 0*/
        s++; /* avanza al siguiente espacio en memoria (aritmetica de punteros) */
    }

    /* solo espacios encontrados devuelve 1 */
    return BUF_EMPTY;
}
#endif /* VALID_H */
