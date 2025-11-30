#ifndef BOOLEAN_SO_H
#define BOOLEAN_SO_H

/*
    BOOLEAN_SO_H
    Esta libreria utiliza un typedef para poder simular al tipo
    booleano que existen en C pero no es represesntado de esa
    forma, sino como simplemente como unos y ceros (la
    cual estos dos valores son representados como true/verdadero
    y false/falso)

    - Moises Aleman
    - Samuel Rueda
*/
typedef char bool;  /* Typedef del booleano que trabaja como char por temas de optimizacion */
#define true  1     /* Verdadero = 1 */
#define false 0     /* Falso = 0 */
#endif