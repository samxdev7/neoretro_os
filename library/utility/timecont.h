#ifndef TIMECONT_H
#define TIMECONT_H

/*
    TIMECONT_H
    LIbreria dedicada al manejo facil y rapido del tiempo, util para mostrar la
    fecha y hora en el sistema operativo.

    Para poder usar esta libreria se debe declarar un arreglo de caracteres
    con tamanos definidos (la cual debes usar las constantes de esta libreria).
    Una vez hecho eso ahora tienes el derecho de usar los arreglos a traves
    de sus punteros.

    Ej: Para obtener la hora.
    char hora_actual[FORMATO_HORA]; que actua como buffer
    obtener_hora(hora_actual);
    Y listo!!!

    - Samuel Rueda
*/

/*
    =======================================
    Importacion de Librerias Estandar
    =========================================
*/
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <dos.h>

/*
    =======================================
    Declaracion de Directivas
    =========================================
*/
#define FORMATO_HORA 6    /* Corresponde a la cantidad de caracteres que tendra la hora */
#define FORMATO_FECHA 11  /* Indica la cantidad de caracteres del formato de la fecha */

/*
    =======================================
    Declaracion de Prototipos
    =========================================
*/
void obtener_hora(char *buffer_hora);
void obtener_fecha(char *buffer_fecha);
void obtener_fecha_hora(char *buffer_hora, char *buffer_fecha);
short verificar_cambio(char *hora_actual, char *fecha_actual);

/*
    =======================================
    Inicializacion de Funciones
    =========================================
*/
/*
    obtener_hora();
    Funcion que permite obtener solo la hora actual.

    Parametros:
    char *buffer_hora: Buffer/arreglo de caracteres de la hora.
*/
void obtener_hora(char *buffer_hora)
{
    /* 1. Se declaran las estructuras de tiempo */
    time_t t;
    struct tm *hora_local;
    
    /* 2. Se obtienen los datos de tiempo */
    time(&t);
    hora_local = localtime(&t);

    /* 3. Se inscribe la hora actual al buffer de hora con sprintf */
    sprintf(buffer_hora, "%02d:%02d",
        hora_local->tm_hour, 
        hora_local->tm_min,
        hora_local->tm_sec);
}

/*
    obtener_fecha();
    Funcion que permite obtener solo la fecha actual.

    Parametros:
    char *buffer_fecha: Buffer/arreglo de caracteres de la fecha.
*/
void obtener_fecha(char *buffer_fecha)
{
    /* 1. Se declaran las estructuras de tiempo */
    time_t t;
    struct tm *hora_local;
    
    /* 2. Se obtienen los datos de tiempo */
    time(&t);
    hora_local = localtime(&t);

    /* 3. Se inscriben al buffer de fecha la fecha actual */
    sprintf(buffer_fecha, "%02d/%02d/%d",
       hora_local->tm_mday,
       hora_local->tm_mon + 1,
       hora_local->tm_year + 1900);
}

/*
    obtener_fecha_hora();
    Funcion que permite obtener la fecha y hora actual al mismo tiempo.

    Parametros:
    char *buffer_hora: Buffer/arreglo de caracteres de la hora.
    char *buffer_fecha: Buffer/arreglo de caracteres de la fecha.
*/
void obtener_fecha_hora(char *buffer_hora, char *buffer_fecha)
{
    /* 1. Se declaran las estructuras de tiempo */
    time_t t;
    struct tm *hora_local;
    
    /* 2. Se obtienen los dato de tiempo */
    time(&t);
    hora_local = localtime(&t);

    /* 3. Se inscribe en primer lugar la hora al buffer de hora */
    sprintf(buffer_hora, "%02d:%02d",
        hora_local->tm_hour, 
        hora_local->tm_min,
        hora_local->tm_sec);

    /* 4. Posterior se inscribe la fecha al buffer de fecha */
    sprintf(buffer_fecha, "%02d/%02d/%d",
       hora_local->tm_mday,
       hora_local->tm_mon + 1,
       hora_local->tm_year + 1900);
}

/*
    verificar_cambio();
    Funcion que verifica si los buffers tienen la hora y fecha actual,
    si es el caso se actualizan los buffers, sino, se mantienen.

    Parametros:
    char *buffer_hora: Buffer/arreglo de caracteres de la hora.
    char *buffer_fecha: Buffer/arreglo de caracteres de la fecha.

    Devuelve cero si no se ha actualizado (ya que es la hora/fecha actual), pero
    en caso de que se haya actualizado devuelve 1.
*/
short verificar_cambio(char *hora_actual, char *fecha_actual)
{
    /* 1. Se declaran dos nuevos buffers de fecha y hora + una bandera */
    char nueva_hora[FORMATO_HORA];
    char nueva_fecha[FORMATO_FECHA];
    short cambio = 0; /* Bandera que indica el cambio de fecha/hora */

    /* 2. Se obtiene la fecha y la hora pero con estos dos nuevos buffers */
    obtener_hora(nueva_hora);
    obtener_fecha(nueva_fecha);

    /* 3. Se compara si la nueva hora coincide con la hora del buffer */
    if (strcmp(hora_actual, nueva_hora) != 0)
    {
        /* Se cambia la hora */
        strcpy(hora_actual, nueva_hora);
        cambio = 1; /* Y se indica que cambio el tiempo */
    }

    /* 4. Ahora se compara la fecha del buffer con la nueva fecha */
    if (strcmp(fecha_actual, nueva_fecha) != 0)
    {
        /* Se cambia la fecha */
        strcpy(fecha_actual, nueva_fecha);
        cambio = 1; /* Y se indica que cambio el tiempo */
    }

    /* Dependiendo de la bandera se devuelve si se actualizo o no el tiempo */
    return cambio;
}
#endif