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

int main() {
    BootManageOS status_os = RESTART_ACTION; /* SO Status */
	if (iniciar_modo_svga_256("C:\\TC20\\BIN")) 
        return EXIT_FAILURE;

	do {
        if (status_os == RESTART_ACTION)
        { animacion_arranque(); pantalla_bienvenida(); }

        pantalla_bloqueo_inicial();
        status_os = pantalla_bloqueo_sesion();

        if (status_os == SUSPEND_ACTION)
        { mocultar(); colocar_protector_pantalla(); mver(); }

        if (status_os == START_ACTION)
            status_os = escritorio_so();
        
        if (status_os == RESTART_ACTION || status_os == SHUT_DOWN_ACTION)
            apagar_sistema_operativo();
	} while (status_os != SHUT_DOWN_ACTION);

    closegraph();
    return EXIT_SUCCESS;
}