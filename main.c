#include <stdio.h>
#include <conio.h>
#include <time.h>

#include "bootanim.h"
#include "block.h"

int main()
{
    short estado_os = REINICIAR;
	if (iniciar_modo_svga_256("C:\\TC20\\BIN")) return EXIT_FAILURE;

	do {
        if (estado_os == REINICIAR)
        {
            animacion_arranque();
            pantalla_bienvenida();
        }

        pantalla_bloqueo_inicial();
        estado_os = pantalla_bloqueo_sesion();

        if (estado_os == REINICIAR)
            apagar_sistema_operativo();

        if (estado_os == INICIAR)
            return 0;
            
    } while(estado_os != APAGAR);

    apagar_sistema_operativo();
    closegraph();

    /* Programa Principal */
    return 0;
}