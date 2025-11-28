/* 
    ======================= NEORETRO OS ============================

    NeoRetro OS es un sistema operativo que ofrece aplicaciones
    basicas para satisfacer las necesidades basicas cotidianas,
    haciendo uso eficiente del hardware a traves de su desarrollo
    en C. NeoRetro se caracteriza por combinar un estilo moderno
    junto al ambiente retro (por utilizar pocos recursos y
    tambien por ser desarrollado para DOSBOX/TC20), dando
    una frescura al utilizar este sistema operativo.

    Agradecimientos.
    Agradecemos primeramente a Dios por guiarnos, 
    fortalecernos y permitirnos llegar hasta este 
    punto del proyecto. Extensivo tambien nuestro 
    agradecimiento a nuestras familias, cuyo apoyo 
    constante y motivacion fueron fundamentales 
    durante todo el proceso. Asimismo, expresamos 
    nuestro profundo agradecimiento a la 
    MSc. Gloria Quintana y al MSc. Grevin Silva, 
    quienes con su orientacion, dedicacion y 
    confianza nos inspiraron a crecer como 
    estudiantes, futuros profesionales y 
    como personas.

    Desarrolladores.
    - Carlos Bayardo Valdivia Alaniz
    - Carlos Humberto Torres M.
    - Gabriela Abigail Ruiz Rodriguez
    - Johanna Patricia Castellon Duarte
    - Moises Eliseo Aleman Garcia
    - Said Abel Garay B.
    - Samuel Enrique Rueda Ruiz

    Stack de trabajo utilizado.
        Desarrollo y compilacion
        - DOSBox+TC20
        - Visual Studio Code
        Utilidades y soporte
        - Python
        - ChatGPT + Deepseek
        Colaboracion y control de versiones
        - Git + GitHub
        - Discord + Notion

    Datos adicionales.
    -------------Universidad------------
    Universidad Nacional de Ingenieria
    ---------------Carrera---------------
    Ingenieria en Computacion
    ---------------Docente---------------
    MSc. Gloria Talia Quintana Flores
    ---------Periodo de desarrollo---------
    Inicio: Viernes, 17 de octubre de 2025
    Finalizacion: Martes, 25 de noviembre de 2025

    Pasen buen dia y disfruten del sistema operativo!!!
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
#include "protect.h"    /* Libreria que administra los protectores de pantalla */
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

        /* Antes de verificar si se inicia, si se apaga o se reinicia 
        se verifica si se decide suspender para activar el protector
        de pantalla (metodo de suspension) */
        if (estado_os == ACCION_SUSPENDER)
        {
            mocultar();                     /* Se oculta el raton */
            colocar_protector_pantalla();   /* Se pone el protector de pantalla actual */
            mver();                         /* Una vez se toca una tecla, se restaura el raton */
        }

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