/*
																																						===== Guia para crear aplicaciones =====
	En este archivo C se demuestra como usar las funciones de las librerias
    appadmin.h y comp.h, importantes para la creacion de interfaces, tanto
    para las aplicaciones y como del escritorio del sistema operativo.

    Para este proceso se mantiene el enfoque de que el programador debe crear su
    entorno semipreparado con estas librerias, definiendo los componentes a usar
    en base a sus estructuras, y luego aplicar los callbacks en un bucle.

    Incluso esta funcion les puede servir como plantilla para crear sus apps.

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
#include "appadmin.h"   /* Libreria de gestor de aplicaciones */
#include "dynamic.h"    /* Libreria de memoria dinamica */

/*
    =======================================
    Declaracion de Variables Globales
    =========================================
*/
/* Cadena que contiene el directorio del fichero de bloc */
char ruta_bloc[] = "blocdata.bin"; 

/* Cadena que contiene el directorio del fichero de estado */
char ruta_bloc_temp[] = "bloctemp.bin"; 

/*
    =======================================
    Definicion de Estructuras / Uniones
    =========================================
*/

/* 
	Estructura CampoBloc
	Estructura responsable de manejar el bloc de notas, trabajandolo a traves de un fichero
    y como un campo de texto que contiene diferentes caracteristicas. Tambien es un componente.
*/
typedef struct {
    FILE *bloc;              /* Fichero que guarda y lee los datos del bloc */
    FILE *extra;             /* Fichero que respalda al anterior */
	Component interfaz;      /* Componente que representa la interfaz del bloc de notas*/
    short longitud;          /* Longitud actual */
    short foco;              /* Si está seleccionado */
    short pos;               /* Posicion actual del cursor */
} CampoBloc;

/*
    =======================================
    Definicion de Enum
    =========================================
*/
/* 
	Enum FileMode
	Enum que indica si se guardara o se cargara el fichero.
*/
typedef enum {
    LOAD_MODE,
    SAVE_MODE,
    DELETE_CHAR
} FileMode;

/*
    =======================================
    Declaracion de Prototipos
    =========================================
*/
/* Seccion 1: Administracion de bloc */
short inicializar_bloc(CampoBloc *bloc);
void inicializar_campo_bloc(CampoBloc *bloc);
void cerrar_bloc(CampoBloc *bloc);

/* Seccion 2: Operaciones con buffer adicional */
void datos_bloc(CampoBloc *bloc, FileMode mode);

/* Seccion 3: Operaciones basicas a campos de bloc */
short contar_caracteres_file(FILE *stream);
void eliminar_caracter(CampoBloc *bloc);

/* Seccion 4: Renderizado de bloc de notas */


/*
    =======================================
    Inicializacion de Funciones
    =========================================
*/

/* Seccion 1: Administracion de bloc */
/*
	inicializar_bloc()
	Funcion que inicializa el fichero la cual se va a trabajar como bloc de notas.

    Parametros:
    - CampoBloc *bloc: responsable de manejar la interfaz y logica de bloc de notas.

    Devuelve 1 si bloc se pudo inicializar, sino, devuelve 0.
*/
short inicializar_bloc(CampoBloc *bloc)
{    
    /* Se crea/abre el fichero y evalua si el bloc se cargo correctamente */
	bloc -> bloc = fopen(ruta_bloc, "a+b");
    if ((bloc -> bloc) == NULL) return 0;

    /* Se retorna 1, indicando que se inicializo con exito */
    return 1;
}

/*
    inicializar_campo_bloc()
    Inicializa el campo de texto del bloc y carga en caso de haberse guardado.

    Parametros:
    - CampoBloc *bloc: responsable de manejar la interfaz y logica de bloc de notas.
*/
void inicializar_campo_bloc(CampoBloc *bloc) {
    /* 1. Se inicializan los valores en cero */
    bloc -> foco = 0; /* No hay foco en el campo del bloc */

    /* 2. Se recarga donde se guardo por ultima vez */
    datos_bloc(bloc, LOAD_MODE);
 
    /* 3. Desplazamos el puntero del fichero hasta el final */
    fseek(bloc -> bloc, 0, SEEK_END);
}

/* 
    cerrar_bloc()
    Funcion que cierra el bloc.

    Parametros:
    - CampoBloc *bloc: responsable de manejar la interfaz y logica de bloc de notas.
*/
void cerrar_bloc(CampoBloc *bloc)
{
    /* Se vacia y cierra el bloc */
    bloc -> bloc = freopen(ruta_bloc, "wb", bloc -> bloc);
    fclose(bloc -> bloc);
}

/* Seccion 2: Operaciones con buffer adicional */
/*
    datos_bloc()
    Funcion que carga o guarda los datos guardados del fichero bloc.

    Parametros:
    - CampoBloc *bloc: responsable de manejar la interfaz y logica de bloc de notas.
*/
void datos_bloc(CampoBloc *bloc, FileMode mode)
{
    /* 1. Declarar un buffer de guardado temporal */
    char *temp_buffer;

    /* 4. Se decide que operacion de datos de fichero realizar */
    if (mode == SAVE_MODE && bloc -> longitud >= 0) /* Caso: Guardado */
    {
        /* Guardar posicion actual de bloc */
        short pos = ftell(bloc -> bloc);

        /* Crear fichero temporal */
        bloc -> extra = fopen(ruta_bloc_temp, "w+b");
        if (ferror(bloc -> extra)) return; /* Validar si no ocurrio un error al abrir el fichero */

        bloc -> longitud = contar_caracteres_file(bloc -> bloc);

        /* Reservar memoria */
        temp_buffer = (char *) malloc(bloc -> longitud);
        if (!temp_buffer) return; /* Verificar reserva */

        /* Rebobinar los ficheros */
        rewind(bloc -> bloc);

        /* Se escribe al buffer el texto procesado actualmente en bloc */
        fread(temp_buffer, sizeof(char), bloc -> longitud, bloc -> bloc);
        if (ferror(bloc -> bloc)) { printf("Error: Linea 203\n"); return; } /* Validar si no ocurrio un error en el guardado */
    
        /* Ahora se escribe del buffer al fichero de estado */
        fwrite(temp_buffer, sizeof(char), bloc -> longitud, bloc -> extra);
        if (ferror(bloc -> extra)) { printf("Error: Linea 207\n"); return; }

        /* Recuperar posicion actual */
        fseek(bloc -> bloc, pos, SEEK_SET);

        /* Se libera el buffer */
        liberar_elemento(temp_buffer);
    }

    else if (mode == LOAD_MODE && bloc -> longitud >= 0) /* Caso: Cargado */
    {
        /* Crear fichero temporal */
        bloc -> extra = fopen(ruta_bloc_temp, "a+b");
        if (ferror(bloc -> extra)) return; /* Validar si no ocurrio un error al abrir el fichero */

        bloc -> longitud = contar_caracteres_file(bloc -> extra);

        /* Reservar memoria */
        temp_buffer = (char *) malloc(bloc -> longitud);
        if (!temp_buffer) return; /* Verificar reserva */

        /* Se escribe al buffer el texto guardado en temp */
        fread(temp_buffer, sizeof(char), bloc -> longitud, bloc -> extra);
        if (ferror(bloc -> extra)) { printf("Error: Linea 228\n"); return; } /* Validar si no ocurrio un error en el guardado */
    
        /* Ahora se escribe del buffer al fichero de estado */
        fwrite(temp_buffer, sizeof(char), bloc -> longitud, bloc -> bloc);
        if (ferror(bloc -> bloc)) { printf("Error: Linea 232\n"); return; }

        /* Se libera el buffer */
        liberar_elemento(temp_buffer);
    }

    /* 5. Se cierra el fichero de estado */
    fclose(bloc -> extra);
}

/*
	contar_caracteres_file()
	Funcion que devuelve la longitud de un fichero binario.

    Parametros:
    - FILE *stream: Fichero la cual se opera el bloc de notas.
*/
short contar_caracteres_file(FILE *stream)
{
	/* Se calcula el tamaño de la siguiente forma */
    /* 1. Declaramos variable que contendra la longitud del fichero */
    short longitud;

    /* 2. Se obtiene la posicion actual del fichero */
    short pos = ftell(stream);
    
    /* 3. Se desplaza hasta el final del fichero */
    fseek(stream, 0, SEEK_END);       
    
    /* 4. Se captura la cantidad de elementos en base a la posicion */
	longitud = ftell(stream);

    /* 5. Se desplaza hasta el punto donde estabamos */
    fseek(stream, pos, SEEK_SET);     

    /* 6. Retorna la longitud */
    return longitud;
}

/*
	eliminar_caracter();
	Funcion que elimina el caracter anterior al cursor.

    Parametros:
    - CampoBloc *bloc: Estructura que administra el bloc de notas.
*/
void eliminar_caracter(CampoBloc *bloc)
{
    char *temp_buffer;
	short pos_actual;

    /* 1. Se verifica si tiene la longitud indicada para eliminar el caracter */
    if (bloc -> longitud <= 0) return;

    /* 2. Se reserva memoria */
    temp_buffer = (char *) malloc(bloc -> longitud);
    if (!temp_buffer) return; /* Verificar reserva */

    /* 3. Se guarda la posicion actual */
    pos_actual = ftell(bloc -> bloc);

    /* 4. Leer fichero actual para ser procesado despues a traves de buffer */
	rewind(bloc -> bloc);
    fread(temp_buffer, sizeof(char), bloc -> longitud, bloc -> bloc);

    /* 5. Se vacia el fichero actual y se rellena con lo anteriormente guardado */
    bloc -> bloc = freopen(ruta_bloc, "w+b", bloc -> bloc);

    /* Izquierda */
	rewind(bloc -> bloc);
    fwrite(temp_buffer, sizeof(char), pos_actual - 1, bloc -> bloc);

    /* Derecha */
    fseek(bloc -> bloc, pos_actual, SEEK_SET);
    fwrite(temp_buffer, sizeof(char), bloc -> longitud - pos_actual, bloc -> bloc);

    /* 6. Se ubica en la posicion correspondiente */
    bloc -> longitud = pos_actual - 1;
    fseek(bloc -> bloc, pos_actual - 1, SEEK_SET);

    /* 7. Se libera el buffer */
    liberar_elemento(temp_buffer);
}

/*
    =======================================
    Callbacks
    =========================================
*/

/* Funciones Callbacks para Menu Despegable */
void callback_guardar() {
    colocar_configuraciones();
    mocultar();
	outtextxy(60, 100, "Opcion: Guardar - Archivo guardado!");
    mver();
}

/*
    manejar_opcion_menu() - FUNCIÓN ÚNICA PARA TODAS LAS OPCIONES
    - El programador novato solo necesita modificar esta función
    - Muy fácil de entender y mantener
*/
void manejar_opcion_menu_bloc(CampoBloc *bloc, int opcion_seleccionada)
{
    switch (opcion_seleccionada) {
        case 0:  /* Primera opción del menú */
			callback_guardar();
            datos_bloc(bloc, SAVE_MODE);
            break;
    }
}

/*
    =======================================
    Funciones simplificadas
    =========================================
*/

/*
    renderizar_caracter()
    - Dibuja un caracter
*/
void renderizar_caracter(CampoBloc *bloc)
{
    short i;
    short x = bloc -> interfaz.x1, y = bloc -> interfaz.y1;
    char str[2];
    
    str[1] = '\0';
    bloc -> pos = ftell(bloc -> bloc);
    rewind(bloc -> bloc);

    mocultar();
	renderizar_componente(&bloc -> interfaz);
    colocar_configuraciones();

    for (i = 0; i < bloc -> longitud; i++)
    {
        fread(&str[0], 1, 1, bloc -> bloc);

        if (str[0] == 13)
        {
            x = bloc -> interfaz.x1;    /* Retorno de carro */
            y += textheight(str);       /* Nueva linea */
        }
        else
            x += textwidth(str);    /* Se desplaza*/

        outtextxy(x, y, str);
    }
    
    mver();

    fseek(bloc -> bloc, bloc -> pos, SEEK_SET);
}

/*
    funcion_campo_bloc()
    - Funcion que maneja las entradas del bloc de notas.

    Parametros:
    CampoBloc *bloc: responsable de manejar la interfaz y logica de bloc de notas.
    short mouse_x: posicion capturada del mouse en X.
    short mouse_y: posicion capturada del mouse en Y.
*/
void funcion_campo_bloc(CampoBloc *bloc, short mouse_x, short mouse_y) {
	/* 1. Se detecta si se hizo click en el campo del bloc */
	if (detectar_click_componente(&bloc->interfaz, mouse_x, mouse_y))
    {
        /* Se activa el bloc de notas, pero antes se hace limpieza del buffer */
        while (kbhit()) getch();
        /* Y el bloc se enfoca */
		bloc -> foco = 1;
        return; /* La funcion se detiene para evitar conflicto con el getch */
    }
    
    /* 2. Se detecta si se esta tecleando al campo del bloc */
    if (bloc -> foco && kbhit()) {
        /* Se captura la tecla presionada para ser procesada */
        char tecla = getch();
        
        /* Casos Especiales de Teclas */
        if (tecla == 27) /* Caso: Escape */
        { 
            /* Se quita el foco */
            bloc -> foco = 0; 
        }

        else if (tecla == 8) { /* Caso: Retroceso */
            /* Se elimina el caracter anterior */
            eliminar_caracter(bloc);
			renderizar_caracter(bloc);
        }

        /* Caso: Caracteres Imprimibles Permitidos */
        /* RECORDATORIO: OPERAR ENTER */
        else if (tecla == 13 || tecla >= 32 && tecla <= 126 && bloc -> longitud < 255)
        {
            /* Se escribe el caracter ingresado al bloc */
            fwrite(&tecla, sizeof(char), 1, bloc -> bloc);

            /* La longitud incrementa */
            bloc -> longitud++;
            
            /* Se imprimen los caracteres escritos/borrados en el campo bloc */
			renderizar_caracter(bloc);
            
        }
    }
}

/*
	=======================================
    Programa Principal
    =========================================
*/
int main() {
    /* 1. Declaracion de componentes y variables */
    /* Declarar componentes principales (NO TOCAR) */
    Component barra_de_ventana, cerrar, despliegue;

    /* Declarar componentes personalizados */
    /* ==================================== */
    CampoBloc editor;

    /* ==================================== */

    /* Declarar la cantidad de opciones del menu desplegable a utilizar */
    char **opciones_desplegables; /* NO TOCAR */
    unsigned char n_opciones = 1; /* Asignar cuantas opciones habran del menu despegable */

    /* Declarar variables del funcionamiento del menu de despliegue (NO TOCAR) */
    int *desplegable_buffer = NULL;
    unsigned char despliegue_activo = 0;
    unsigned char salir = 0;

    /* 2. Inicializar modo grafico */
    if (iniciar_modo_svga_256("C:\\TC20\\BIN")) return EXIT_FAILURE;

    /* 3. Limpiar y ajustar las configuraciones por defecto, y rellenar fondo */
    colocar_configuraciones();
    set_bg(WHITE);

    /* 4. Reservar memoria para las opciones a utilizar (NO TOCAR) */
    reservar_opciones_desplegables(&opciones_desplegables, n_opciones);

    /* 5. Definir nombres de las opciones del menu de despliegue (maximo 20 caracteres) */
    /* ==================================== */
    strcpy(opciones_desplegables[0], "Guardar");
    /* ==================================== */

    /* 6. Inicializar barra de ventana (NO TOCAR) */
    inicializar_barra_ventana(&barra_de_ventana, &cerrar, &despliegue, "Bloc de Notas");

    /* 7. Inicializar componentes personalizados */
    /* ==================================== */
    /* Campo de BLoc de Notas */
    constructor_componente(
		&editor.interfaz,
        5, 14, WIDTH - 5, HEIGHT - 14,
        WHITE, BLACK, WHITE,
        HOVER_NO_DISPONIBLE
    );
    /* ==================================== */

    /* 8. Renderizar componentes personalizados */
    /* ==================================== */
    /* Campo de Bloc de Notas */
	renderizar_componente(&editor.interfaz);
    /* ==================================== */

    /* 9. Inicializar campo de bloc de notas */
	if (!inicializar_bloc(&editor)) return EXIT_FAILURE;
	inicializar_campo_bloc(&editor);
    renderizar_caracter(&editor);

    /* 10. Activar raton (NO TOCAR) */
    mver();

	do {
        /* 11. Capturar la posicion actual del mouse */
        short mouse_x = mxpos(), mouse_y = mypos();
        unsigned char resultado; /* Controlador de barra de tareas */

        /* ===================================
         HOVERS Y CALLBACKS DE BARRA DE VENTANA
        ====================================== */

        /* ===== HOVERS ===== */
        manejar_hover_componente(&cerrar, mouse_x, mouse_y, renderizar_boton_cerrar);
        manejar_hover_componente(&despliegue, mouse_x, mouse_y, renderizar_boton_despliegue);

        /* ===== CALLBACKS ===== */
        /* Manejo completo de la barra de ventana */
        resultado = manejar_barra_ventana(
            &cerrar, &despliegue, mouse_x, mouse_y,
            &despliegue_activo, n_opciones,
            opciones_desplegables, &desplegable_buffer
        );

        /* Procesar resultados */
        if (resultado == n_opciones) salir = 1;
		else if (resultado >= 0) manejar_opcion_menu_bloc(&editor, resultado);

        /* ===================================
           HOVERS Y CALLBACKS PERSONALIZADOS
        ====================================== */
        funcion_campo_bloc(&editor, mouse_x, mouse_y);

        delay(100);
	} while (!salir);

	/* 17. Liberar memoria (NO TOCAR) */
    liberar_buffer_desplegable(&desplegable_buffer);
	liberar_arreglo(&opciones_desplegables, n_opciones);

    /* 17. Cerrar bloc */
	cerrar_bloc(&editor);

    /* 18. Cerrar modo grafico */
    closegraph();
	return 0;
}