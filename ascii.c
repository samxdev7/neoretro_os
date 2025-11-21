#include <stdio.h>
#include <conio.h>

int main()
{
	char tecla1, tecla2;
	clrscr();

    printf("Presione una tecla:\n");
    tecla1 = getch();

    printf("La tecla presionada es la siguiente: \n");
    printf("Char: %c\n", (tecla2 = getch()));
    printf("Int: %d\n", tecla2);

	getch();
    return 0;
}