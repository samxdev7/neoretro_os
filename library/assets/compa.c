
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

#include "dynamic.h"	/* Libreria para manejar funciones de memoria dinamica */
#include "mouse256.h"   /* Modificacion de la libreria Mouse.h, utilizada para trabajar con el mouse a 320x200 */
#include "gphadmin.h"   /* Libreria que administra el modo grafico del sistema operativo */
#include "compa.h"      /* Libreria de componentes (prueba) */

/*
    =======================================
    Inicializacion de Funciones
    =========================================
*/

/* Seccion 1: Constructores de componentes */
/*
    constructor_componente()
    - Funcion que inicializa un componente, es decir, define sus caracteristicas.

	- Parametros:
	Component *componente: Componente que se inicializara (pasada por referencia).
	int x1, int y1: esquina superior izquierda de componente (x1, y1).
	int x2, int y2:	esquina inferior derecha de componente (x2, y2).
	unsigned char bg: color del fondo del componente.
	unsigned char color: color de la letra.
	unsigned char border: color del borde del componente.
    unsigned char disponible: indica si el hover del componente esta disponible.
*/
void constructor_componente(
	Component *componente,
	short x1, short y1, 		
	short x2, short y2, 		
	unsigned char bg,		
	unsigned char color,
	unsigned char border,
	unsigned char disponible
) {
	/* Asignar directamente caracteristicas del componente */
	componente -> x1 = x1; /* Coordenadas */
    componente -> y1 = y1;
    componente -> x2 = x2;
	componente -> y2 = y2;
    componente -> bg = bg; /* Colores */
    componente -> color = color;
    componente -> border = border;
    componente -> hover.disponible = disponible; /* Hover disponible */
}

/*
    constructor_hover_componente()
    - Funcion que inicializa el hover de un componente, pero para ello debe estar
    disponible.

	- Parametros:
	Component *componente: Componente la cual se inicializara 
    se inicializara su hover (pasada por referencia).
	unsigned char bg: color del fondo de hover del componente.
	unsigned char color: color de la letra de hover del componente.
	unsigned char border: color del borde de hover del componente.
*/
void constructor_hover_componente(
    Component *componente,
	unsigned char bg,
	unsigned char color,
	unsigned char border
) {
    /* 1. Verificar si el hover esta disponible */
    if (componente->hover.disponible == HOVER_DISPONIBLE)
    {
        /* 2. Asignar directamente caracteristicas del componente */
        componente -> hover.bg = bg; /* Colores (HOVER) */
        componente -> hover.color = color;
        componente -> hover.border = border;
        /* Estado actual (corresponde a ninguno por ser un hover inicializado) */
        componente -> hover.estado = HOVER_NINGUNO; 
    }
}

/* Seccion 2: Funciones de renderizado */
/*
    renderizar_componente()
    - Funcion que dibuja un componente con sus caracteristicas a traves de una estructura
	que se pasa como estructura.
    - Obtiene el color de fondo, las coordenadas (porque es un rectangulo) y el color de borde.
    - A traves de esas caracteristicas se realiza operaciones con las funciones de graphics.h
	para dibujar este componente.

	- Parametros:
	Component *componente: Componente que se pasa por referencia y de la cual se extraen los datos.
*/
void renderizar_componente(Component *componente)
{
    /* 1. Dibujar el relleno con bar */
    setfillstyle(SOLID_FILL, componente->bg); /* Definir color del fondo */
    bar(
		componente -> x1, /* Esquina superior izquierda en X */
		componente -> y1, /* Esquina superior izquierda en Y */
		componente -> x2, /* Esquina inferior derecha en X */
		componente -> y2  /* Esquina inferior derecha en Y */
	);
    
    /* 2. Dibujar el borde con rectangle */
    setcolor(componente -> border); /* Definir color del borde */
    rectangle(
		componente -> x1, /* Esquina superior izquierda en X */
		componente -> y1, /* Esquina superior izquierda en Y */
		componente -> x2, /* Esquina inferior derecha en X */
		componente -> y2  /* Esquina inferior derecha en Y */
	);
}

/*
    renderizar_componente_texto()
    - Funcion heredada que dibuja un componente adjunto a un texto
	interno.

	- Parametros:
	Component *componente: Componente que se pasa por referencia, corresponde
	con el componente de borde de ventana.
	char *texto: Cadena de texto la cual contendra el componente.
*/
void renderizar_componente_texto(Component *componente, char *texto) 
{
    /* 1. Declaracion de variables */
	short texto_x, texto_y;
    
    /* 2. Renderizar el componente primero */
    renderizar_componente(componente);
    
    /* 3. Calcular posicion centrada */
	/* Formula: Centro en X - Longitud de Texto X / 2 = Posicion para Texto en Horizontal */
    texto_x = CENTRO_X(componente) - textwidth(texto) / 2;
	/* Formula: Centro en Y - Longitud de Texto Y / 2 - 2 = Posicion para Texto en Vertical 
	(tiene spacing arriba, por eso se le resta 2) */
	texto_y = CENTRO_Y(componente) - textheight(texto) / 2 - 2;
    
    /* 4. Renderizar texto */
    setcolor(componente -> color);
    
    /* 5. Si el texto es muy largo, dividir en dos lineas */
    if (textwidth(texto) >= componente->x2 - componente->x1)
        ajustar_texto_largo_componente(componente, texto, &texto_x, &texto_y);
    else /* Sino es largo, imprimir el texto tal y como es XD */
	    outtextxy(texto_x, texto_y, texto);
}

/* Seccion 3: Manejo de hover de componentes */
/*
    renderizar_componente_hover()
    - Funcion que renderiza un componente con los colores de hover a traves de un
    renderizado temporal con colores diferentes.

    - Parametros:
    Component *componente: Componente a renderizar con hover
    void (*renderizar_especial)(Component*): Funci�n de renderizado especial (opcional, debe pasarse
    el puntero de la funcion)
*/
void renderizar_componente_hover(Component *componente, void (*renderizar_especial)(Component*))
{
    /* 1. Se guardan los colores originales a traves de un componente temporal */
    Component temp = *componente;

    temp.bg = componente->bg;
    temp.color = componente->color;
    temp.border = componente->border;
    
    /* 2. Se aplican temporalmente los colores de hover */
    componente->bg = componente->hover.bg;
    componente->color = componente->hover.color;
    componente->border = componente->hover.border;
    
    /* 3. Renderizar con colores hover */
    if (renderizar_especial != NULL) { /* Caso: El renderizado es especial */
        renderizar_especial(componente);
    } else { /* Caso: se renderizara con la funcion base renderizar_componente */
        renderizar_componente(componente);
    }
    
    /* 4. Restaurar colores originales) */
    componente->bg = temp.bg;
    componente->color = temp.color;
    componente->border = temp.border;
}

/*
    manejar_hover_componente()
    - Funcion general para manejar el hover de cualquier componente.
    - Puede manejar componentes normales o componentes con renderizado especial.
    - Mantiene la filosofia de que esta funcion maneja individualmente el hover de 
    un componente, no el de multiples componentes.

    - Parametros:
    Component *componente: Componente que se evaluara para hover
    int mouse_x, int mouse_y: Posicion actual del mouse
    void (*renderizar_componente_especial)(Component*): Funcion de renderizado especial
    (NULL para renderizado normal)
*/
void manejar_hover_componente(
    Component *componente,
    short mouse_x,
    short mouse_y,
    void (*renderizar_componente_especial)(Component*)
) {
    /* 1. Se declaran variables de Enum HoverStatus para evaluar los hovers manejados
    por la interfaz */
    HoverStatus hover_actual;   /* Corresponde con el hover actual */
    HoverStatus hover_anterior; /* Corresponde con el hover anterior */

    /* 2. Verificar si el hover est� disponible, sino, detiene la funcion */
    if (componente -> hover.disponible != HOVER_DISPONIBLE) return;
    
    /* 3. Determinar estado actual del hover a traves de un operador ternario */
    /* Esta variable obtendra un resultado booleano a traves de la funcion 
    mouse_sobre_componente, que permite evaluar si el raton esta encima de un componente,
    siendo util para poder determinar si es el hover actual o lo era */
    hover_actual = mouse_sobre_componente(componente, mouse_x, mouse_y) ? 
                   HOVER_ACTIVO : HOVER_NINGUNO;
    
    /* 4. Obtener estado anterior (dependiendo de lo que guardo hover.estado anteriormente) */
    hover_anterior = componente -> hover.estado;

    /* 5. Evaluar si el hover actual no es el anterior (sino, detener la funcion) */
    if (hover_actual == hover_anterior) return;
    
    /* 6. Procesar cambio de hover (solo si los hovers son diferentes) */
    mocultar(); /* Para evitar problemas con el renderizado se oculta primero el raton */
        
    /* 7. Si el hover actual es el que esta encima se dibuja el componente pero en 
    su version hover */
    if (hover_actual == HOVER_ACTIVO) {
        /* Mouse esta encima == Dibujar hover */
        renderizar_componente_hover(componente, renderizar_componente_especial);
    } 

    /* 8. Si el raton no esta encima de ningun componente */
    else {
        /* Mouse salio == Restaurar normal */
        if (renderizar_componente_especial != NULL) { /* Caso: Renderizado Especial */
            renderizar_componente_especial(componente);
        } else { /* Caso: Renderizado Normal/NULL */
            renderizar_componente(componente);
        }
    }
    
    /* 9. Una vez hecho el renderizado el raton vuelve a ser visible */
    mver();

    /* 10. Actualizar estado en la estructura */
    componente -> hover.estado = hover_actual;
}

/*
    manejar_hover_componente_texto()
    - Funcion general para manejar el hover para un componente de texto.
    - Replica la funcion original, con la diferencia de que este esta creado
    especialmente para manejar los textos en los componentes.

    - Parametros:
    Component *componente: Componente que se evaluara para hover
    int mouse_x, int mouse_y: Posicion actual del mouse
    char *texto: Texto inscrito en el centro del componente
*/
void manejar_hover_componente_texto(
    Component *componente,
    short mouse_x,
    short mouse_y,
    char *texto
) {
    /* 1. Se declaran variables de Enum HoverStatus para evaluar los hovers manejados
    por la interfaz y posiciones del raton */
    HoverStatus hover_actual;   /* Corresponde con el hover actual */
    HoverStatus hover_anterior; /* Corresponde con el hover anterior */
    short texto_x, texto_y;     /* Posiciones del texto */

    /* 2. Verificar si el hover est� disponible, sino, detiene la funcion */
    if (componente -> hover.disponible != HOVER_DISPONIBLE) return;
    
    /* 3. Determinar estado actual del hover a traves de un operador ternario */
    /* Esta variable obtendra un resultado booleano a traves de la funcion 
    mouse_sobre_componente, que permite evaluar si el raton esta encima de un componente,
    siendo util para poder determinar si es el hover actual o lo era */
    hover_actual = mouse_sobre_componente(componente, mouse_x, mouse_y) ? 
                   HOVER_ACTIVO : HOVER_NINGUNO;
    
    /* 4. Obtener estado anterior (dependiendo de lo que guardo hover.estado anteriormente) */
    hover_anterior = componente -> hover.estado;
    
    /* 5. Evaluar si el hover actual no es el anterior (sino, detener la funcion) */
    if (hover_actual == hover_anterior) return;
    
    /* 6. Capturar los centros del texto del componente */
    texto_x = CENTRO_X(componente) - textwidth(texto) / 2;
	texto_y = CENTRO_Y(componente) - textheight(texto) / 2 - 2;

    /* 7. Procesar cambio de hover (solo si los hovers son diferentes) */
    mocultar(); /* Para evitar problemas con el renderizado se oculta primero el raton */
        
    /* 8. Si el hover actual es el que esta encima se dibuja el componente pero en 
    su version hover */
    if (hover_actual == HOVER_ACTIVO) {
        /* Mouse esta encima == Dibujar hover */
        renderizar_componente_hover(componente, NULL);

        /* e imprimir texto */
        setcolor(componente->hover.color);
		if (textwidth(texto) >= componente->x2 - componente->x1)
            ajustar_texto_largo_componente(componente, texto, &texto_x, &texto_y);
        else
            outtextxy(texto_x, texto_y, texto);
        setcolor(componente->color);
    } 
    
    /* 9. Si el raton no esta encima de ningun componente */
    else {
        renderizar_componente(componente);

        /* Imprimir texto */
        setcolor(componente->color);
		if (textwidth(texto) >= componente->x2 - componente->x1)
			ajustar_texto_largo_componente(componente, texto, &texto_x, &texto_y);
        else
            outtextxy(texto_x, texto_y, texto);
    }
    
    /* 10. Una vez hecho el renderizado el raton vuelve a ser visible */
    mver();
    
    /* 11. Actualizar estado en la estructura */
    componente -> hover.estado = hover_actual;
}

/* Seccion 4. Funciones para manejo de logica interna */
/*
    mouse_sobre_componente()
    - Funcion que evalua si el raton esta encima de un componente a traves
	de comparaciones.
	- Devuelve un entero que corresponde a un estado (1 si es verdadero, 0 si es falso).

	- Parametros:
	Component *componente: Componente que se pasa por referencia, corresponde
	con el componente a evaluar.
	int mouse_x: Posicion actual del mouse en X.
	int mouse_y: Posicion actual del mouse en Y.
*/
short mouse_sobre_componente(Component *componente, short mouse_x, short mouse_y) {
	/* Retorna si el raton esta dentro del rango, superando la
    eficiencia de minlimit() */
    return (mouse_x >= componente->x1 && mouse_x <= componente->x2 &&
            mouse_y >= componente->y1 && mouse_y <= componente->y2);
}

/*
    detectar_click_componente()
    - Funcion que evalua si el raton hizo click a un componente.
	- Devuelve un entero que corresponde al click hacia un componente
    (1 si es verdadero, 0 si es falso).

	- Parametros:
	Component *componente: Componente que se pasa por referencia, corresponde
	con el componente a evaluar.
	int mouse_x: Posicion actual del mouse en X.
	int mouse_y: Posicion actual del mouse en Y.
*/
short detectar_click_componente(Component *componente, short mouse_x, short mouse_y) {
	/* Retorna si el raton esta dentro del rango */
	return mouse_sobre_componente(componente, mouse_x, mouse_y) && mclick() == 1;
}

/*
    ajustar_texto_largo_componente()
    - Funcion que ajusta y muestra el texto del componente solo si este es
    suficientemente largo para poder dividirse en dos lineas.

	- Parametros:
	Component *componente: Componente que se pasa por referencia, corresponde
	con el componente a evaluar.
	char *texto: Cadena de texto perteneciente al componente.
    short *texto_x, short *texto_y: Punteros a posiciones del texto dentro de componente.
*/
void ajustar_texto_largo_componente(Component *componente, char *texto, short *texto_x, short *texto_y)
{
    /* 1. Se declaran las variables a utilizar */
    /* Variables de longitudes */
    short distancia_x;
    short espaciado_y = textheight(texto) / 2;
    short tamano_temp = 0;

    /* Variables de indice */
    short letra = 0, ultimo_espacio = 0;

    /* Variables de proceso + buffer */
    char letra_procesada[2], *texto_temporal;

    /* 2. Reservar memoria para guardar lineas temporales */
    texto_temporal = (char *) malloc(sizeof(texto));
    if (texto_temporal == NULL)
        return; /* Tambien se valida esta reserva */

    /* 3. Limpiar buffer de texto temporal */
    memcpy(texto_temporal, 0, sizeof(texto));

    /* 4. Iniciar letra como cadena (el ultimo elemento es el caracter nulo) */
    letra_procesada[1] = '\0';
    
    /* e iniciar distancia de componente en x */
    distancia_x = (componente->x2 - componente->x1);

    /* 5. Buscar indice para separar texto */
    do 
    {
        /* 6. Capturar letra actual para convertirla a cadena */
        letra_procesada[0] = texto[letra];

        /* 7. Capturar espacio */
        if (texto[letra] == ' ') ultimo_espacio = letra;

        /* 8. Acumular tamano */
        tamano_temp += textwidth(letra_procesada);

        /* 9. Incrementar letra */
        letra++;
    /* Verificar si la linea no excede el tamano del componente */
    } while (tamano_temp - 3 < distancia_x || texto[letra] != '\0');

    /* 10. Guardar primera linea */
    letra = 0;
    while (letra < ultimo_espacio)
    {
        texto_temporal[letra] = texto[letra];
        letra++;
    }
    texto_temporal[letra] = 0;

    /* 11. Calcular posicion de primera linea */
    *texto_x = (distancia_x - textwidth(texto_temporal)) / 2 + componente->x1;

    /* 12. Imprimir primera linea */
    outtextxy(
        *texto_x,
        *texto_y - espaciado_y,
        texto_temporal
    );

    /* 13. Vaciar buffer de texto temporal */
    memcpy(texto_temporal, 0, sizeof(texto));

    /* 14. Guardar segunda linea */
    letra = ultimo_espacio;
    while (texto[letra] != '\0')
    {
        texto_temporal[letra - ultimo_espacio] = texto[letra + 1];
        letra++;
    }

    /* 15. Calcular tamano de segunda linea */
    *texto_x = (distancia_x - textwidth(texto_temporal)) / 2 + componente->x1;

    /* 16. Imprimir primera linea */
    outtextxy(
        *texto_x,
        *texto_y + espaciado_y,
        texto_temporal
    );

    /* 17. Liberar memoria */
    free(texto_temporal);
}