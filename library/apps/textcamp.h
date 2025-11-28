#ifndef TEXTCAMP_H
#define TEXTCAMP_H

/*
    TEXTCAMP_H
    Esta libreria esta dedicada exclusivamente al manejo de campos de entrada,
    siendo un agregado extra para los componentes. Esta libreria facilita
    el manejo de entradas graficos a traves de un buffer de texto que se trabaja
    con memoria dinamica, componentes (POR SUPUESTO, TAMBIEN ES UN COMPONENTE),
    y otras propiedades que le agregan funcionalidad.

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
#include <dos.h>
#include <string.h>

/*
    =======================================
    Importacion de Librerias Personalizadas
    =========================================
*/
#include "comp.h"       /* Libreria de componentes, el corazon de las interfaces */
#include "dynamic.h"    /* Libreria de memoria dinamica */

/*
    =======================================
    Declaracion de Directivas
    =========================================
*/
#define ANCHO_CARACTERES_BLOC 7     /* Ancho fijo para caracteres en Campos de Texto */
#define ALTO_CARACTERES_BLOC  12    /* Alto fijo para caracteres en Campos de Texto */
#define FONDO_CAMPO    19           /* Color del fondo */

/*
    =======================================
    Definicion de Estructuras / Uniones
    =========================================
*/
/* 
	struct CampoTexto
	Estructura responsable de manejar el bloc de notas, trabajandolo a traves de una matriz
    de caracteres y propiedades importantes para facilitar su funcionamiento. 
    Tambien es un componente.
*/
typedef struct {
	char **texto_buffer;                 /* Campo la cual se procesara el texto del bloc de notas */
	Component interfaz;                  /* Componente que representa la interfaz del bloc de notas */
    short longitud;                      /* Longitud actual === MANEJO DE OPERACIONES */
    short pos_cursor_x, pos_cursor_y;    /* Posicion actual del cursor */
    short total_filas,  total_columnas;  /* Dimensiones del campo de texto === PROPIEDADES */
    short inicio_x,     inicio_y;        /* Posicion en pantalla inicial del campo */             
    short color,        fondo;           /* Color de letra/cursor + fondo/borrador */ 
} CampoTexto;

/*
    =======================================
    Definicion de Enums
    =========================================
*/
/*
    enum CommonKeys
    Estructura enum que categoriza el ASCII de las teclas
    conmunente utilizadas para el procesamiento de texto,
    manejando principalmente las teclas de retroceso, enter,
    escape y cualquier caracter imprimible.
*/
typedef enum {
    BACKSPACE  = 8,     /* Retroceso */
    ENTER      = 13,    /* Enter */
    ESCAPE     = 27,    /* Escape */
    SPACE      = 32,    /* Espacio */
    SWUNG_DASH = 126    /* Tilde */
} CommonKeys;

/*
    =======================================
    Declaracion de Prototipos
    =========================================
*/
/* Seccion 1: Administracion de campos de texto */
void constructor_campo_texto(
    CampoTexto *bloc, 
    short total_filas, short total_columnas, 
    short inicio_x, short inicio_y,
    short color, short fondo
);
void destruir_campo_texto(CampoTexto *bloc);

/* Seccion 2: Cursor */
void dibujar_cursor(short pos_x, short pos_y, short color);
void cursor_bloc(CampoTexto *bloc, short modo);
short cursor_en_limite(CampoTexto *bloc);

/* Seccion 3: Dibujado de Entradas */
void dibujar_entrada_campo(CampoTexto *bloc, short modo);
void insertar_caracter_bloc(CampoTexto *bloc, char tecla);
void eliminar_caracter_bloc(CampoTexto *bloc);

/* Seccion 4: Manejo de entradas */
void procesar_campo_texto(CampoTexto *bloc);

/*
    =======================================
    Inicializacion de Funciones
    =========================================
*/
/* Seccion 1: Administracion de bloc de notas */
/*
    constructor_campo_texto()
    Funcion que permite construir un campo de texto. Util para el procesamiento
    del bloc de notas. (OJO, UNA VEZ CREADA ESTA FUNCION DEBES TENER
    PRECAUCION CUANDO DEJARAS DE USARLO, YA QUE TRABAJA CON MEMORIA DINAMICA).

    En pocas palabras, inicializa las propiedades, el componente interno 
    y el buffer de texto a traves de malloc.

    Parametros:
    - CampoTexto *bloc: responsable de manejar la interfaz y logica de bloc de notas.
    - short total_filas, total_columnas: dimensiones del campo de texto.
    - short inicio_x, inicio_y: Posicion en pantalla inicial para procesar el 
        campo de texto.
    - short color, fondo: Colores del campo (Letra + Cursor + Fondo).
*/
void constructor_campo_texto(
    CampoTexto *bloc, 
    short total_filas, short total_columnas, 
    short inicio_x, short inicio_y,
    short color, short fondo
) {
    /* 1. Se reserva memoria para el buffer de texto y luego se inicializa */
    short i; /* Variable de indice */

    /* Reserva a buffer */
    bloc -> texto_buffer = (char **) malloc(sizeof(char *) * total_filas);
    if (!bloc -> texto_buffer) return; /* Y se valida */

    /* Reserva a filas del buffer */
    for (i = 0; i < total_filas; i++)
    {
        bloc -> texto_buffer[i] = (char *) malloc(sizeof(char) * total_columnas);
        if (!bloc -> texto_buffer[i]) return; /* Se valida cada caracter */

        /* Inicializacion a ceros con memset */
        memset(bloc -> texto_buffer[i], 0, total_columnas);
    }

    /* 2. Se construye el componente del campo de texto */
    constructor_componente(
		&bloc -> interfaz, inicio_x, inicio_y, 
        (ANCHO_CARACTERES_BLOC * total_columnas) + inicio_x,
        (ALTO_CARACTERES_BLOC * total_filas) + inicio_y,
        fondo, color, fondo, HOVER_NO_DISPONIBLE
    );

    /* 3. Se inicializan propiedaedes del campo de texto */
    bloc -> longitud     = 0;  /* Longitud en cero */
    bloc -> pos_cursor_x = 0;  /* Posicionar cursor en el inicio respecto a X */
    bloc -> pos_cursor_y = 0;  /* Posicionar cursor en el inicio respecto a Y */

    bloc -> total_columnas  = total_columnas;   /* Cantidad de columnas del campo de texto */
    bloc -> total_filas     = total_filas;      /* Cantidad de filas del campo de texto */
    bloc -> inicio_x        = inicio_x;         /* Posicion inicial en pantalla del campo en X */
    bloc -> inicio_y        = inicio_y;         /* Posicion inicial en pantalla del campo en Y */

    bloc -> color   = color;  /* Color de letra + Cursor */
    bloc -> fondo   = fondo;  /* Color de fondo */
}

/*
    destruir_campo_texto()
    Funcion que libera de la memoria el texto procesado del campo de texto.
    OJO, usar cuando realmente ya no se necesite el uso de un campo de texto.

    Parametros:
    - CampoTexto *bloc: responsable de manejar los campos de texto.
*/
void destruir_campo_texto(CampoTexto *bloc)
{
    /* Utilizar librerar_arreglo de dynamic.h */
    liberar_arreglo((void **) bloc->texto_buffer, bloc->total_filas);
}

/* Seccion 2: Cursor */
/* 
    dibujar_cursor();
    Funcion que dibuja el cursor en dependencia
    de sus posiciones en pantalla ya calculadas.

    short pos_x, pos_y: Posiciones calculadas del cursor.
    short color: Color del cursor.
*/
void dibujar_cursor(short pos_x, short pos_y, short color)
{
    /* Se define el color de linea y luego se dibuja el cursor */
    setcolor(color);
    line(pos_x - 1, pos_y + 2, pos_x - 1, pos_y + ALTO_CARACTERES_BLOC - 1);
}

/*
    cursor_bloc();

    Funcion encargada de administrar el dibujado del cursor del bloc de notas,
    destacando tambien los cuatro modos las cuales puede ser procesado:
    - Modo 0: Solo se dibuja el cursor en su posicion actual.
    - Modo 1: El cursor se desplaza a la izquierda.
    - Modo 2: El cursor se desplaza a la derecha.
    - Modo 3: Se borra el cursor de la posicion actual.

    Paramteros:
    - CampoTexto *bloc: Campo del bloc de notas, de esta se obtendra
    la posicion actual.
    - short modo: El modo la cual sera procesado el cursor (son aquellos
    que se mencionaron en la descripcion).
*/
void cursor_bloc(CampoTexto *bloc, short modo) 
{
    /* 1. Se declaran variables para trabajar con la posicion
    del cursor del bloc, las dimensiones e inicio del campo de 
    texto de forma mas facil */
    short pos_x, pos_y, total_x, total_y, inicio_x, inicio_y;

    /* 2. Se inicializan dimensiones y punto de inicio */
    total_x = bloc -> total_columnas; total_y = bloc -> total_filas;
    inicio_x = bloc -> inicio_x;      inicio_y = bloc -> inicio_y;

    /* Se calcula las posiciones en pantalla */
    pos_x = (bloc -> pos_cursor_x * ANCHO_CARACTERES_BLOC) + inicio_x;
    pos_y = (bloc -> pos_cursor_y * ALTO_CARACTERES_BLOC) + inicio_y;

    /* 3. Se procesa cada uno de las opciones */
    /* Solo dibujar (0) o borrar (3) cursor */
    if (modo == 0 || modo == 3) 
        dibujar_cursor(pos_x, pos_y, modo == 0 ? bloc -> color : bloc -> fondo); /* Se dibuja el cursor */

    /* 4. Desplazarse izquierda (1) o derecha (2) */
    else if (modo == 1 || modo == 2) 
    {
        /* Borrar cursor en posicion actual */
        dibujar_cursor(pos_x, pos_y, bloc -> fondo);

        /* Obtener posiciones del cursor */
        pos_x = bloc -> pos_cursor_x; pos_y = bloc -> pos_cursor_y;

        /* Mover posicion logica del cursor */
        if (modo == 1) /* Izquierda */
        { 
            if (pos_x > 0) /* Si la posicion en x no esta en el borde izquierdo */
                pos_x--;               /* Retroceder a la anterior posicion en X */
            
            else if (pos_y > 0) /* Si la posicion debe retroceder una linea */
            {    
                pos_x = total_x  - 1;    /* En X se pone en la ultima posicion */
                pos_y--;                 /* En Y retrocede a la linea anterior */
            }
        } 
        else /* Derecha */
        { 
            if (pos_x < total_x - 1)   /* El cursor no esta en el borde derecho */
                pos_x++;               /* Avanzar a la siguiente posicion en X */
            else if (pos_y < total_y - 1) /* Si se debe avanzar en la siguiente linea */
            {
                pos_x = 0;             /* Realizar retorno de carro ('\r') */
                pos_y++;               /* Salto de linea ('\n') */
            }
            else if (pos_x == total_x - 1 && pos_y == total_y - 1) /* El cursor esta al final */
                pos_x++;               /* Se avanza igualmente a una posicion */
        }

        /* Calcular nueva posicion y dibujar cursor */
        pos_x = (pos_x * ANCHO_CARACTERES_BLOC) + inicio_x; 
        pos_y = (pos_y * ALTO_CARACTERES_BLOC) + inicio_y;
        dibujar_cursor(pos_x, pos_y, bloc -> color);
    }
}

/*
    cursor_en_limite();
    Funcion que permite saber si el cursor del campo de texto esta dentro
    de los limites. Si esta dentro de los limites devuelve uno, sino,
    devuelve cero.

    Parametros:
    - CampoTexto *bloc: Puntero a campo de texto, la cual obtendremos su cursor.
*/
short cursor_en_limite(CampoTexto *bloc) 
{
    return (bloc -> pos_cursor_x >= 0 && bloc -> pos_cursor_x <= bloc -> total_columnas &&
            bloc -> pos_cursor_y >= 0 && bloc -> pos_cursor_y < bloc -> total_filas);
}

/* Seccion 3: Dibujado de Entradas */
/*
    dibujar_entrada_campo();
    Funcion que renderiza o borra las entradas
    ingresadas por el usuario. Esta trabaja con la posicion
    actual de cursor, pero para ello debe recibir el puntero al 
    campo de texto.

    Parametros:
    - CampoTexto *bloc: Puntero al campo de texto actual.
    - short modo: El modo de dibujo: este corresponde con los siguientes:
        Modo 0: Borrar un caracter.
        Modo 1: Dibujar un caracter.
*/
void dibujar_entrada_campo(CampoTexto *bloc, short modo)
{
    /* 1. Se declara e incializa una cadena para imprimir el caracter */
    char caracter_imprimible[2]; 
    caracter_imprimible[0] = bloc -> texto_buffer[bloc -> pos_cursor_y][bloc -> pos_cursor_x];
    caracter_imprimible[1] = 0;

    /* 2. Se selecciona el color en dependencia del modo, luego se renderiza 
    o borra el caracter */
    setcolor(modo ? bloc -> color : bloc -> fondo);
    mocultar();
    outtextxy(
        (bloc -> pos_cursor_x * ANCHO_CARACTERES_BLOC) + bloc -> inicio_x, 
        (bloc -> pos_cursor_y * ALTO_CARACTERES_BLOC) + bloc -> inicio_y, 
        caracter_imprimible
    );
    mver();
}

/*
    insertar_caracter_bloc();
    Funcion que maneja el ingreso de caracteres a traves de una tecla
    y el buffer de texto.

    Parametros:
    - CampoTexto *bloc: Campo asociado al bloc de notas, que contiene el buffer de
    texto y sus propiedades, utiles para manejar el ingreso.
    - char tecla: Entrada del teclado la cual sera procesada a traves de esta
    funcion.
*/
void insertar_caracter_bloc(CampoTexto *bloc, char tecla) 
{
    /* 1. Se declaran variables para trabajar con la posicion
    del cursor del bloc, las dimensiones e inicio del campo de 
    texto de forma mas facil */
    short pos_x = bloc -> pos_cursor_x, pos_y = bloc -> pos_cursor_y;

    /* 2. Se inicializan dimensiones y punto de inicio */
    short total_x = bloc -> total_columnas, total_y = bloc -> total_filas;

    /* 3. Se valida si el cursor esta dentro del limite permitido */
    if (!cursor_en_limite(bloc)) return;
    
    /* 4. Se ingresa la entrada al buffer de proceso */
    bloc -> texto_buffer[pos_y][pos_x] = tecla;

    /* 5. Se imprime el caracter escrito */
    dibujar_entrada_campo(bloc, 1);     /* Luego se dibuja el caracter a traves de la posicion actual */

    /* 6. La longitud incrementa */
    bloc -> longitud++;

    /* 7. El cursor se mueve dependiendo de las siguientes circunstancias */
    if (pos_x < total_x - 1)    /* Si el cursor en X no sobrepasa el borde derecho */
        bloc -> pos_cursor_x++; /* Avanzar al siguiente caracter */
    
    else if (pos_y < total_y - 1) /* En el caso de que tenga que avanzar a la siguiente linea */
    {
        bloc -> pos_cursor_x = 0;   /* Se realiza retorno de carro */
        bloc -> pos_cursor_y++;     /* Y salto de linea (\r + \n) */
    }
    
    /* Este caso se cubre si el cursor esta en la ultima posicion del caracter, dejandolo
    al final para el renderizado del cursor */
	else if (pos_x == total_x - 1 && pos_y == total_y - 1)  
        bloc -> pos_cursor_x++;
}

/*
    eliminar_caracter_bloc();
    Funcion que maneja el borrado de caracteres, manejando 
    el borrado de derecha a izquierda. Al igual que la funcion
    anterior, esta trabaja con la posicion del cursor para
    el borrado de caracteres.

    Parametros:
    - CampoTexto *bloc: Campo de texto la cual se esta trabajando.
*/
void eliminar_caracter_bloc(CampoTexto *bloc)
{ 
    /* 1. Se declaran variables para trabajar con la posicion
    del cursor del bloc, las dimensiones e inicio del campo de 
    texto de forma mas facil */
    short pos_x = bloc -> pos_cursor_x;
    short pos_y = bloc -> pos_cursor_y;

    /* 2. Se inicializan dimensiones y punto de inicio */
    short total_x = bloc->total_columnas;

    /* 3. Validar posicion y longitud para realizar borrado */
    if (!cursor_en_limite(bloc) || !bloc -> longitud) return;

    /* 4. Antes de realizar el borrado se calcula la posicion logica */
    if (pos_x == 0 && pos_y > 0) 
    {  
        /* Caso de que el cursor este en el limite izquierdo */
        bloc->pos_cursor_x = total_x - 1;   /* Se coloca al final de las columnas */
        bloc->pos_cursor_y--;               /* Y se retrocede a la linea anterior */
    } 
    
    /* Caso generico en el que el cursor pueda borrar a la izquiera */
    else if (pos_x > 0) 
        bloc->pos_cursor_x--;               /* Se retrocede al siguiente caracter */

    /* 6. Actualizar posiciones despuos del movimiento */
    pos_x = bloc->pos_cursor_x; pos_y = bloc->pos_cursor_y;

    /* 7. Se realiza el borrado del caracter */
    dibujar_entrada_campo(bloc, 0); /* Se borra el caracter anterior */

    /* Se elimina del buffer y la longitud decrementa */
    bloc -> texto_buffer[pos_y][pos_x] = 0;
    bloc -> longitud--;
}

/* Seccion 4: Manejo de entradas */
/*
    procesar_campo_texto();
    Funcion base para trabajar con el procesamiento del
    texto, ya que gracias al teclado se puede procesar la 
    escritura de los campos de texto, realizandose de
    forma interna (el buffer del campo de texto) y de
    forma externa (el dibujado de cada entrada).

    CampoTexto *bloc: Campo de texto la cual se procesara.
    CampTextProcess modo: Modo de procesamiento, siendo los siguientes:
        - 0/PROCESAMIENTO: Solo se reciben las entradas.
        - 1/GUARDADO: Se trata como un campo de guardado.

    La razon de meter modos es para cambiar las teclas a utilizar,
    siendo el ENTER muy importante para el campo de texto de guardado.

    Devuelve 1 si se salio con la opcion de guardar seleccionada.
    Devuelve 0 si se salio pero sin gurardar.
*/
void procesar_campo_texto(CampoTexto *bloc)
{
    /* 1. Se declara una variable para procesar las entradas
    y otra para trabajar con el tamano total del campo de texto */
    unsigned char tecla;
    short limite_campo = bloc -> total_columnas * bloc -> total_filas;

    /* Se limpia el buffer de entradas */
    while (kbhit()) getch();

    /* 2. ==== FUNCIONAMIENTO DE PROCESAMIENTO DE TEXTO ==== */
    do  /* Se utiliza un do while para trabajar con el procesamiento */
    {
        /* 3. Se recibe una entrada del teclado y se asgina a la
        variable de tecla */
		tecla = getch();

        /* 4. Se procesa cada entrada a traves de un if else */
        if (tecla == 0) 
        { while (kbhit()) getch(); continue; }
           
        /* Caso: Escape/ESCAPE/13 */
        if (tecla == ESCAPE)
            return;  /* Se desactiva el foco (se detiene el proceso de campo de texto) */
    
        /* Caso: Retroceso/BACKSPACE/8 */
        else if (tecla == BACKSPACE)
        {
            /* Se dibuja el cursor, despues se quita el caracter */
            cursor_bloc(bloc, 1); eliminar_caracter_bloc(bloc);
        }

        /* Caso: cualquier imprimible si la longitud no sobrepasa el limite */
        else if (tecla >= SPACE && tecla <= SWUNG_DASH && bloc -> longitud < limite_campo)
        {
            /* Se dibuja primero el cursor, luego la entrada */
            cursor_bloc(bloc, 2); insertar_caracter_bloc(bloc, tecla);
        }

        /* Se pone como condicional 1 por ser muy eficiente en memoria, ya que elimina
        procesos muy repetitivos. La unica forma de poder detener el bucle es a
        traves de break o return (o cambiando la funcion) */
    } while (1); 
}
#endif