/*
    =======================================
    PROGRAMA DE MUESTRA
    Llama a los 3 Protectores de Pantalla
    =======================================
*/

#include <stdio.h>
#include <conio.h>
#include <dos.h>

#include "protect.h"

int protector1(void);
int protector2(void);
int protector3(void);
void pausar(void);

void pausar() {
    printf("\nPresione una tecla para continuar...");
    getch();
}

int main() {
    int opcion;

    do {
        clrscr();
        printf("=======================================\n");
        printf("         MENU DE PROTECTORES\n");
        printf("=======================================\n");
        printf("1. Protector DVD\n");
        printf("2. Carrusel de imagenes\n");
        printf("3. Figuras geometricas\n");
        printf("4. Salir\n");
        printf("=======================================\n");
        printf("Seleccione una opcion: ");

        opcion = getch();

        switch (opcion) {
            case '1':
                clrscr();
                printf("Ejecutando Protector 1...\n");
                delay(1000);
                protector1();
		closegraph();
                pausar();
                break;

            case '2':
                clrscr();
                printf("Ejecutando Protector 2...\n");
                delay(1000);
                protector2();
		closegraph();
                pausar();
                break;

            case '3':
                clrscr();
                printf("Ejecutando Protector 3...\n");
                delay(1000);
                protector3();
		closegraph();
                pausar();
                break;

            case '4':
                printf("\nSaliendo del programa...\n");
                delay(700);
                break;

            default:
                printf("\nOpcion invalida.\n");
                delay(800);
                break;
        }

    } while (opcion != '4');

    return 0;
}
