#include <stdio.h>
#include <conio.h>

#include "VALID.H"

int main(void) {

    int numero, result;
    float numero_f;

    do
    {
        getch();
        printf("Ingrese un numero entero: ");    
        switch (result = validar_entero(&numero, 1, 100)) {
            case VALID_NUMBER_OK:
                printf("Numero entero valido: %d\n", numero);
                break;
            case VALID_NUMBER_INVALID_TYPE:
                printf("Error: Tipo de dato invalido para entero.\n");
                break;
            case VALID_NUMBER_BELOW_MIN:
                printf("Error: Numero entero por debajo del minimo.\n");
                break;
            case VALID_NUMBER_ABOVE_MAX:
                printf("Error: Numero entero por encima del maximo.\n");
                break;
    }
    } while (result != VALID_NUMBER_OK);
    
    do
    {
        getch();
        clrscr();
        printf("Ingrese un numero flotante: ");
        switch (result = validar_float(&numero_f, 0.0f, 100.0f)) {
            case VALID_NUMBER_OK:
                printf("Numero flotante valido: %.2f\n", numero_f);
                break;
            case VALID_NUMBER_INVALID_TYPE:
                printf("Error: Tipo de dato invalido para flotante.\n");
                break;
            case VALID_NUMBER_BELOW_MIN:
                printf("Error: Numero flotante por debajo del minimo.\n");
                break;
            case VALID_NUMBER_ABOVE_MAX:
                printf("Error: Numero flotante por encima del maximo.\n");
                break;
    }
    } while (result != VALID_NUMBER_OK);
    
    
    return 0;
}