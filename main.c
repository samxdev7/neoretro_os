/* 
    ======================= NEORETRO OS ============================

    Descripcion breve sobre sistema operativo.
    Agradecimientos.
    Desarrolladores.
    Stack de trabajo utilizado.
    Datos adicionales.
*/

/*
    =======================================
    Importacion de Librerias Estandar
    =========================================
*/
#include <conio.h>
#include <dos.h>
#include <graphics.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
    =======================================
    Importacion de Librerias Personalizadas
    =========================================
*/
#include "gphadmin.h"   /* Libreria que administra el modo grafico y el sistema operativo */
#include "bootanim.h"   /* Libreria que contiene animaciones del arranque y apagado del SO */
#include "block.h"      /* Libreria que administra la pantalla de bloqueo */
#include "desktop.h"    /* Libreria que administra el escritorio del sistema operativo */

/*
    =======================================
    Sistema Operativo
    =========================================
*/
int main()
{
    /* 1. Inicializar variable de estado del sistema operativo */
    /* Inicia con la accion de reiniciar ya que esta programada para
    ejecutar la simulacion del arranque */
    BootManageOS estado_os = ACCION_REINICIAR;

    /* 2. Iniciar modo grafico SVGA256 - 320x200x256 */
	if (iniciar_modo_svga_256("C:\\TC20\\BIN")) return EXIT_FAILURE;

    /* === BUCLE PRINICIPAL DEL SISTEMA OPERATIVO === */
	do {
        /* Animacion de arranque + Pantalla de bienvenida */
        if (estado_os == ACCION_REINICIAR)
        { animacion_arranque(); pantalla_bienvenida(); }

        /* Pantalla de bloqueo inicial */
        pantalla_bloqueo_inicial();

        /* Pantalla de bloqueo de inicio de sesion */
        estado_os = pantalla_bloqueo_sesion();

        /* Si se dio a iniciar en el inicio de sesion se 
        ejecuta el escritorio del sistema operativo */
        if (estado_os == INICIAR)
            estado_os = escritorio_so();
        
        /* Si se decide apagar desde la pantalla de bloqueo o el escritorio 
        se ejecuta la simulacion del apagado */
        if (estado_os == ACCION_REINICIAR || estado_os == ACCION_APAGAR)
            apagar_sistema_operativo();
 
	} while(estado_os != ACCION_APAGAR); /* Condicion de salida clave del sistema operativo */

    /* Se cierra el modo grafico */
    closegraph();

    /* Salida con exito!!! */
    return EXIT_SUCCESS;
}