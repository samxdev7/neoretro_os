#ifndef DYNAMIC_H
#define DYNAMIC_H

/*
	DYNAMIC_H
	Libreria que administra la memoria dinamica de los programas. 
	
    OJO, ES MUY IMPORTANTE TRABAJAR SI O SI CON ESTA LIBRERIA SI QUIERES
    USAR MEMORIA DINAMICA, DE LO CONTRARIO TE VERAS PERJUDICADO CON LA MEMORIA

	- Samuel Rueda
*/

/*
    =======================================
    Importacion de Librerias Estandar
    =========================================
*/
#include <conio.h>
#include <graphics.h>
#include <stdlib.h>

/*
    =======================================
    Declaracion de Prototipos
    =========================================
*/
void liberar_elemento(void *ptr);
void liberar_arreglo(void **ptr, short n_elementos);
void reservar_opciones_desplegables(char ***opciones_desplegables, short n_opciones);

/*
    =======================================
    Inicializacion de Funciones
    =========================================
*/
/* 
	liberar_elemento()
	- Libera la memoria de un espacio de memoria.

    Parametros:
    void *ptr: Puntero a espacio de memoria a liberar.
*/
void liberar_elemento(void *ptr)
{
    /* Validar si no es puntero nulo, luego librerar si se cumplo el caso */
    if (ptr != NULL) free(ptr);
}

/* 
	liberar_arreglo()
	- Libera la memoria de varios espacios de memoria.

    Parametros:
    void **ptr: Puntero a punteros del arreglo.
    short n_elementos: Cantidad de elementos del arreglo.
*/
void liberar_arreglo(void **ptr, short n_elementos)
{
    /* 1. Definir ariable de indice */
    unsigned short i; 

    /* 2. Validar si el puntero del arreglo no es nulo */
    if (ptr == NULL) return;

    /* 3. Liberar elemento por elemento y asignar a null */
    for (i = 0; i < n_elementos; i++) {    
        if (ptr[i] == NULL) return; /* Validar si el puntero del elemento no es nulo */

        free(ptr[i]);
        ptr[i] = NULL;
    }

    /* 4. Liberar arreglo y asignar a null */
    free(ptr);
    ptr = NULL;
}

/* 
	reservar_opciones_desplegables()
	- Reserva cadenas utilizadas para appadmin.h y las aplicaciones del so (principalmente el 
    menu de despliegue).

    Parametros:
    char ***opciones_desplegables: Puntero a matriz de cadenas.
    short n_opciones: Cantidad de opciones (elementos) del menu desplegable.
*/
void reservar_opciones_desplegables(char ***opciones_desplegables, short n_opciones)
{
    /* 1. Declarar variable de indice */
    unsigned short i;

    /* 2. Reservar a puntero char (**) (matriz de cadenas) */
    *opciones_desplegables = (char **) malloc(n_opciones * sizeof(char *));

    /* 3. Reservar cada cadena de la matriz char (*)*/
    for (i = 0; i < n_opciones; i++) {
        /* Se reservan 20 caracteres para cada cadena */
        (*opciones_desplegables)[i] = (char *) malloc(20 * sizeof(char));
    }
}
#endif