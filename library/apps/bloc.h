/*
	BLOC_NOTAS_H

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
#include "textcamp.h"   /* Libreria de campos de texto */

/*
    =======================================
    Declaracion de Directivas
    =========================================
*/
/* Cantidades */
#define TOTAL_FILAS    15  /* Cantidad de filas del Bloc de Notas */
#define TOTAL_COLUMNAS 43  /* Cantidad de columnas del Bloc de Notas */

/* Posiciones */
#define INICIO_BLOC_X 7    /* Posicion de inicio en X del Bloc de Notas */
#define INICIO_BLOC_Y 15   /* Posicion de inicio en Y del Bloc de Notas */

/*
    =======================================
    Declaracion de Prototipos
    =========================================
*/
/* Seccion 1: Funciones del bloc de notas */
short guardar_bloc(CampoTexto *bloc);
short cargar_bloc(CampoTexto *bloc);
short activar_campo_bloc(CampoTexto *bloc, short mouse_x, short mouse_y, short despliegue_activo);
void manejar_opcion_menu_bloc(short opcion_seleccionada, CampoTexto *bloc) ;

/* Seccion 2: Aplicacion de Bloc de Notas (Programa Principal) */
void app_bloc_de_notas(void);

/*
    =======================================
    Inicializacion de Funciones
    =========================================
*/
/* Seccion 1: Funciones del bloc de notas */
/*
    guardar_bloc();
    Funcion que permite guardar el bloc de notas actual a traves de
    ficheros.

    Parametros:
    - CampoTexto *bloc: Campo de bloc de notas procesado a traves
    de un buffer de texto.
*/
short guardar_bloc(CampoTexto *bloc)
{
    /* 1. Se declaran las siguientes variables */
    FILE *archivo;      /* Apuntador a FILE para guardar fichero */
    short x, y;         /* Variables de posicion */
	short stop = 0;     /* Variable bandera */
    char salto = '\n';  /* Variable que maneja del salto de linea '\n' */                 

    /* 2. Se crea el fichero */
    archivo = fopen("bloc.bin", "wb");
    
    /* 3. A su vez este se valida */
    if (ferror(archivo)) { fclose(archivo); return EXIT_FAILURE; }

    /* 4. Se guardan metadatos al inicio */
    /* Longitud + Cursor en X + Cursor en Y */
    fwrite(&bloc -> longitud, sizeof(short), 1, archivo);
    fwrite(&bloc -> pos_cursor_x, sizeof(short), 1, archivo);
    fwrite(&bloc -> pos_cursor_y, sizeof(short), 1, archivo);

    /* 5. Se guarda el fichero a traves de bucles, permitiendo el
    recorrido */
    for (x = 0; x < bloc -> total_filas; x++)  /* Recorrido por filas */
    {
        for (y = 0; y < bloc -> total_columnas; y++)   /* Recorrido por caracter en la fila */
        {
            /* Se verifica si la posicion actual esta dentro de la cantidad de
            caracteres escritos por el usuario, si se excede se detienen 
            los dos bucles a traves de la bandera */            
            if (y + (x * bloc -> total_columnas) > bloc -> longitud)
            { stop = 1; break; }

            /* Se guarda el caracter procesado, desde el primero hasta el ultimo
            escrito */
			fwrite(&bloc -> texto_buffer[x][y], sizeof(char), 1, archivo);
        }

        /* Tambien en caso de que se termine de leer una fila
        pero que no se haya terminado de leer todo el bloc, se inscribe
        en el fichero un salto de linea */
        if (stop) break;
        fwrite(&salto, sizeof(char), 1, archivo);
    }

    /* 6. Finalmente se cierra el fichero y se indica el exito de operacion */
    fclose(archivo); return EXIT_SUCCESS;
}

/*
    cargar_bloc();
    Funcion que carga los ficheros las cuales contienen bloc de notas
    guardados anteriormente, a su vez LOS DIBUJA.

    Parametros:
    - CampoTexto *bloc: Campo de bloc de notas procesado a traves
    de un buffer de texto.
*/
short cargar_bloc(CampoTexto *bloc)
{
    /* 1. Se declaran las siguientes variables */
    FILE *archivo;                /* Apuntador a FILE para guardar fichero */
    short x, y;                   /* Variables de posicion */
    short stop = 0;               /* Variable bandera */
    short pos_x_temp, pos_y_temp; /* Variables que guardan la posicion actual del bloc a cargar */

    /* 2. Se abre el fichero */
    archivo = fopen("bloc.bin", "rb");
    
    /* 3. A su vez este se valida */
    if (ferror(archivo)) { fclose(archivo); return EXIT_FAILURE; }

    /* 4. Se limpian datos y se cargan los guardados con anterioridad */
    bloc -> pos_cursor_x = 0; bloc -> pos_cursor_y = 0;
    
    /* Longitud + Cursor en X + Cursor en Y */
    fread(&bloc -> longitud, sizeof(short), 1, archivo);
    fread(&pos_x_temp, sizeof(short), 1, archivo);
    fread(&pos_y_temp, sizeof(short), 1, archivo);

    /* 5. Se limpia el campo y se inicaliza caracter imprimible */
    mocultar();
	renderizar_componente(&bloc -> interfaz);

    /* 6. Se recorre cada caracter del fichero para guardarlo
    en el buffer y a su vez imprimirlo */
    for (x = 0; x < bloc -> total_filas; x++)  /* Recorrido por filas */
    {
        setcolor(WHITE);
        for (y = 0; y < bloc -> total_columnas; y++)   /* Recorrido por caracter en la fila */
        {
            /* Se valida la longitud del fichero */
            if (y + (x * bloc -> total_columnas) > bloc -> longitud)
            { stop = 1; break; }

            /* Se carga el caracter procesado, guardandose en el buffer de texto */
			fread(&bloc -> texto_buffer[x][y], sizeof(char), 1, archivo);
            
            /* Se dibuja y se avanza a la siguiente posicion */
            dibujar_entrada_campo(bloc, 1);
            bloc -> pos_cursor_x++;
        }
        bloc -> pos_cursor_x = 0;       /* Se reinicia el cursor en X */
        bloc -> pos_cursor_y++;         /* Se incrementa el cursor en y */
        if (stop) break;                /* Se evalua si se detendra */
        fseek(archivo, 1, SEEK_CUR);    /* Sino se detendra, se descarta el salto de linea */
    }
    mver();

    /* Posiciona cursor en donde estaba antes */
    bloc -> pos_cursor_x = pos_x_temp;
    bloc -> pos_cursor_y = pos_y_temp;
    cursor_bloc(bloc, 3); /* Se borra cursor en todo caso */
    return EXIT_SUCCESS;  /* Se indica que se realizo con exito el cargado de bloc */
}

/*
    funcion_campo_bloc()
    Funcion que maneja la logica del campo de bloc de notas. Esta logica incluye:
    - Insertado de caracteres.
    - Eliminado de caracteres.
    - Teclas especiales.

    Parametros:
    CampoTexto *bloc: responsable de manejar la interfaz y logica de bloc de notas.
    short mouse_x: posicion capturada del mouse en X.
    short mouse_y: posicion capturada del mouse en Y.
    short despliegue_activo: bandera del despliegue del menu.
*/
short activar_campo_bloc(CampoTexto *bloc, short mouse_x, short mouse_y, short despliegue_activo)
{
	/* 1. Se detecta si se hizo click en el campo del bloc */
	if (detectar_click_componente(&bloc->interfaz, mouse_x, mouse_y) && !despliegue_activo)
    {
        /* 1. Se dibuja la barra de indicacion + el cursor */
        dibujar_mensaje_ventana("Pulse ESC para regresar");
        cursor_bloc(bloc, 0);

        /* 2. Se procesa el bloc de notas */
		procesar_campo_texto(bloc);
        return 1;
    }

    /* 2. En caso de que no se activo ni se proceso se indica a traves
    de retornar cero */
    return 0;
}

/*
    manejar_opcion_menu_bloc()
    Funcion que maneja las opciones del menu de despliegue, pero varia
	principalmente para el desplieuge del bloc de notas.
*/
void manejar_opcion_menu_bloc(short opcion_seleccionada, CampoTexto *bloc) 
{
    /* A traves de un switch y de la opcion seleccionada se guarda
    o se selecciona a traves de la opcion seleccionada */
    switch (opcion_seleccionada) {
        case 0:  /* Opcion: Guardar Bloc de Notas */
            guardar_bloc(bloc); break;
        case 1:  /* Opcion: Cargar Bloc de Notas */
            cargar_bloc(bloc);  break;
    }
}

/* Seccion 2: Aplicacion de Bloc de Notas (Programa Principal) */
void app_bloc_de_notas(void) 
{
    /* 1. Declaracion de componentes y variables */
    /* Declarar componentes de barra de ventana */
    Component barra_de_ventana, cerrar, despliegue;
    CampoTexto editor; /* Declarar campo de bloc de notas */

    /* Declarar la cantidad de opciones del menu desplegable a utilizar */
    char **opciones_desplegables;
    unsigned char n_opciones = 2; /* Asignar cuantas opciones habran del menu despegable */

    /* Declarar variables del funcionamiento del menu de despliegue (NO TOCAR) */
    int *desplegable_buffer = NULL;
    unsigned char despliegue_activo = 0;
    
    /* 2. Rellenar pantalla con color de fondo y ajustar las configuraciones por defecto */
    set_bg(FONDO_CAMPO); colocar_configuraciones();

    /* 3. Reservar memoria para las opciones a utilizar */
    reservar_opciones_desplegables(&opciones_desplegables, n_opciones);

    /* 4. Definir nombres de las opciones del menu de despliegue */
    strcpy(opciones_desplegables[0], "Guardar bloc");
    strcpy(opciones_desplegables[1], "Cargar bloc");

    /* 5. Inicializar barra de ventana */
    inicializar_barra_ventana(&barra_de_ventana, &cerrar, &despliegue, "Bloc de Notas");

    /* 6. Inicializar bloc de notas (componente + propiedades) */
    constructor_campo_texto(&editor, TOTAL_FILAS, TOTAL_COLUMNAS, 
        INICIO_BLOC_X, INICIO_BLOC_Y, WHITE, FONDO_CAMPO);

    /* 7. Renderizar componentes personalizados */
    /* Campo de Bloc de Notas */
	renderizar_componente(&editor.interfaz);

    /* 8. Activar raton y volver a colocar configuraciones de texto */
    mver(); setcolor(WHITE);

	do {
        /* 9. Capturar la posicion actual del mouse */
        short mouse_x = mxpos(), mouse_y = mypos();
        short resultado; /* Controlador de barra de tareas */
        
        /* ====================================
           HOVERS Y CALLBACKS DE BLOC DE NOTAS
        ======================================= */
        /* Se evalua si se activara la mecanica del bloc de notas */
        if (activar_campo_bloc(&editor, mouse_x, mouse_y, despliegue_activo))
        {
            /* Una vez hecho eso se ejecuta todo el bloc de notas, al finalizar
            vuelve a dibujar la barra de ventana y se recuperan los callbacks */
            mocultar();
            renderizar_componente_texto(&barra_de_ventana, "Bloc de Notas");
            renderizar_boton_cerrar(&cerrar); renderizar_boton_despliegue(&despliegue);
            
            /* Y se quita el cursor */
			cursor_bloc(&editor, 3);
            mver();
        }

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

        /* Procesar resultados  */
        if (resultado == n_opciones) break;
        else if (resultado >= 0) 
            manejar_opcion_menu_bloc(resultado, &editor);

        delay(50);
	} while (1);

	/* 10. Liberar memoria y destruir campo de texto */
    destruir_campo_texto(&editor);
    liberar_buffer_desplegable(&desplegable_buffer);
	liberar_arreglo((void *) &opciones_desplegables, n_opciones);
}