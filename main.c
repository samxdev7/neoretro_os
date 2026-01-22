/*
                           Welcome to...
    ======================= NEORETRO OS ============================
*/

#include <conio.h>
#include <dos.h>
#include <graphics.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "gphadmin.h"   
#include "bootanim.h"   
#include "block.h"      
#include "protect.h"

int main()
{
    BootManageOS estado_os = ACCION_REINICIAR; /* SO Status */
	if (iniciar_modo_svga_256("C:\\TC20\\BIN")) 
        return EXIT_FAILURE;

	do {
        if (estado_os == ACCION_REINICIAR)
        { animacion_arranque(); pantalla_bienvenida(); }

        pantalla_bloqueo_inicial();
        estado_os = pantalla_bloqueo_sesion();

        if (estado_os == ACCION_SUSPENDER)
        { mocultar(); colocar_protector_pantalla(); mver(); }

        if (estado_os == INICIAR)
            estado_os = escritorio_so();
        
        if (estado_os == ACCION_REINICIAR || estado_os == ACCION_APAGAR)
            apagar_sistema_operativo();
	} while (estado_os != ACCION_APAGAR);

    closegraph();
    return EXIT_SUCCESS;
}