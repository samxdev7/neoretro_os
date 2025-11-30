#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifndef M_PI_2
#define M_PI_2 1.57079632679489661923
#endif

#ifndef PROTECT_H
#define PROTECT_H

/* 	"PROTECT_H"

	Libreria encargada de dibujar los tres protectores de pantalla
	Para utilizarla solo se debes llamar:
		-protector_1();		Movimiento similar al del DVD con "NeoRetro OS"
		-protector_2();		Carrusel de imagenes de paisajes en Nicaragua
		-protector_3();   	Creacion de figuras geometricas en lugar aleatorios, con colores aleatorios

    - Gabriela Ruiz
*/

/*
    =======================================
    Librerias Estandar
    =======================================
*/
#include <conio.h>
#include <dos.h>
#include <stdio.h>
#include <graphics.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

/*
    =======================================
    Librerias Propias
    =======================================
*/
#include "gphadmin.h"   /* Libreria que administra el modo grafico del sistema operativo */
#include "dynamic.h"    /* Libreria para manejar funciones de memoria dinamica */
#include "raster.h"     /* Libreria encargada de administrar la rasterizacion de ficheros */

/*
    =======================================
    Declaracion de Prototipos
    =========================================
*/
/* Protector de pantalla #1 - Estilo DVD */
int protector_1(void);
void dibujar_logo(int x, int y, int color);
void actualizar_posicion(int *x, int *y, int dx, int dy);
void manejar_rebotes(int *x, int *y, int *dx, int *dy, int width_logo, int height_logo, int *color);

/* Protector de pantalla #2- Carrusel de imagenes de paisajes */
int protector_2(void);
void mostrar_imagen_bin(const char *nombre_bin, int ancho, int alto, int sec);

/* Protector de pantalla #3- Figuras geometricas */
int protector_3(void);
void p_figuras(void);
void dibujar_estrella(void);
void dibujar_cuadrado(void);
void dibujar_triangulo(void);
void dibujar_pentagono(void);
void dibujar_hexagono(void);
void dibujar_circulo(void);
void dibujar_elipse(void);
void rellena_figuras_aleatorio(int npuntos, int *pts);
void liberar_puntos(int **p);
int **reservar_puntos(int n);
int y_aleatorio(int margen);
int x_aleatorio(int margen);
int color_aleatorio(void);

/* Array de punteros a funciones para pfiguras */
/* Explicacion general:
   - Cada elemento del arreglo es una funcion que dibuja una figura diferente.
   - Esto permite, por ejemplo, llamar a cualquier funcion de dibujo usando un indice: figuras[0]() dibuja el triangulo, figuras[5]() dibuja el circulo, etc.
*/
static void (*figuras[])(void) = {
    dibujar_triangulo, dibujar_cuadrado, dibujar_estrella,
    dibujar_pentagono, dibujar_hexagono, dibujar_circulo,
    dibujar_elipse
};

/* ===========================================================
   			1. ESTILO DVD
   =========================================================== */
/* ==========================================================
   Funcion: protector_1()
        Consiste en simular el movimiento que se realizaba en 
        pantalla durante la espera al conectar un reproductor DVD.
        Es decir nuestro logo, NeoRetro OS, recorriendo la pantalla 
        y al chocar con los bordes de la pantalla este rebote y 
        cambie de color.
   ========================================================== */
int protector_1(){
    int x = 100, y = 100;           /* Posicion inicial del logo en pantalla (coordenadas X y Y) */
    int dx = 6, dy = 3;             /* Velocidad del logo en X y en Y (pixeles por actualizacion) */
    int width_logo, height_logo;      /* Variables para guardar el ancho y alto del logo */
    int color = MAGENTA;            /* Color inicial del logo */

    colocar_configuraciones();      /*Se le aplican las configuraciones que caracterizan nuestro logo*/
    width_logo  = textwidth("NeoRetro OS");      /*Y se toman las medidas de este, de ancho*/
    height_logo = textheight("NeoRetro OS");     /*Y largo*/

    srand((unsigned)time(NULL));    
    /* Inicializa el generador de numeros aleatorios usando la hora actual, 
    para que los rebotes cambien de color de forma impredecible */

    /* Bucle principal */
    while (!kbhit()) {          /*Se ejecuta mientras no se presione una tecla*/
        set_bg(BLACK);

        dibujar_logo(x, y, color);       
        /* Dibuja el logo con los datos asignados en un inicio 
        y despues al continuar el bucle de va dibujando con la 
        nueva posicion actualizada*/

        actualizar_posicion(&x, &y, dx, dy);     /*Mueve el logo a traves de punteros en base a la velocidad dx, dy*/

        manejar_rebotes(&x, &y, &dx, &dy, width_logo, height_logo, &color); 
        /*Detecta los bordes de la pantalla y evita que el logo se salga, si rebota cambia de direccion y de color*/

        delay(60); /*Controla la velocidad de animacion*/
    }
	if (kbhit()) getch();   /* Si se presiono una tecla, la consume para evitar que quede en el buffer */
	return 0;
}
/* ==========================================================
   Funcion: dibujar_logo()
        Se encarga de dibujar el logo con las caracteristicas asignadas a 
        traves de colocar_configuraciones de la libreria gphadmin.h
        Sus parametros son x, y (la poscicion donde se dibuja) y el color.
   ========================================================== */
void dibujar_logo(int x, int y, int color) {
    colocar_configuraciones();
    setcolor(color);
    outtextxy(x, y, "NeoRetro OS");
}

/* ==========================================================
   Funcion: actualizar_posicion()
        Esta funcion es la encargada del movimiento del logo en toda la pantalla. 
        Primero recibe los punteros *x y *y los cuales contienen una direccion 
        que se ira modificando con el movimiento, luego dx y dy se encargan de 
        recibir el desplazamiento que se realizara y se sumara a los valores 
        de *x y *y (actualizando asi la posicion).
   ========================================================== */
void actualizar_posicion(int *x, int *y, int dx, int dy) {
    *x += dx;   /*Recibe el puntero *x, luego se le suma dx (desplazamiento) y modifica el valor de *x*/
    *y += dy;   /*Recibe el puntero *y, luego se le suma dy (desplazamiento) y modifica el valor de *y*/
}

/* ==========================================================
   Funcion: manejar_rebotes()
        Esta funcion es la encargada de detectar si el logo esta chocando con 
        alguno de los bordes de la pantalla (previamente definida la dimension 
        de la pantalla y la dimension del logo), una vez que lo detecta invierte 
        la direccion del movimiento modificando directamente el valor debido al 
        uso de punteros (generando el rebote) y cambia el color del logo.
   ========================================================== */
void manejar_rebotes(int *x, int *y, int *dx, int *dy, int width_logo, int height_logo, int *color)
{
    /* Rebote horizontal */
    /*Verifica si el logo toco el borde izq(*x <= 0) o der (*x + ancho del logo >= ancho de la pantalla)*/
    if (*x <= 0 || *x + width_logo >= WIDTH) { 
        *dx = -(*dx);   /*Invierte la direccion horizontal*/
        *color = color_aleatorio();  /*Cambia el color al rebotar*/
    }

    /* Rebote vertical */
    /*Verifica si el logo toco el borde superior(*y <= 0) o inferior (*y + alto del logo >= alto de la pantalla)*/
    if (*y <= 0 || *y + height_logo >= HEIGHT) {
        *dy = -(*dy);   /*Invierte la direccion vertical  del movimiento*/
        *color = color_aleatorio();  /*Cambia el color al rebotar*/
    }
}

/* ===========================================================
   			2. CARRUSEL
   =========================================================== */
/* ==========================================================
   Funcion: protector_2()
        Esta funcion consiste en la presentacion de un carrusel de imagenes 
        a traves de rasterizacion. Cada imagen se dibuja y espera 6 segundos 
        para pasar a la siguiente imagen. Las imagenes son de paisajes de 
        Nicaragua en Rivas, Tipitapa, Masaya, Bonanza y La Isla de Ometepe.
   ========================================================== */
int protector_2(){
    /* Fondo blanco */
    set_bg(WHITE);

     /* Bucle infinito hasta que usuario presione una tecla */
    while (!kbhit()) {

        /*Se muestra la imagen durante seis segundos y pasa a la siguiente imagen. 
        Si el usuario presiona una tecla despues de mostrarla, se rompe el ciclo 
        para salir del protector.*/

        mostrar_imagen_bin("rivas.bin",    320, 200, 6000);
        if (kbhit()) break;

        mostrar_imagen_bin("tipitapa.bin", 320, 200, 6000);
        if (kbhit()) break;

        mostrar_imagen_bin("masaya.bin",   320, 200, 6000);
        if (kbhit()) break;

        mostrar_imagen_bin("bonanza.bin",  320, 200, 6000);
        if (kbhit()) break;

        mostrar_imagen_bin("ometepe.bin",  320, 200, 6000);
        if (kbhit()) break;
    }
	if (kbhit()) getch();   /* Si se presiono una tecla, la consume para evitar que quede en el buffer */
	return 0;
}

/* ==========================================================
   Funcion: mostrar_imagen_bin()
        Esta funcion se encarga de dibujar las imagenes a utilizar en pantalla 
        (trabaja en conjunto a las funciones validar_archivo(file) 
        y dibujar_con_rasterizado_pos(file, CENTER, ancho, alto, WIDTH, HEIGHT). 
        Primeramente abre el archivo, la dibuja en pantalla usando 
        rasterizado, y cierra el archivo.
   ========================================================== */
void mostrar_imagen_bin(const char *nombre_bin, int ancho, int alto, int sec)
{
    FILE *file;

    /* Abrir archivo .bin en modo rb (lectura binaria) */
    file = fopen(nombre_bin, "rb");
    if (validar_archivo(file))
        return; 
    /*Si el archivo no se pudo abrir validar_archivo() devuelve verdadero 
    (de raster.h) y no detiene el programa, solo omite esa imagen*/

    /* Dibujar con la funcion dibujar_con_rasterizado_pos de raster.h */
    dibujar_con_rasterizado_pos(file, CENTER, ancho, alto, WIDTH, HEIGHT);

    /* Cerrar archivo despues de usarlo*/
    fclose(file);
	
    /*Retardo en cambiar la imagen*/
    delay(sec);
}


/* ===========================================================
   			3. FIGURAS GEOMETRICAS
   =========================================================== */
/* ==========================================================
   Funcion: protector_3()
        La funcion del tercer protector se basa en la creacion de figuras geometricas 
        en posiciones aleatorias en pantalla y colores aleatorios. Dichas figuras se 
        dibujan sobre un fondo negro con colores de la paleta svga256 desde el 32 hasta 103.
   ========================================================== */
int protector_3(){
    srand((unsigned)time(NULL));

    set_bg(BLACK);
    cleardevice();

    p_figuras();

	return 0;
}

/* ===========================================================
   Funcion: p_figuras()
        Esta funcion selecciona aleatoriamente de las diferentes funciones 
        encargadas de dibujar las figuras por separado para dibujarlas cada 
        dos segundos, mientras no se toque ninguna tecla.
   =========================================================== */
void p_figuras() {
	int nTipos = sizeof(figuras) / sizeof(figuras[0]);
    	int indice;

    /*Dibujar figuras indefinidamente hasta que el usuario toque una tecla*/
    while (!kbhit()) {
        indice = rand() % nTipos; /*elegir figura aleatoria*/
        figuras[indice]();        /*dibujar la figura*/
        delay(2000);               /*espera 2 s para ver aparecer la figura*/
    }
	if (kbhit()) getch();
}

/* ===========================================================
   Funcion: color_aleatorio()
        Selecciona un color de la paleta SVGA256 con el rango 
        especificado del tono 32 al 103.
   =========================================================== */
int color_aleatorio() {
     return 32 + rand() % (103 - 32 + 1);  
     /*Regresa un numero aleatorio entre 0 y (103-32), es decir entre 0 y 71.
       Al sumarle 32, el resultado final queda entre 32 y 103.*/
}

/* ===========================================================
   Funcion: x_aleatorio()
        Esta funcion genera una coordenada X aleatoria, garantizando 
        que la posicion este dentro de la pantalla y a una distancia 
        minima del borde.
   =========================================================== */
int x_aleatorio(int margen) {
    if (margen < 0) margen = 0; /* Si el margen enviado es negativo, se corrige a 0 para evitar errores */
    return margen + rand() % (WIDTH - 2*margen); 
    /* Produce un valor entre 0 y (WIDTH - 2*margen - 1), que se suma al margen inicial.
       Esto asegura que la posicion generada quede dentro del area visible
       dejando un margen a la izquierda y a la derecha.*/
}

/* ===========================================================
   Funcion: y_aleatorio()
        Esta funcion genera una coordenada Y aleatoria, garantizando 
        que la posicion esté dentro de la pantalla y a una distancia 
        minima del borde.
   =========================================================== */
int y_aleatorio(int margen) {
    if (margen < 0) margen = 0; /* Si el margen es negativo, se corrige para evitar posiciones invalidas */
    return margen + rand() % (HEIGHT - 2*margen);
    /* Genera un numero entre 0 y (HEIGHT - 2*margen - 1). 
        Esto asegura que la posicion generada quede dentro del area visible
       dejando un margen superior e inferior.*/
}

/* ===========================================================
   Funcion: **reservar_puntos()
        Esta función reserva memoria dinámica usando funciones de nuestra librería 
        dynamic.h para almacenar n puntos en 2D usando un puntero doble: primero 
        asigna memoria para un int** llamado mat, y luego asigna un bloque contiguo 
        de n * 2 enteros (cada punto tiene dos valores: X e Y). Si cualquiera de 
        las dos asignaciones falla, la función libera lo que ya se había reservado 
        y devuelve NULL; si todo sale bien, devuelve el puntero doble donde *mat 
        apunta al arreglo de coordenadas.
   =========================================================== */
int **reservar_puntos(int n) {
    /* Se reserva memoria para un puntero a puntero*/
    int **mat = (int **) malloc(sizeof(int *));
    if (mat == NULL) return NULL;   /* Si falla la reserva, regresar NULL */

    *mat = (int *) malloc(sizeof(int) * n * 2);
    /* Se reserva un bloque de memoria para n puntos (n*2 enteros). 
       *mat sera el arreglo plano donde se guardan los valores.
    */
    if (*mat == NULL) {
        /* Si falla esta segunda reserva, se limpia la memoria previa */
        liberar_elemento(mat);
        return NULL;
    }
    return mat; /*Si todo el proceso salio bien,entonces regresa el dobre puntero*/
}

/* ===========================================================
   Funcion: liberar_puntos()
        Esta funcion libera correctamente la memoria reservada para los puntos: 
        - Primero verifica que el puntero no sea nulo.
        - Luego, si *p apunta al bloque donde estaban almacenadas las coordenadas,
        lo libera usando liberar_elemento
        - Finalmente libera el propio puntero doble p, asegurando que ambas partes 
        de la memoria asignada por reservar_puntos queden completamente liberadas.
   =========================================================== */
void liberar_puntos(int **p) {
    if (p == NULL) return;  /* Si el puntero principal es NULL, no hay nada que liberar */
    
    /* Si el contenido (*p) apunta al bloque donde se guardan los puntos,
       se libera primero ese arreglo de enteros.*/
    if (*p != NULL) liberar_elemento(*p);

    liberar_elemento(p); /* Se libera el puntero doble 'p', que contenía la dirección del arreglo.*/
}

/* ===========================================================
   Funcion: rellenar_figuras_aleatori()
        Esta funcion recibe un arreglo de puntos que define un poligono y, si los 
        datos son validos, genera al azar un color para el borde y otro para el relleno.
   =========================================================== */
void rellena_figuras_aleatorio(int npuntos, int *pts){
    int borde, relleno;
	if (pts == NULL || npuntos <= 0) return; /* Si no hay puntos o el arreglo es nulo, no se hace nada */
    /*Se le asignan colores aleatorios a las variables de borde y relleno*/
    borde =color_aleatorio();
    relleno =color_aleatorio();
    /*Se configuran los colores para dibujar y rellenar*/
    setcolor(borde);
    setfillstyle(SOLID_FILL, relleno);

    drawpoly(npuntos, pts);     /*Se dibuja el contorno del poligono*/
    fillpoly(npuntos, pts);     /*Se rellena el poligono*/
	}

/* ================== Figuras geometricas ==================== */

/* ===========================================================
   Funcion: dibujar_triangulo()
        Esta funcion genera un triangulo aleatorio en pantalla:
        - Calcula una altura entre 20 y 39 (para el tamaño del triangulo).
        - Elige coordenadas dentro de la pantalla usando x_aleatorio y y_aleatorio.
        - Reserva memoria para los 3 puntos del triangulo con reservar_puntos, 
            asigna las coordenadas de los vertices en forma de triangulo isosceles
        - Llama a rellena_figuras_aleatorio para dibujarlo y rellenarlo.
        - Libera la memoria reservada con liberar_puntos.
   ======================================================== */
void dibujar_triangulo() {
    int altura = 20 + rand() % 30; /* Altura aleatoria del triangulo entre 20 y 49 */
    /*Se asignan coordenadas de x y y aleatorias*/
    int x = x_aleatorio(altura/2);
    int y = y_aleatorio(altura);
    /* Numero de puntos de un triangulo */
    int n = 3;
    /* Reserva memoria para los puntos del triangulo */
    int **pts = reservar_puntos(n);
    int *p = *pts;

    if (!pts) return; /* Si falla la reserva, se sale de la funcion */

    /* Definicion de las coordenadas de los 3 vertices del triangulo */
    p[0] = x; p[1] = y;         /*Vertice superior*/
    p[2] = x - altura/2; p[3] = y + altura;     /*Vertice izq*/
    p[4] = x + altura/2; p[5] = y + altura;     /*Verice der*/

    rellena_figuras_aleatorio(n, p); /*Dibuja y rellena el triangulo con colores aleatorios*/
    liberar_puntos(pts); /*Libera los puntos*/
}

/* ===========================================================
   Funcion: dibujar_cuadrado()
        Genera un cuadrado aleatorio en pantalla.
        - Calcula un tamaño entre 30 y 79 y la mitad de ese tamaño (half). 
        - Luego selecciona coordenadas centrales usando x_aleatorio y y_aleatorio.  
        - Reserva memoria para los 4 vertices con reservar_puntos. 
        - Asigna las coordenadas de cada esquina 
        - Llama a rellena_figuras_aleatorio para dibujar y rellenar el cuadrado. 
        - Libera la memoria reservada con liberar_puntos.
   =========================================================== */
void dibujar_cuadrado() {
    int tam = 30 + rand() % 50; /* Tamaño aleatorio del cuadrado entre 30 y 79*/
    /* Se calcula la mitad del tamaño para calcular los vertices respecto al centro */
    int half = tam / 2; 

    /*Se asignan coordenadas de x y y aleatorias*/
    int x = x_aleatorio(half);
    int y = y_aleatorio(half);

    /* Reserva memoria para los 4 vertices del cuadrado */
    int n = 4;
    int **pts = reservar_puntos(n);
    int *p = *pts;

    if (!pts) return; /* Validacion de la reserva*/

    /*Definicion de las coordenadas para los vertices del cuadradp*/
    p[0] = x-half; p[1] = y-half;   /*Vertice superior izq*/
    p[2] = x+half; p[3] = y-half;   /*Vertice superior der*/
    p[4] = x+half; p[5] = y+half;   /*Vertice inferior der*/
    p[6] = x-half; p[7] = y+half;   /*Vertice inferior izq*/

    rellena_figuras_aleatorio(n, p); /*Dibuja y rellena el cuadrado con colores aleatorios*/
    liberar_puntos(pts); /*Libera los puntos*/
}

/* ===========================================================
   Funcion: dibujar_estrella()
        Genera una estrella de 5 puntas aleatoria en pantalla.
        - Calcula un radio externo entre 20 y 49 y un radio interno(la mitad).
        - Selecciona un centro usando x_aleatorio y y_aleatorio 
        - Reserva memoria para los 10 puntos de la estrella con reservar_puntos 
            y, mediante trigonometría, alterna los radios externo e interno 
            para calcular las coordenadas de cada vértice usando coseno y seno. 
        - Dibuja y rellena la estrella con rellena_figuras_aleatorio.
        - Libera la memoria con liberar_puntos.
   =========================================================== */
void dibujar_estrella() {
    int ext = 20 + rand() % 30; /* Radio externo aleatorio entre 20 y 49 */
    int inte = ext / 2; /* Radio interno: la mitad del externo */

    /* Coordenadas aleatorias del centro de la estrella */
    int x = x_aleatorio(ext);
    int y = y_aleatorio(ext);

    /*Reserva de los vertices de la estrella (internos 5 y externos 5)*/
    int n = 10;
    int **pts = reservar_puntos(n);
    int *p = *pts;

    double paso = 2.0 * M_PI / n;   /* Paso angular entre cada vertice */
    double ang, r;
    int i;

    if (!pts) return; /* Validacion de la reserva*/

    /* Calcula las coordenadas de cada vertice alternando radio externo e interno */
    for (i = 0; i < n; i++) {
        ang = -M_PI_2 + i * paso; 
        r = (i % 2 == 0) ? ext : inte;
        p[2*i] = x + (int)(r * cos(ang));
        p[2*i+1] = y + (int)(r * sin(ang));
    }

    rellena_figuras_aleatorio(n, p); /*Dibuja y rellena el cuadrado con colores aleatorios*/
    liberar_puntos(pts); /*Libera los puntos*/
}

/* ===========================================================
   Funcion: dibujar_pentagono()
        Esta función genera un pentágono aleatorio en pantalla.
        - Calcula un tamaño entre 20 y 49.
        - Selecciona un centro usando x_aleatorio y y_aleatorio.
        - Reserva memoria para los 5 vertices con reservar_puntos y, usando 
            trigonometria, calcula las coordenadas de cada vertice 
            distribuyendolos uniformemente en un circulo alrededor del centro.
        - Dibuja y rellena el pentagono con rellena_figuras_aleatorio.
        - Libera la memoria reservada con liberar_puntos.
   =========================================================== */
void dibujar_pentagono() {
    int tam = 20 + rand() % 30;  /* Tamaño aleatorio del pentagono entre 20 y 49*/
    
    /* Coordenadas aleatorias del centro */
    int x = x_aleatorio(tam);
    int y = y_aleatorio(tam);

    /* Reserva memoria para los puntos del pentagono */
    int n = 5, i;
    int **pts = reservar_puntos(n);
    int *p = *pts;

    /* Paso angular entre cada vertice (2π dividido entre n vertices) */
    double paso = 2.0 * M_PI / n;
    double ang;
    
    if (!pts) return;   /* Validación de la reserva */

    for(i=0;i<n;i++){   /*Calcula las coordenadas de cada vertice*/
        ang = -M_PI_2 + i*paso;
        p[2*i] = x + (int)(tam*cos(ang));
        p[2*i+1] = y + (int)(tam*sin(ang));
    }
    
    rellena_figuras_aleatorio(n, p);    /*Dibuja y rellena el cuadrado con colores aleatorios*/
    liberar_puntos(pts);    /*Libera los puntos*/
}

/* ===========================================================
   Funcion: dibujar_hexagono()
        Esta funcion dibuja un hexagono en posiciones aleatorias.
        - Calcula un tamaño entre 20 y 49.
        - Selecciona un centro (x, y) usando x_aleatorio y y_aleatorio.
        - Reserva memoria para los 6 vertices con reservar_puntos y, 
            mediante trigonometria, distribuye uniformemente los vertices.
        - Dibuja y rellena el hexagono con rellena_figuras_aleatorio.
        - Libera la memoria con liberar_puntos.
   =========================================================== */
void dibujar_hexagono() {
    int tam = 20 + rand() % 30; /*Tamaño aleatorio entre 20 y 49*/
    /*Coordenadas centrales aleatorias*/
    int x = x_aleatorio(tam);
    int y = y_aleatorio(tam);

    /*Reserva de 6 vertices (puntos) del hexagono*/
    int n = 6, i;
    int **pts = reservar_puntos(n);
    int *p = *pts;

    /* Paso angular entre cada vertice (2π dividido entre n vertices) */
    double paso = 2.0 * M_PI / n;
    double ang;

    if (!pts) return; /* Validacion de la reserva */

    for(i=0;i<n;i++){   /* Calcula las coordenadas de cada vertice */
        ang = -M_PI_2 + i*paso;
        p[2*i] = x + (int)(tam*cos(ang));
        p[2*i+1] = y + (int)(tam*sin(ang));
    }
    rellena_figuras_aleatorio(n, p);    /*Dibuja y rellena el cuadrado con colores aleatorios*/
    liberar_puntos(pts);    /*Libera los puntos*/
}

/* ===========================================================
   Funcion: dibujar_circulo()
        Dibuja un circulo aleatorio en pantalla.
        - Calcula un radio entre 10 y 29.
        - Elige un centro usando x_aleatorio y y_aleatorio 
        - Genera colores aleatorios para el borde y el relleno y configura 
            esos colores con setcolor y setfillstyle.
        - Dibuja una elipse rellena con fillellipse y dibuja el contorno del circulo con circle.
   =========================================================== */
void dibujar_circulo() {
    int radio = 10 + rand() % 20;   /* Se asgina un radio aleatorio del circulo entre 10 y 29 pixeles */
    
    /* Coordenadas aleatorias del centro del circulo */
    int x = x_aleatorio(radio);
    int y = y_aleatorio(radio);

    /* Colores aleatorios para el borde y el relleno */
    int borde = color_aleatorio();
    int relleno = color_aleatorio();

    /*Configuracion de borde y relleno del circulo*/
    setcolor(borde);
    setfillstyle(SOLID_FILL, relleno);

    /*Dibuja el circulo relleno y el contorno*/
    fillellipse(x, y, radio+5, radio);
    circle(x, y, radio);
}

/* ===========================================================
   Funcion: dibujar_()
        Esta funcion dibuja una elipse aleatoria en pantalla.
        - Calcula los radios horizontal rx (10–29) y vertical ry (5–19).
        - Selecciona un centro usando x_aleatorio y y_aleatorio.
        - Genera colores aleatorios para el borde y el relleno y los configura con setcolor y setfillstyle.
        - Dibuja la elipse rellena con fillellipse y dibuja el contorno con ellipse.
   =========================================================== */
void dibujar_elipse() {
    int rx = 10 + rand() % 20;  /*Se genera un radio aleatorio en x (horizontal) de 10 a 29 pixeles*/
    int ry = 5 + rand() % 15;   /*Se genera un radio aleatorio en y (vertical) de 5 a 19 pixeles*/

     /* Coordenadas aleatorias del centro de la elipse */
    int x = x_aleatorio(rx);
    int y = y_aleatorio(ry);

     /* Colores aleatorios para el borde y el relleno */
    int borde = color_aleatorio();
    int relleno = color_aleatorio();

    /*Configuracion de borde y relleno de la elipse*/
    setcolor(borde);
    setfillstyle(SOLID_FILL, relleno);

    /*Dibuja la elipse rellena y el contorno*/
    fillellipse(x, y, rx, ry);
    ellipse(x, y, 0, 360, rx, ry);
}
#endif