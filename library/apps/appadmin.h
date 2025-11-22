#ifndef APPADMIN_H
#define APPADMIN_H

/*
	APPADMIN_H
	Esta libreria se dedica a gestionar las aplicaciones del sistema operativo, basandose
    principalmente en la barra de ventana y el manejo del menu de despliegue.
	
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

#include "mouse256.h"   /* Modificacion de la libreria Mouse.h, utilizada para trabajar con el mouse a 320x200 */
#include "gphadmin.h"   /* Libreria que administra el modo grafico del sistema operativo */
#include "comp.h"       /* Libreria que administra componentes, es decir, botones, secciones, cajas de texto, etc.. */

/*
    =======================================
    Declaracion de Prototipos
    =========================================
*/
/* Seccion 1: Inicializacion de barra de ventana */
void inicializar_barra_ventana(Component *barra, Component *cerrar, Component *despliegue, char *titulo);

/* Seccion 2: Funciones de renderizado para barra de ventana */
void renderizar_borde(Component *componente);
void renderizar_boton_cerrar(Component *componente);
void renderizar_boton_despliegue(Component *componente);

/* Seccion 3: Renderizado de barra de ventana */
void renderizar_barra_de_ventana(
    Component *barra_de_ventana,
    Component *cerrar,
    Component *despliegue,
    char *titulo
);

/* Seccion 4: Funciones de buffer de menu desplegable */
void crear_buffer_desplegable(Component *interfaz, int **buffer);
void guardar_buffer_desplegable(Component *interfaz, int **buffer);
void liberar_buffer_desplegable(int **buffer);
int mostrar_menu_desplegable(unsigned short n_opciones, char **opciones_texto, int **buffer);
int ocultar_menu_desplegable(int **buffer);

/* Seccion 5: Funciones de control de barra de ventana */
short manejar_barra_ventana(
    Component *cerrar,
    Component *despliegue, 
    short mouse_x, short mouse_y,
    unsigned char *despliegue_activo,
    unsigned char n_opciones,
    char **opciones_desplegables,
    int **desplegable_buffer
);
short detectar_opcion_menu(
    short xi, short yi, 
    short mouse_x, short mouse_y, 
    int n_opciones, 
    char **opciones_desplegables
);

/*
    =======================================
    Inicializacion de Funciones
    =========================================
*/
/* Seccion 1: Inicializacion de barra de ventana */
/*
    inicializar_barra_ventana()
    - Funcion que inicializa la barra de ventana.

	- Parametros:
	Component *componente: Componente que se pasa por referencia, corresponde
	con el boton de cerrar.
    Component *barra: Componente que se pasa por referencia, corresponde
	con la barra de ventana.
    Component *cerrar: Componente que se pasa por referencia, corresponde
	con el boton de cerrar. 
    Component *despliegue: Componente que se pasa por referencia, corresponde
	con el boton de despligue.
    char *titulo: Cadena que corresponde con el titulo de la aplicacion.
*/
void inicializar_barra_ventana(Component *barra, Component *cerrar, Component *despliegue, char *titulo)
{
    /* Inicializar barra de ventana */
    constructor_componente(barra, 0, 0, WIDTH, 11, BLACK, WHITE, BLACK, HOVER_NO_DISPONIBLE);

    /* Inicializar boton cerrar */
    constructor_componente(cerrar, 0, 0, 11, 11, BLACK, WHITE, 40, HOVER_DISPONIBLE);
    constructor_hover_componente(cerrar, 40, WHITE, 40);

    /* Inicializar boton despliegue */
    constructor_componente(despliegue, 12, 0, 23, 11, BLACK, WHITE, 55, HOVER_DISPONIBLE);
    constructor_hover_componente(despliegue, 55, WHITE, 55);

    /* Renderizar componentes */
    renderizar_barra_de_ventana(barra, cerrar, despliegue, titulo);
}

/* Seccion 2: Funciones de renderizado para barra de ventana */
/*
    renderizar_borde()
    - Funcion heredada que dibuja el borde de ventana (decoracion).

	- Parametros:
	Component *componente: Componente que se pasa por referencia, corresponde
	con el componente de borde de ventana.
*/
void renderizar_borde(Component *componente) {
    /* 1. Dibujar con rectangle en toda la pantalla */
	setcolor(componente -> bg); 
	rectangle(0, 0, WIDTH-1, HEIGHT-1);
	rectangle(1, 1, WIDTH-2, HEIGHT-2);
}

/*
    renderizar_boton_cerrar()
    - Funcion heredada de renderizar_componente, utilizada para dibujar
	particularmente el boton de cerrar, esto incluye el componente y un icono
	de equis (X).

	- Parametros:
	Component *componente: Componente que se pasa por referencia, corresponde
	con el boton de cerrar.
*/
void renderizar_boton_cerrar(Component *componente)
{
    /* 1. Declaracion de variables */
	int centro_x, centro_y, margen;

    /* 2. Renderizar boton de cerrar */
    renderizar_componente(componente);

    /* 3. Calcular centro del boton */
	centro_x = CENTRO_X(componente);
    centro_y = CENTRO_Y(componente);

    /* 4. Definir margen que separa el icono de cerrar en dos pixeles */
    margen = 2;
    setcolor(componente -> color); /* Ajustar color de icono */

	/* 5. Dibujar icono (que corresponde a una X) */
    /* Dibujar primera linea (\) */
    line( /* Se ajusta dependiendo del margen puesto */
		centro_x - margen, /* Izquierda del centro */
        centro_y - margen, /* Arriba del centro */
        centro_x + margen, /* Derecha del centro */
        centro_y + margen  /* Abajo del centro */
    );
    /* Dibujar segunda linea (/) */
    line(
		centro_x + margen, 	/* Derecha del centro */
        centro_y - margen, 	/* Arriba del centro */
        centro_x - margen,  /* Izquierda del centro */
        centro_y + margen  	/* Abajo del centro */
	);
}

/*
    renderizar_boton_despliegue()
    - Funcion heredada de renderizar_componente, utilizada para dibujar
	especificamente el boton de despliegue, esto incluye el componente y un icono
	de tres barras horizontales puestas de forma vertical.

	- Parametros:
	Component *componente: Componente que se pasa por referencia, corresponde
	con el boton de despliegue.
*/
void renderizar_boton_despliegue(Component *componente)
{
	/* 1. Definir centro de componente y margen */
    int centro_x = CENTRO_X(componente);
    int centro_y = CENTRO_Y(componente);
    int margen = 2;  /* Usar mismo margen para X y Y */

	/* 2. Renderizar componente */
    renderizar_componente(componente);
    setcolor(componente -> color); /* Configurar color para icono */

    /* 3. Dibujar tres lineas horizontales (icono) */
    line(centro_x - margen, centro_y - margen,
		 centro_x + margen + 1, centro_y - margen);  /* Linea superior */

    line(centro_x - margen, centro_y,
		 centro_x + margen + 1, centro_y);           /* Linea central */

    line(centro_x - margen, centro_y + margen,
		 centro_x + margen + 1, centro_y + margen);  /* Linea inferior */
}

/* Seccion 3: Renderizado de barra de ventana */
/*
    renderizar_boton_despliegue()
    - Funcion dedicada para renderizar toda la barra de ventana de las aplicaciones.

	- Parametros:
	Component *barrra_de_ventana: Componente que se pasa por referencia, corresponde
	con la barra de ventana.
    Component *cerrar: Componente que se pasa por referencia, corresponde
	con el boton para cerrar.
    Component *despliegue: Componente que se pasa por referencia, corresponde
	con el boton de despliegue.
    char *titulo: Titulo de la aplicacion.
*/
void renderizar_barra_de_ventana(
    Component *barra_de_ventana, 
    Component *cerrar, 
    Component *despliegue, 
    char *titulo
) {   
    /* Llamar funciones para realizar renderizado de componentes */
    renderizar_borde(barra_de_ventana);
    renderizar_componente_texto(barra_de_ventana, titulo);
    renderizar_boton_cerrar(cerrar);
    renderizar_boton_despliegue(despliegue);
}

/* Seccion 4: Funciones de buffer de menu desplegable */
/*
    crear_buffer_desplegable()
    - Funcion que crea el buffer de la posicion puesta en el menu de despliegue.
    - Reserva para un puntero una sola vez con el fin de reutilizarse durante
    la ejecucion de la aplicacion.

    Parametros:
    Component *interfaz: Componente del menu de despliegue pasado como
    referencia.
    int **buffer: Direccion del buffer para almacenar imagen.
*/
void crear_buffer_desplegable(Component *interfaz, int **buffer)
{
    /* 1. Se define la variable que contendra el tamano del menu de despliegue
    en bytes */
    int tamano;

    /* 2. Se valida si los punteros son validos, sino, la funcion
    se detiene */
    if (interfaz == NULL || buffer == NULL) return;

    /* 3. Si ya existe el buffer, no hace nada */
    if (*buffer != NULL) return;

    /* 4. Se define el tamano de la imagen en base a las coordenadas de interfaz */
    tamano = imagesize(
        interfaz -> x1,
        interfaz -> y1,
        interfaz -> x2,
        interfaz -> y2
    );

    /* 5. Se valida si el calculo de la imagen es valida, sino 
    se valida puntero y se detiene la funcion. */
    if (tamano == -1 || tamano <= 0) { *buffer = NULL; return; }

    /* 6. Si los procesos se validaron y se ejecutaron correctamente
    se realiza la reserva de memoria al *buffer */
    *buffer = (int*) malloc(tamano);
}

/*
    guardar_buffer_desplegable()
    - Funcion que guarda la imagen que sera superpuesta por el menu de despliegue.
    - Se separo de la reserva del buffer para solo ejecutar la accion de guardado
    al buffer, con fines de reutilizacion.

    Parametros:
    Component *interfaz: Componente del menu de despliegue pasado como
    referencia.
    int **buffer: Direccion del buffer para almacenar imagen.
*/
void guardar_buffer_desplegable(Component *interfaz, int **buffer)
{
    /* 1. Se valida si los punteros son validos, sino se detiene la funcion */
    if (*buffer == NULL || buffer == NULL || interfaz == NULL) return;

    /* 2. Si la validacion anterior se ejecuta con exito se captura la
    imagen con getimage y los puntos del menu desplegable */
    getimage(
        interfaz -> x1,
        interfaz -> y1,
        interfaz -> x2,
        interfaz -> y2,
        *buffer
    );
}

/*
    liberar_buffer_desplegable()
    - Funcion que librera el buffer de la imagen superpuesta por el menu
    desplegable.

    Parametros:
    int **buffer: Direccion del buffer para almacenar imagen.
*/
void liberar_buffer_desplegable(int **buffer)
{
    /* 1. Se valida si los punteros no son nulos para realizar
    la liberacion, sino se detiene */
    if (buffer == NULL || *buffer == NULL) return;

    /* 2. Se libera la memoria con free y el dangling pointer pasa
    a ser null pointer */
    free(*buffer);
    *buffer = NULL;
}

/*
    mostrar_menu_desplegable()
    - Funcion que muestra el menu desplegable.

    Parametros:
    unsigned short n_opciones: Indica la cantidad de opciones que mostrara
    el menu desplegable.
    char **opciones_texto: Puntero a muchas cadenas, la cual son los titulos de las
    opciones que se contendran en el menu de despliegue.
    int **buffer: Direccion del buffer para almacenar imagen.

    Devuelve uno si el menu de despliegue se muestra correctamente, sino, devuelve cero.
*/
int mostrar_menu_desplegable(unsigned short n_opciones, char **opciones_texto, int **buffer)
{
    /* 1. Declarar componente de menu desplegable */
    Component interfaz_desplegable;

    /* 2. Declarar e inicializar datos adicionales */
    char salir[] = "Salir";             /* Cadena que representa la opcion para salir*/
    unsigned short int padding = 5;     /* Pixeles de distancia entre los bordes y entre las opciones */
    int alto_opcion, ancho_maximo = 0;  /* Variables de alto y ancho maximo */
    unsigned short int i;               /* Variable de indice */

    /* 3. Se valida si el puntero a las opciones de texto y el buffer 
    son ingresados correctamente, si no la funcion devuelve cero */
    if (opciones_texto == NULL || buffer == NULL) return 0;

    /* 3. Se calcula el alto de las opciones (contando tambien el padding) */
    alto_opcion = textheight(salir) + padding;

    /* 4. Se filtra el ancho maximo de las opciones */
    for (i = 0; i < n_opciones; i++) {
        /* Se declara en el ambito local una variable de guardado temporal */
        int ancho; 

        /* Se valida si las opciones de texto son validas, sino la funcion
        devuelve cero */
        if (opciones_texto[i] == NULL) return 0;

        /* Se calcula el ancho de cada opcion */
        ancho = textwidth(opciones_texto[i]);

        /* Se compara el ancho actual con el ancho maximo */
        if (ancho > ancho_maximo) {
            /* Si la condicion se cumple el ancho actual se convierte
            en el ancho maximo */
            ancho_maximo = ancho;
        }
    }

    /* 5. Ahora al ancho maximo se le toma en cuenta el padding */
    ancho_maximo += padding * 2;

    /* 6. Se construye el componente representante al menu de despliegue */
    constructor_componente(
        &interfaz_desplegable, /* Puntero a Componente*/
        12,                    /* Punto x1, y1 */
        12,                    
        12 + ancho_maximo,     /* Punto x2, y2 */
        12 + (alto_opcion * (n_opciones + 1)) + padding,
        LIGHTGRAY,             /* Colores */
        BLACK, 
        BLACK, 
        HOVER_NO_DISPONIBLE    /* Disponiblidad de hover */
    );

    /* 7. Antes de renderizar el componente del menu de despliegue ocultamos el mouse */
    mocultar();

    /* 8. Se crea el buffer de imagen solo si no existe */
    if (*buffer == NULL) {
        crear_buffer_desplegable(&interfaz_desplegable, buffer);
    }
    
    /* 9. Se guarda el area actual antes de mostrar menu */
    guardar_buffer_desplegable(&interfaz_desplegable, buffer);

    /* 10. Se renderiza el menu de despliegue (se muestra) */
    renderizar_componente(&interfaz_desplegable);

    /* 11. Se muestra cada uno de los titulos de opciones a traves de 
    un bucle y operaciones */
    setcolor(BLACK);
    for (i = 0; i < n_opciones; i++) {
        outtextxy(
            interfaz_desplegable.x1 + padding,
            interfaz_desplegable.y1 + padding + (alto_opcion * i),
            opciones_texto[i]
        );
    }

    /* 12. Se dibuja la linea separadora de la opcion "Salir" y el resto de opciones */
    if (n_opciones > 0) { /* Esta solo se mostrara si hay mas de una opcion */
        line(
            interfaz_desplegable.x1 + padding,
            interfaz_desplegable.y1 + padding + (alto_opcion * n_opciones),
            interfaz_desplegable.x2 - padding,
            interfaz_desplegable.y1 + padding + (alto_opcion * n_opciones)
        );
    }

    /* 13. Luego se muestra el titulo de opcion "Salir" */
    setcolor(BLACK);
    outtextxy(
        interfaz_desplegable.x1 + padding,
        interfaz_desplegable.y1 + padding + (alto_opcion * n_opciones),
        salir
    );

    /* 13. Se vuelve a activar el raton y se retorna que el menu de despliegue
    se mostro con exito */
    mver();
    return 1;
}

/*
    ocultar_menu_desplegable()
    - Funcion que oculta el menu desplegable.

    Parametros:
    int **buffer: Direccion del buffer para colocar imagen.

    Devuelve uno si el menu de despliegue se cerro correctamente, sino, devuelve cero.
*/
int ocultar_menu_desplegable(int **buffer)
{
    /* 1. Se valida si el puntero es valido, si no devuelve cero */
    if (buffer == NULL || *buffer == NULL) return 0;

    /* 2. Se oculta el mouse para no alterar renderizado */
    mocultar();

    /* 3. Se coloca la imagen con putimage */
    putimage(12, 12, *buffer, COPY_PUT);

    /* 4. Se activa el mouse y se devuelve uno, indicando que el menu
    se oculto correctamente */
    mver();
    return 1;
}

/* Seccion 5: Funciones de control de barra de ventana */
/*
    manejar_barra_ventana()
    - Administra la logica interna de la barra de ventana, administrando
    principalmente la logica de sus botones. Para que funcione correctamente
    debe estar englobada por una condicion de click.
    
    Parametros:
    Component *cerrar: 
        Componente asociado al boton de cerrar.
    Component *despliegue: 
        Componente asociado al boton de menu de despliegue.
    short mouse_x, short mouse_y: 
        Coordenadas del mouse en (x, y).
    unsigned char *despliegue_activo: 
        Indicacion si el menu de despliegue esta activo.
    unsigned char n_opciones:
        Cantidad de opciones que tiene el menu desplegable.
    char **opciones_desplegables:
        Matriz de cadenas que contienen los titulos de las opciones del desplegable.
    int **desplegable_buffer:
        Puntero a buffer de la zona marcada por el menu desplegable.
    
    Retorna:
    - La cantidad de opciones si se debe salir.
    - La opcion correspondiente a su indice en caso de su seleccion.
    - Fuera de los casos anteriores, retorna -1.
*/
/*
    manejar_barra_ventana()
    - Administra la logica interna de la barra de ventana, administrando
    principalmente la logica de sus botones.
    
    Parametros:
    Component *cerrar: 
        Componente asociado al boton de cerrar.
    Component *despliegue: 
        Componente asociado al boton de menu de despliegue.
    short mouse_x, short mouse_y: 
        Coordenadas del mouse en (x, y).
    unsigned char *despliegue_activo: 
        Indicacion si el menu de despliegue esta activo.
    unsigned char n_opciones:
        Cantidad de opciones que tiene el menu desplegable.
    char **opciones_desplegables:
        Matriz de cadenas que contienen los titulos de las opciones del desplegable.
    int **desplegable_buffer:
        Puntero a buffer de la zona marcada por el menu desplegable.
    
    Retorna uno si se debe salir, si no retorna cero.
*/
short manejar_barra_ventana(
    Component *cerrar,
    Component *despliegue, 
    short mouse_x, short mouse_y,
    unsigned char *despliegue_activo,
    unsigned char n_opciones,
    char **opciones_desplegables,
    int **desplegable_buffer
) {

    if (mclick() == 1)
    {
        /* 1. Manejar la logica del boton para cerrar */
        if (mouse_sobre_componente(cerrar, mouse_x, mouse_y)) return n_opciones;
        
        /* 2. Alternar estados del boton de menu de despliegue */
        if (mouse_sobre_componente(despliegue, mouse_x, mouse_y)) 
        {
            /* Se detecta click en el boton y se verifica si no esta activo */
            if (!*despliegue_activo) {
                /* Si es el caso se muestra el menu desplegable */
                mostrar_menu_desplegable(n_opciones, opciones_desplegables, desplegable_buffer);
                *despliegue_activo = 1; /* Y el estado cambia */
            } else {
                /* Si esta activo y se presiona se oculta el menu desplegable */
                ocultar_menu_desplegable(desplegable_buffer);
                *despliegue_activo = 0; /* Y el estado cambia xd */
            }

            return -1;
        }
    }
    
    /* 3. Se detecta que opcion se selecciono del menu desplegable */
    if (*despliegue_activo && mclick()) { /* Solo aplica si el menu desplegable esta activo y se hizo click */
        /* A traves de esta funcion y las coordenadas iniciales del menu de despliegue
        se realiza la seleccion de la opcion elegida en el menu de despliegue */
        short opcion_seleccionada = detectar_opcion_menu(12, 12, mouse_x, mouse_y, n_opciones, opciones_desplegables);
        
        /* Se eligio o no una opcion, despues de dar un click el menu siempre se oculta */
        ocultar_menu_desplegable(desplegable_buffer);
        *despliegue_activo = 0; /* Y ahora el despliegue no esta activo */
        
        /* Caso: se eligio alguna de las opciones */
        if (opcion_seleccionada != -1) return opcion_seleccionada;
    }
    
    /* Caso: no se eligio alguna de las opciones */
    return -1;
}

/*
    detectar_opcion_menu()
    - Funcion que detecta alguna opcion elegida desde el menu desplegable.

    Parametros:
    short xi, short yi:
        Coordenadas iniciales del menu de despliegue.
    short mouse_x, short mouse_y:
        Coordenadas de la posicion del mouse.
    int n_opciones:
        Cantidad de opciones del menu desplegable.
    char **opciones_desplegables:
        Titulos de las opciones desplegables (a traves de una matriz/doble puntero).

    Devuelve segun los siguientes casos:
        - Si no se toco alguna de las opciones: -1.
        - Si se toca alguna de las funciones:
            Un numero entre el cero y n_opciones - 1.
        - Si se toca el boton salir: n_opciones.
*/
short detectar_opcion_menu(
    short xi, short yi, 
    short mouse_x, short mouse_y, 
    int n_opciones, 
    char **opciones_desplegables
) {
    /* 1. Definir variables importantes para calculos aritmeticos */
    /* Distancias para calcular los espacios de opciones */
    short ancho_maximo = textwidth("Salir");      /* Ancho maximo */
    short alto_opcion = textheight("Salir") + 5;  /* Alto + Padding */

    /* Se aplica padding para el punto inicial */
    short y = yi + 5;

    /* Variables miscelaneas */
    short i; /* Variable de indice XD */
    short xf, y_salir; /* Variables de fin de coordenada */
    
    /* 2. Encontrar el ancho maximo del menu */
    for (i = 0; i < n_opciones; i++) {
        /* Se contrasta cada opcion respecto a su ancho */
        int ancho = textwidth(opciones_desplegables[i]);

        /* Si la condicion se cumple se obtendra un nuevo ancho */
        if (ancho > ancho_maximo) ancho_maximo = ancho;
    }
    
    /* 3. Se obtiene la coordenada en X del punto final del menu de despliegue */
    xf = xi + ancho_maximo + (5*2);
    
    /* 4. Verificar opciones normales */
    for (i = 0; i < n_opciones; i++) {
        /* Se trabaja opcion por opcion */
        short y_opcion = y + (alto_opcion * i);

        /* Si se detecta que el raton pulso en esa opcion se devuelve
        su iteracion correspondiente */
        if (mouse_y >= y_opcion && mouse_y < y_opcion + alto_opcion 
            && mouse_x >= xi && mouse_x <= xf)
            return i;
    }
    
    /* 5. Verificar opcion "Salir" (en caso de no haber pulsado las opciones anteriores) */
    /* Se obtiene la posicion en Y minima de la opcion salir */
    y_salir = y + (alto_opcion * n_opciones);

    /* Si se detecta que se pulso la opcion salir se devuelve el indice de esa opcion */
    if (mouse_y >= y_salir && mouse_y < y_salir + alto_opcion 
        && mouse_x >= xi && mouse_x <= xf) 
        return n_opciones;
    
    return -1; /* Se devuelve -1 si no se selecciono nada */
}
#endif
