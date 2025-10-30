#ifndef VALIDGPH_H
#define VALIDGPH_H

/*
	VALIDGPH_H
	Libreria encargada de validar si se inicializo correctamente el
    modo gráfico.

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

/*
    =======================================
    Importacion de Librerias Personalizadas
    =========================================
*/

#include "svga256.h" 	/* Libreria de constantes de resoluciones con 256 colores */

/*
    =======================================
    Declaracion de Prototipos
    =========================================
*/

int huge detectar_svga_256(void);
short int verificar_instalacion_driver(int driver);
short int verificar_inicializacion_driver(void);
short int verificar_soporte_colores(void);

/*
    =======================================
    Inicializacion de Funciones
    =========================================
*/

/*
    detectar_svga_256()
    - Funcion que detecta y devuelve el modo de video SVGA 256 colores
    - Retorna la constante SVGA320x200x256 que define la resolucion 320x200 con 256 colores
*/
int huge detectar_svga_256(void)
{
    return SVGA320x200x256;
}

/*
    verificar_instalacion_driver()
    - Funcion que valida si el driver de video SVGA256 se instaló correctamente
    - Si el driver no se pudo instalar (valor negativo), muestra un mensaje de error
    y retorna EXIT_FAILURE
    - OJO: Esta validacion es crucial antes de intentar inicializar el modo grafico
*/
short int verificar_instalacion_driver(int driver)
{
    if (driver < 0) {
        printf("Failed to install SVGA256 driver\n");
        return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

/*
    verificar_inicializacion_driver()
    - Funcion que valida si la inicializacion del modo grafico fue exitosa
    - Utiliza graphresult() para verificar el estado de la inicializacion grafica
    - Si la inicializacion falla, muestra un mensaje de error y retorna EXIT_FAILURE
    - OJO: Se debe verificar inmediatamente despues de initgraph()
*/
short int verificar_inicializacion_driver()
{
    if (graphresult() != grOk) {
        printf("Graphics initialization failed\n");
        return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

/*
    verificar_soporte_colores()
    - Funcion que valida si los colores son los correspondientes al modo de video seleccionado
    (valida si la cantidad de colores son los 256 colores del svga)
    - Este proceso se realiza a traves de getmaxcolor() que obtiene la cantidad de colores del modo
    de video seleccionado.
    - Si la cantidad de colores no es la correspondiente imprime un mensaje de aviso y devuelve
    EXIT_FAILURE de proceso fallido.
    - OJO: Se debe validar de inmediato para evitar problemas con los colores.
*/
short int verificar_soporte_colores()
{
	/* Validacion Principal */
	int max_colors = getmaxcolor();
	if (max_colors != 255) {
        /* Manejo del error si la cantidad de colores no es la correcta */
        closegraph();
        printf("SVGA 256-color mode not supported\n");
        return EXIT_FAILURE; /* Devuelve 1 */
    }

    /* Se imprime mensaje de exito y devuelve cero */
    printf("SVGA 256-color mode initialized successfully\n");
    return EXIT_SUCCESS;
}
#endif
