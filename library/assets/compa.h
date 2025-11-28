#ifndef COMPONENTS_H
#define COMPONENTS_H

/*
	COMPONENTS_H
	Libreria encargada de trabajar con una filosofia ideal para botones,
    cuadros de texto, secciones asi como estilo HTML, pero aplicadas en lenguaje
    C, esta se llamada "Componentes", que a traves de solo funciones puedes
    renderizar (es decir, dibujar) botones y agregarles tu propia logica de forma
    muy abstracta, pero para esto debes que tener un plantel agregado en tu main o
    funcion principal, ahorrando dolores de cabeza con las posiciones XD.

	Para usarla se debe iniciar en el main o en la funcion principal de una libreria
	la estructura Component, definir sus caracteristicas y luego pasar por referencia
	esa estructura y con ello realizar las diferentes operaciones que ofrece.

	- Memoria Dinamica (mucho cuidado con esta seccion)
	- Estructuras y Enum
	- Funciones de Infraestructura (no necesario usar)
	- Funciones de abstraccion (las funciones que siempre usaras para crear componentes)
	
	- Samuel Rueda
*/

/*
    =======================================
	Declaracion de Directivas
    =========================================
*/
#define CENTRO_X(COMP) ((COMP)->x1 + ((COMP)->x2 - (COMP)->x1) / 2) /* Centrar coordenadas de estructura COMP en X */
#define CENTRO_Y(COMP) ((COMP)->y1 + ((COMP)->y2 - (COMP)->y1) / 2) /* Centrar coordenadas de estructura COMP en Y */

/*
    =======================================
    Definicion de Estructuras / Uniones
    =========================================
*/

/*
	Estructura HoverStyle
	- Encargada de definir las propiedades de hover de un componente (concepto abordado en la
    siguiente estructura), la cual solo cambia las propiedades y evalua su disponibilidad y 
    estado.
 */
typedef struct
{
	unsigned char disponible;	/* Hover disponible o no */
	unsigned char bg;		    /* Color del fondo del componente */
	unsigned char color;    	/* Color de la letra */
	unsigned char border;	    /* Color del borde del componente */
    unsigned char estado;	    /* Estado actual del hover */
} HoverStyle;

/* 
	Estructura Component
	- En pocas palabras, el corazon de la libreria, encargada de almacenar los
	datos de un componentes para luego aplicarlos al modo grafico. Utiliza HoverStyle
    para manejar estados de hover.
 */
typedef struct
{
	short x1, y1;			/* Esquina superior izquierda de componente */
	short x2, y2;			/* Esquina inferior derecha de componente */
	unsigned char bg;		/* Color del fondo del componente */
	unsigned char color;	/* Color de la letra */
	unsigned char border;	/* Color del borde del componente */
    HoverStyle hover;	    /* Datos de hover (reaccion) */
} Component;

/*
    =======================================
    Definicion de Enum
    =========================================
*/

/* 
    Enum HoverAvailable
    - Estructura enum para evaluar la disponiblidad del hover.
 */
typedef enum
{
    HOVER_NO_DISPONIBLE,
    HOVER_DISPONIBLE
} HoverAvailable;

/* 
	Enum HoverStatus
	- Estructura enum para evaluar estado de hover.
 */
typedef enum
{
    HOVER_NINGUNO,
    HOVER_ACTIVO
} HoverStatus;

/*
    =======================================
    Declaracion de Prototipos
    =========================================
*/

/* Seccion 1: Constructores de componentes */
void constructor_componente(
	Component *componente,
	short x1, short y1, 		
	short x2, short y2, 		
	unsigned char bg,		
	unsigned char color,
	unsigned char border,
	unsigned char disponible
);
void constructor_hover_componente(
    Component *componente,
	unsigned char bg,
	unsigned char color,
	unsigned char border
);

/* Seccion 2: Funciones de renderizado (dibujo) */
void renderizar_componente(Component *boton);
void renderizar_componente_texto(Component *componente, char *texto);
/* Nota: si quiere agregar un componente plantilla por favor comunicarlo al autor */

/* Seccion 3: Manejo de hover de componentes */
void renderizar_componente_hover(Component *componente, void (*renderizar_especial)(Component*));
void manejar_hover_componente(
    Component *componente,
    short mouse_x,
    short mouse_y,
    void (*renderizar_componente_especial)(Component*)
);
void manejar_hover_componente_texto(
    Component *componente,
    short mouse_x,
    short mouse_y,
    char *texto
);

/* Seccion 4. Funciones para manejo de logica interna */
short mouse_sobre_componente(Component *componente, short mouse_x, short mouse_y);
short detectar_click_componente(Component *componente, short mouse_x, short mouse_y);
void ajustar_texto_largo_componente(Component *componente, char *texto, short *texto_x, short *texto_y);
#endif