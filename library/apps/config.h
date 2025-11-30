#ifndef CONFIG_H
#define CONFIG_H

/*
	CONFIG_H
    Libreria que maneja la aplicacion de configuraciones, las cuales
    cambia propiedades basicas del sistema operativo. Esta cambia
    el tema y protector de pantalla seleccionado, teniendo como sus
    primeras opciones las que ya estan por defecto.

    Este programa se divide en la barra de tareas gestionada por
    appadmin.h, una seccion de opciones a la izquierda la cual
    se puede alternar para cambiar o un tema o un protector, y
    producto de ello a la derecha se muestran las opciones de tema
    o protectores de pantalla.

    - Gabriela Ruiz
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
    Declaracion de Directivas
    =========================================
*/
#define TEMAS_CANTIDAD 5        /* Cantidad de opciones de los temas */
#define PROTECTORES_CANTIDAD 3  /* Cantidad de opciones de los protectores */

/*
    =======================================
    Definicion de Variables Globales
    =========================================
*/
/* Arreglo que contiene la paleta de temas a mostrar como opciones en la seccion de
temas */
static short temas_colores[TEMAS_CANTIDAD] = {29, 109, 39, 49, 54};

/* Titulos correspondiente con las opciones de temas */
static char *temas_opciones_titulos[TEMAS_CANTIDAD] = {
    "Vanilla", "Morado", "Rojo", "Verde", "Azul"
};

/* Titulos pertenecientes a las opciones de protectores de pantallas */
static char *protectores_opciones_titulos[PROTECTORES_CANTIDAD] = {
    "Logo NeoRetro OS",
    "Carrusel de Imagenes",
    "Lienzo de Figuras"
};

/*
    =======================================
    Definicion de Enums
    =========================================
*/
/*
    Enum ConfigMode
    Estructura enum correspondiente con el modo actual de las opciones
    de configuracion.
*/
typedef enum {
    MODO_TEMAS,
    MODO_PROTECTORES
} ConfigMode;

/*
    =======================================
    Declaracion de Prototipos
    =========================================
*/
/* Seccion 1: Inicializacion de menus de opciones */
void inicializar_componentes_temas(Component tema_opciones[]);
void inicializar_componentes_protectores(Component protector_opciones[]);

/* Seccion 2: Renderizado de menus de opciones */
void renderizar_configuraciones_temas(Component tema_opciones[]);
void renderizar_configuraciones_protectores(Component protector_opciones[]);
void renderizar_modo_actual(ConfigMode modo, Component tema_opciones[], Component protector_opciones[]);

/* Seccion 3: Limpieza de areas */
void limpiar_area_configuraciones(void);

/* Seccion 4: Guardado/cargado de fondos */
void guardar_fondo(int fondo);
int cargar_fondo(void);
void renderizar_fondo_de_pantalla(char *nombre_bin, short tipo);

/* Seccion 5: Guardado/cargado protectores de pantalla */
void guardar_protector(int protector);
int cargar_protector(void);
void colocar_protector_pantalla(void);

/* Seccion 6: Manejo de logica de app (principal) */
void app_configuraciones(void);

/*
    =======================================
    Inicializacion de Funciones
    =========================================
*/

/* Seccion 1: Inicializacion de menus de opciones */
/*
    inicializar_componentes_temas();
    Funcion que construye los componentes necesarios para el menu de opciones
    de temas.

    Parametros:
    - Component tema_opciones[]: Arreglo de componentes relacionado con las opciones
    de temas del sistema operativo.
*/
void inicializar_componentes_temas(Component tema_opciones[])
{
    /* 1. Se declara una variable de recorrido */
    short i;
    
    /* 2. Se recorre cada uno de los componentes de tema_opciones[] */
    for (i = 0; i < TEMAS_CANTIDAD; i++)
    {
        /* 3. Se construye el componente como opcion de tema */
        constructor_componente(
            &tema_opciones[i],
            116, 40 + 27 * i,
            140, 64 + 27 * i,
            temas_colores[i],
            BLACK, BLACK,
            HOVER_DISPONIBLE
        );

        /* 4. Luego se construye su hover */
        constructor_hover_componente(
            &tema_opciones[i],
            temas_colores[i],
            BLACK, 102
        );
    }
}

/*
    inicializar_componentes_protectores();
    Funcion que construye los componentes necesarios para el menu de opciones
    de protectores de pantalla.

    Parametros:
    - Component protector_opciones[]: Arreglo de componentes relacionado con las opciones
    de protectores de pantalla del sistema operativo.
*/
void inicializar_componentes_protectores(Component protector_opciones[])
{
    /* 1. Se declara una variable de recorrido */
    short i;
    
    /* 2. Se crea cada uno de los componentes de opciones de protectores de pantalla */
    for (i = 0; i < PROTECTORES_CANTIDAD; i++)
    {
        /* 3. Se inicializa el componente de opcion */
        constructor_componente(
            &protector_opciones[i],
            116, 37 + 47 * i,
            160, 81 + 47 * i,
            BLACK, BLACK, BLACK,
            HOVER_DISPONIBLE
        );

        /* 4. Y se inicializa su hover */
        constructor_hover_componente(
            &protector_opciones[i],
            102, BLACK, 102
        );
    }
}

/* Seccion 2: Renderizado de menus de opciones */
/*
    renderizar_configuraciones_temas();
    Funcion que dibuja la interfaz de opciones para los temas del sistema operativo.

    Parametros:
    Component tema_opciones[]: Arreglo de componentes relacionado con los componentes de 
    opciones para los temas del sistema operativo.
*/
void renderizar_configuraciones_temas(Component tema_opciones[])
{
    /* 1. Se declaran las variables a utilizar */
    short i;                               /* Variable de indice */
    char temas_titulo[] = "Cambiar tema:"; /* Titulo de opciones de tema */
    
    /* 2. Se ajustan las configuraciones del texto */
    colocar_configuraciones();
    
    /* 3. Se oculta el raton para evitar problemas de dibujado */
    mocultar();

    /* 4. Se muestra el encabezado de las opciones de tema (temas_titulo) */
    outtextxy(110 + (210 - textwidth(temas_titulo)) / 2, 20, temas_titulo);
    
    /* 5. Se dibuja cada una de las opciones de tema */
    for (i = 0; i < TEMAS_CANTIDAD; i++)
    {
        /* Se renderiza el compomente como opcion de tema */
        renderizar_componente(&tema_opciones[i]);
        /* Y un texto adjunto para guia visual */
        outtextxy(150, (40 + 27 * i) + (24 - textheight(temas_opciones_titulos[i])) / 2, temas_opciones_titulos[i]);
    }

    /* 6. Finalmente el raton vuelve a ser visible */
    mver();
}

/*
    renderizar_configuraciones_temas();
    Funcion que dibuja la interfaz de opciones para los temas del sistema operativo.

    Parametros:
    Component tema_opciones[]: Arreglo de componentes relacionado con los componentes de 
    opciones para los temas del sistema operativo.
*/
void renderizar_configuraciones_protectores(Component protector_opciones[])
{
    /* 1. Se declaran las variables a utilizar */
    short i;                                                        /* Variable de indice */
    char protector_titulo[] = "Cambiar protector de pantalla:";     /* Titulo de opciones de protector */
    
    /* 2. Se ajustan las configuraciones de texto */
    colocar_configuraciones();
    
    /* 3. Se oculta el raton para evitar problemas de dibujado */
    mocultar();

    /* 4. Se muestra el titulo de opciones de protector */
    outtextxy(110 + (210 - textwidth(protector_titulo)) / 2, 20, protector_titulo);
    
    /* 5. Se dibuja cada uno de las opciones/componentes de protector */
    for (i = 0; i < PROTECTORES_CANTIDAD; i++)
    {
        /* Se renderiza cada uno */
        renderizar_componente(&protector_opciones[i]);

        /* Y se muestra adjunto un texto como apoyo visual */
        outtextxy(165, (37 + 47 * i) + (44 - textheight(protectores_opciones_titulos[i])) / 2, protectores_opciones_titulos[i]);
    }

    /* 6. Finalmente se vuelve a mostrar el raton */
    mver();
}

/*
    renderizar_modo_actual();
    Funcion que renderiza el menu seleccionado, cambiandolo dependiendo de la
    opcion del usuario.

    Parametros:
    ConfigMode modo: Constante del menu seleccionado.
    Component tema_opciones[]: Arreglo de componentes relacionado con los componentes de 
    opciones para los temas del sistema operativo.
    Component protector_opciones[]: Arreglo de componentes relacionado con los componentes de
    opciones para los protectores de pantalla del sistema operativo.
*/
void renderizar_modo_actual(ConfigMode modo, Component tema_opciones[], Component protector_opciones[])
{
    /* 1. Antes se limpia el area la cual se dibuja el menu seleccionado */
    limpiar_area_configuraciones();
    
    /* 2. A traves de esta seleccion se renderiza el modo seleccionado */
    switch(modo)
    {
        /* Caso: Se renderiza el menu de opciones de temas */
        case MODO_TEMAS: 
			renderizar_configuraciones_temas(tema_opciones);
            break;
        /* Caso: Se renderiza el menu de opciones de protectores */
        case MODO_PROTECTORES:
			renderizar_configuraciones_protectores(protector_opciones);
            break;
        }
}

/* Seccion 3: Limpieza de areas */
/*
    limpiar_area_configuraciones();
    Funcion que limpia el area utilizada para la interfaz de configuraciones.
*/
void limpiar_area_configuraciones(void)
{
    setfillstyle(SOLID_FILL, 30);
    bar(111, 12, WIDTH, HEIGHT);
}

/* Seccion 4: Guardado/cargado de fondos y protectores de pantalla */
/*
    cargar_fondo();
    Funcion que a traves de un fichero guardado con guardar_fondo
    carga el fondo al cual se quiere aplicar el cambio. Devuelve
    el indice del fondo que se selecciono en las opciones del menu.
*/
int cargar_fondo(void)
{
    /* 1. Se carga la configuracion del fondo de pantalla aplicado
    a traves de fopen y FILE */
    FILE *f = fopen("C:\\TC20\\Ficheros\\config.dat", "rb");
    int fondo = 1;    /* Se declara una variable por defecto del fondo */

    /* 2. Si el fichero no carga correctamente se devuelve el fondo por
    defecto */
    if (f == NULL) return 1;

    /* 3. Se carga el fondo de pantalla con fread y se cierra el fichero */
    fread(&fondo, sizeof(int), 1, f); fclose(f);
    return fondo;   /* Al final el fondo cargado es el que se terminara devolviendo */
}

/*
    guardar_fondo();
    Se encarga del guardado de la ultima configuracion realizada por el usuario,
    recibiendo como parametro el fondo seleccionado para guardarlo en un fichero .dat,
    que corresponde desde el 1 hasta el 5.
*/
void guardar_fondo(int fondo)
{
    /* 1. Se declara el apuntador a FILE para posteriormente crear (y truncar) el fichero */
    FILE *f = fopen("C:\\TC20\\Ficheros\\config.dat", "wb");
    if (f == NULL) return;  /* Se valida para evitar conflictos */
    
    /* 2. Se escribe en el fichero el fondo actual para ser cargado cuando
    se requiera */
    fwrite(&fondo, sizeof(int), 1, f);
    fclose(f);  /* Una vez escrito se cierra */
}

/*
    renderizar_fondo_de_pantalla();
    Funcion que renderiza el fondo de pantalla segun el fondo seleccionado
    actualmente, para ello se trabaja con las funciones anteriormente
    mencionadas en desktop.h, para poder manejar el fondo seleccionado.

    char *nombre_bin: Cadena que corresponde con el fichero .bin la
    cual es el nombre del fondo a pantalla a renderizar.
    short tipo: Corresponde a que tipo de fondo se renderizara, siendo
    los siguientes:
    - 1/(200x200) = Corresponde con el fondo de pantalla por defecto, la
    cual trabaja con una resolucion de 200x200 pixeles.
    - 2/(320x200) = Corresponde con el resto de fondos que encajan perfectamente
    en pantalla.
*/
void renderizar_fondo_de_pantalla(char *nombre_bin, short tipo)
{
    /* 1. Se declara un buffer de fichero y se abre el fichero del fondo como lectura binaria */
    FILE *fondo = fopen(nombre_bin, "rb");
    if (fondo == NULL) return; /* Tambien se verifica si se abrio correctamente */
    
    /* 2. Se dibuja el fondo con rasterizado */
    switch (tipo)
    {
        case 1:  /* Caso: Si se decide rasterizar el primer fondo */
            dibujar_con_rasterizado_pos(fondo, CENTER, 200, 200, WIDTH, HEIGHT);
            break;
        case 2:  /* Caso: Se decide rasterizar otro fondo */
            dibujar_con_rasterizado_pos(fondo, TOP_LEFT, 320, 200, WIDTH, HEIGHT);
            break;
        default: /* Defecto: Valida en todo caso la opcion seleccionada */
            dibujar_con_rasterizado_pos(fondo, CENTER, 200, 200, WIDTH, HEIGHT);
    }
    fclose(fondo); /* Se cierra el fichero de fondo al renderizarse */
}

/* Seccion 5: Guardado/cargado protectores de pantalla */
/*
    guardar_protector();
    Funcion que guarda el protector de pantalla seleccionado en las 
    configuraciones a traves de un fichero .dat.

    Parametros:
    - int protector: Corresponde con el protector seleccionado
    en las configuraciones (desde 1 al 3).
*/
void guardar_protector(int protector)
{
    /* 1. Se declara el apuntador a FILE para posteriormente crear (y truncar) el fichero */
    FILE *f = fopen("C:\\TC20\\Ficheros\\protect.dat", "wb");
    if (f == NULL) return;  /* Se valida para evitar conflictos */
    
    /* 2. Se escribe en el fichero el fondo actual para ser cargado cuando
    se requiera */
    fwrite(&protector, sizeof(int), 1, f);
    fclose(f);  /* Una vez escrito se cierra */
}

/*
    cargar_protector();
    Funcion que carga el protector de pantalla guardado en el fichero
    que contiene el protector de pantalla guardado. Devuelve el protector
    de pantalla cargado.
*/
int cargar_protector(void)
{
    /* 1. Se carga el protector de pantalla seleccionado gracias
    a fopen */
    FILE *f = fopen("C:\\TC20\\Ficheros\\protect.dat", "rb");
    int protector = 1;    /* Se declara una variable por defecto del protector */

    /* 2. Si el fichero no carga correctamente se devuelve el protector por defecto */
    if (f == NULL) return 1;

    /* 3. Se carga el protector de pantalla con fread y se cierra el fichero */
    fread(&protector, sizeof(int), 1, f); fclose(f);
    return protector;   /* Y protector cargado se devuelve */
}

/*
    colocar_protector_pantalla();
    Esta funcion permite cargar y ejecutar el protector de pantalla a traves
    de las funciones anteriores. A diferencia del fondo, esta no necesita
    depender de otra funcion mas que solo de guardado y cargado, para simplemente
    ejecutar las funciones de protectores de pantalla a traves de un switch.
*/
void colocar_protector_pantalla(void)
{
    /* 1. Se declara una variable conteniendo el protector guardado */
    int protector_actual = cargar_protector();

    /* 2. A traves del switch se selecciona el protector seleccionado
    para simplemente ejecutarse */
    switch (protector_actual)
    {
        case 1:  protector_1(); break;
        case 2:  protector_2(); break;
        case 3:  protector_3(); break;
        default: protector_1(); break;
    }
}

/* Seccion 6: Manejo de logica de app (principal) */
/*
    app_configuraciones();
    Funcion principal de la apps de configuraciones que maneja su logica interna.
*/
void app_configuraciones(void) 
{
    /* 1. Declaracion de componentes y variables */
    /* Componentes de barra de ventana */
    Component barra_de_ventana, cerrar;

    /* Componentes de secciones (las opciones que aparecen a la izquierda) */
    Component tema_seccion, protector_seccion, opciones_seccion;

    /* Componentes de opciones para temas y protectores de pantalla */
    Component tema_opciones[TEMAS_CANTIDAD], protector_opciones[PROTECTORES_CANTIDAD];

    /* Variables de cambio de modos */
    ConfigMode modo = MODO_TEMAS;
    ConfigMode modo_anterior = MODO_TEMAS;

    /* Indice y posiciones del mouse */
    short i = 0;
    short mouse_x, mouse_y;
    
    /* 2. Limpiar y ajustar configuraciones por defecto */
    colocar_configuraciones();
    set_bg(30);
    
    /* 3. Incializar componentes esenciales */
    /* Inicializar barra de ventana */
    constructor_componente(&barra_de_ventana, 0, 0, WIDTH, 11, BLACK, WHITE, BLACK, HOVER_NO_DISPONIBLE);
    constructor_componente(&cerrar, 0, 0, 11, 11, BLACK, WHITE, 40, HOVER_DISPONIBLE);
    constructor_hover_componente(&cerrar, 40, WHITE, 40);
    
    /* Inicializar componentes de la seccion de la izquierda */
    constructor_componente(&opciones_seccion, 0, 12, 110, HEIGHT, BLACK, BLACK, BLACK, HOVER_NO_DISPONIBLE);
    constructor_componente(&tema_seccion, 0, 41, 110, 101, BLACK, WHITE, BLACK, HOVER_DISPONIBLE);
    constructor_hover_componente(&tema_seccion, WHITE, BLUE, BLUE);
    constructor_componente(&protector_seccion, 0, 111, 110, 171, BLACK, WHITE, BLACK, HOVER_DISPONIBLE);
    constructor_hover_componente(&protector_seccion, WHITE, BLUE, BLUE);
    
    /* Inicializar opciones de temas y protectores */
    inicializar_componentes_temas(tema_opciones);
    inicializar_componentes_protectores(protector_opciones);
    
    /* Renderizar componentes principales */
    renderizar_borde(&barra_de_ventana);
    renderizar_componente_texto(&barra_de_ventana, "Configuraciones");
    renderizar_boton_cerrar(&cerrar);

    renderizar_componente(&opciones_seccion);
    renderizar_componente_texto(&tema_seccion, "Temas");
    renderizar_componente_texto(&protector_seccion, "Protectores de Pantalla");

    /* Renderizar modo inicial */
    renderizar_modo_actual(modo, tema_opciones, protector_opciones);
    mver(); /* Activar raton */
    
    /* 4. Manejar los clicks de componentes a traves del do while */
    do {
        /* 5. Verificar si cambio el modo para redibujar */
        if (modo != modo_anterior) {
            renderizar_modo_actual(modo, tema_opciones, protector_opciones);
            modo_anterior = modo;
        }

        /* 6. Capturar posicion del mouse */
        mouse_x = mxpos();
        mouse_y = mypos();
        
        /* 7. Manejar hovers comunes */
        manejar_hover_componente_texto(&tema_seccion, mouse_x, mouse_y, "Temas");
        manejar_hover_componente_texto(&protector_seccion, mouse_x, mouse_y, "Protectores de Pantalla");
        manejar_hover_componente(&cerrar, mouse_x, mouse_y, renderizar_boton_cerrar);
        
        /* 8. Manejar hovers especificos del modo actual */
        if (modo == MODO_TEMAS) {
            for (i = 0; i < TEMAS_CANTIDAD; i++)
                manejar_hover_componente(&tema_opciones[i], mouse_x, mouse_y, NULL);
        } else {
            for (i = 0; i < PROTECTORES_CANTIDAD; i++)
                manejar_hover_componente(&protector_opciones[i], mouse_x, mouse_y, NULL);
        }
        
        /* 9. Manejar clicks */
        if (mclick() == 1)
        {
            /* Callback: Boton para cerrar app */
            if (mouse_sobre_componente(&cerrar, mouse_x, mouse_y))
                break;
            
            /* Callbacks: Secciones de Configuraciones */
            if (mouse_sobre_componente(&tema_seccion, mouse_x, mouse_y))
                modo = MODO_TEMAS; /* Caso: seccion de temas */
            else if (mouse_sobre_componente(&protector_seccion, mouse_x, mouse_y))
                modo = MODO_PROTECTORES; /* Caso: seccion de protectores */

            if (modo == MODO_TEMAS)
            {
                /* Callbacks: Opciones de fondo de pantalla */
                for (i = 0; i < TEMAS_CANTIDAD; i++)
                {
                    /* Se evalua cada componente de opcion de fondo de pantalla */
                    if (mouse_sobre_componente(&tema_opciones[i], mouse_x, mouse_y))
                        guardar_fondo(i + 1);
                }
            } else {
                /* Callbacks: Opciones de protectores de pantalla */
                for (i = 0; i < PROTECTORES_CANTIDAD; i++)
                {
                    /* Se evalua cada componente de opcion de protector de pantalla */
                    if (mouse_sobre_componente(&protector_opciones[i], mouse_x, mouse_y))
                        guardar_protector(i + 1);
                }
            }
        }
        delay(100); /* Retraso para evitar saturacion */
    } while (1); /* Se ejecuta hasta que se decida salir */
}
#endif