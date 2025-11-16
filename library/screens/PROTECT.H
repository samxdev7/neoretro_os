#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifndef M_PI_2
#define M_PI_2 1.57079632679489661923
#endif

/* 	"protect.h"

	Libreria encargada de los tres protectores de pantalla
	Para utilizarla solo se debes llamar:
		-protector1();		Movimiento similar al del DVD con "NeoRetro OS"
		-protector2();		Carrusel de imagenes de paisajes en Nicaragua
		-protector3();   	Creacion de figuras geometricas en lugar aleatorios, con colores aleatorios  */
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
#include "gphadmin.h"
#include "dynamic.h"
#include "raster.h"

/*
    =======================================
    Tamaño de pantalla
    =======================================
*/
#define WIDTH 320
#define HEIGHT 200

/* ===========================================================
   Funciones
   =========================================================== */
	/*Protector de pantalla #1 - Estilo DVD*/
int protector1(void);
void dibujarLogo(int x, int y, int color);
void dibujarFondo(void);
void actualizarPosicion(int *x, int *y, int dx, int dy);
void manejarRebotes(int *x, int *y, int *dx, int *dy, int width, int height, int *color);

	/*Protector de pantalla #2- Carrusel de imagenes de paisajes*/
int protector2(void);
void mostrarImagenBin(const char *nombre_bin, int ancho, int alto, int sec);

	/*Protector de pantalla #3- Figuras geometricas*/
int protector3(void);
void pfiguras(void);
void dibujarEstrella(void);
void dibujarCuadrado(void);
void dibujarTriangulo(void);
void dibujarPentagono(void);
void dibujarHexagono(void);
void dibujarCirculo(void);
void dibujarElipse(void);
void rellenaFigurasAleatorio(int npuntos, int *pts);
void liberarPuntos(int **p);
int **reservarPuntos(int n);
int yAleatorio(int margen);
int xAleatorio(int margen);
int colorAleatorio(void);

	/* Array de punteros a funciones para pfiguras */
static void (*figuras[])(void) = {
    dibujarTriangulo, dibujarCuadrado, dibujarEstrella,
    dibujarPentagono, dibujarHexagono, dibujarCirculo,
    dibujarElipse
};
/* ===========================================================
   			1. ESTILO DVD
   =========================================================== */
/* ==========================================================
   Funcion: protector1
   ========================================================== */
int protector1(){
    int x = 100, y = 100;
    int dx = 6, dy = 3;
    int widthLogo, heightLogo;
    int color = MAGENTA;

    if (iniciar_modo_svga_256("C:\\TC20\\BIN"))
        return EXIT_FAILURE;

    colocar_configuraciones();
    widthLogo  = textwidth("NeoRetro OS");
    heightLogo = textheight("NeoRetro OS");

    srand((unsigned)time(NULL));

    /* Bucle principal */
    while (!kbhit()) {
        dibujarFondo();

        dibujarLogo(x, y, color);

        actualizarPosicion(&x, &y, dx, dy);

        manejarRebotes(&x, &y, &dx, &dy,
                       widthLogo, heightLogo, &color);

        delay(60);
    }
	if (kbhit()) getch();
	return 0;
}
/* ==========================================================
   Funcion: Dibujar Logo
   ========================================================== */
void dibujarLogo(int x, int y, int color) {
    colocar_configuraciones();
    setcolor(color);
    outtextxy(x, y, "NeoRetro OS");
}

/* ==========================================================
   Funcion: Dibujar Fondo
   ========================================================== */
void dibujarFondo() {
    cleardevice();
    setfillstyle(SOLID_FILL, BLACK);
    bar(0, 0, WIDTH, HEIGHT);
}

/* ==========================================================
   Funcion: Actualizar Posición
   ========================================================== */
void actualizarPosicion(int *x, int *y, int dx, int dy) {
    *x += dx;
    *y += dy;
}

/* ==========================================================
   Funcion: Manejar Rebotes + Cambiar color
   ========================================================== */
void manejarRebotes(int *x, int *y, int *dx, int *dy, int widthLogo, int heightLogo, int *color)
{
    /* Rebote horizontal */
    if (*x <= 0 || *x + widthLogo >= WIDTH) {
        *dx = -(*dx);
        *color = colorAleatorio();
    }

    /* Rebote vertical */
    if (*y <= 0 || *y + heightLogo >= HEIGHT - 10) {
        *dy = -(*dy);
        *color = colorAleatorio();
    }
}


/* ===========================================================
   			2. CARRUSEL
   =========================================================== */
/* ==========================================================
   Funcion: protector2
   ========================================================== */
int protector2(){
	/* Iniciar modo grafico */
    if (iniciar_modo_svga_256("C:\\TC20\\BIN"))
        return EXIT_FAILURE;

    /* Fondo blanco */
    setfillstyle(SOLID_FILL, WHITE);
    bar(0, 0, WIDTH, HEIGHT);

     /* Bucle infinito hasta que usuario presione una tecla */
    while (!kbhit()) {

        mostrarImagenBin("rivas.bin",    320, 200, 6000);
        if (kbhit()) break;

        mostrarImagenBin("tipitapa.bin", 320, 200, 6000);
        if (kbhit()) break;

        mostrarImagenBin("masaya.bin",   320, 200, 6000);
        if (kbhit()) break;

        mostrarImagenBin("bonanza.bin",  320, 200, 6000);
        if (kbhit()) break;
    }
	if (kbhit()) getch();
	return 0;
}

/* ==========================================================
   Funcion: Mostrar imagen rasterizada
   ========================================================== */
void mostrarImagenBin(const char *nombre_bin, int ancho, int alto, int sec)
{
    FILE *file;

    /* Abrir archivo */
    file = fopen(nombre_bin, "rb");
    if (validar_archivo(file))
        return; /* no detener el programa, solo omitir imagen */

    /* Dibujar con rasterizado */
    dibujar_con_rasterizado_pos(file, CENTER, ancho, alto, WIDTH, HEIGHT);

    /* Cerrar archivo */
    fclose(file);
	
    /*Retardo en cambiar la imagen*/
    delay(sec);

}


/* ===========================================================
   			3. FIGURAS GEOMETRICAS
   =========================================================== */
/* ==========================================================
   Funcion: protector3
   ========================================================== */
int protector3(){
    srand((unsigned)time(NULL));

    if (iniciar_modo_svga_256("C:\\TC20\\BIN")) return EXIT_FAILURE;

    setbkcolor(0);
    cleardevice();

    pfiguras();

	return 0;
}

/* ===========================================================
   Protector de pantalla con figuras aleatorias
   =========================================================== */
void pfiguras() {
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
   Funciones para posicion y colores aleatorios
   =========================================================== */
int colorAleatorio() {
     return 32 + rand() % (103 - 32 + 1);
}

int xAleatorio(int margen) {
    if (margen < 0) margen = 0;
    return margen + rand() % (WIDTH - 2*margen);
}

int yAleatorio(int margen) {
    if (margen < 0) margen = 0;
    return margen + rand() % (HEIGHT - 2*margen);
}

/* ===========================================================
   Reserva y liberacion de puntos usando dynamic.h
   =========================================================== */
int **reservarPuntos(int n) {
    /* Se reserva un puntero a puntero*/
    int **mat = (int **) malloc(sizeof(int *));
    if (mat == NULL) return NULL;

    *mat = (int *) malloc(sizeof(int) * n * 2);
    if (*mat == NULL) {
        liberar_elemento(mat);
        return NULL;
    }
    return mat;
}

void liberarPuntos(int **p) {
    if (p == NULL) return;
    if (*p != NULL) liberar_elemento(*p);
    liberar_elemento(p);
}


/* ===========================================================
   Rellenar figuras con colores aleatorios
   =========================================================== */
void rellenaFigurasAleatorio(int npuntos, int *pts){
    int borde, relleno;
	if (pts == NULL || npuntos <= 0) return;
    borde =colorAleatorio();
    relleno =colorAleatorio();
    setcolor(borde);
    setfillstyle(SOLID_FILL, relleno);
    drawpoly(npuntos, pts);
    fillpoly(npuntos, pts);
	}

/* ===========================================================
   Figuras geometricas
   =========================================================== */

/* -----Triangulo------ */
void dibujarTriangulo() {
    int altura = 20 + rand() % 30;
    int x = xAleatorio(altura/2);
    int y = yAleatorio(altura);

    int n = 3;
    int **pts = reservarPuntos(n);
    int *p = *pts;

    if (!pts) return;

    p[0] = x; p[1] = y;
    p[2] = x - altura/2; p[3] = y + altura;
    p[4] = x + altura/2; p[5] = y + altura;

    rellenaFigurasAleatorio(n, p);
    liberarPuntos(pts);
}

/* -------Cuadrado------- */
void dibujarCuadrado() {
    int tam = 30 + rand() % 50;
    int half = tam / 2;
    int x = xAleatorio(half);
    int y = yAleatorio(half);

    int n = 4;
    int **pts = reservarPuntos(n);
    int *p = *pts;

    if (!pts) return;

    p[0] = x-half; p[1] = y-half;
    p[2] = x+half; p[3] = y-half;
    p[4] = x+half; p[5] = y+half;
    p[6] = x-half; p[7] = y+half;

    rellenaFigurasAleatorio(n, p);
    liberarPuntos(pts);
}

/* ------Estrella-------- */
void dibujarEstrella() {
    int ext = 20 + rand() % 30;
    int inte = ext / 2;

    int x = xAleatorio(ext);
    int y = yAleatorio(ext);
    int n = 10;

    int **pts = reservarPuntos(n);
    int *p = *pts;

    double paso = 2.0 * M_PI / n;
    double ang, r;
    int i;

    if (!pts) return;

    for (i = 0; i < n; i++) {
        ang = -M_PI_2 + i * paso;
        r = (i % 2 == 0) ? ext : inte;
        p[2*i] = x + (int)(r * cos(ang));
        p[2*i+1] = y + (int)(r * sin(ang));
    }

    rellenaFigurasAleatorio(n, p);
    liberarPuntos(pts);
}

/* ------Pentagono------- */
void dibujarPentagono() {
    int tam = 20 + rand() % 30;
    int x = xAleatorio(tam);
    int y = yAleatorio(tam);

    int n = 5, i;
    int **pts = reservarPuntos(n);
    int *p = *pts;

    double paso = 2.0 * M_PI / n;
    double ang;
    
    if (!pts) return;

    for(i=0;i<n;i++){
        ang = -M_PI_2 + i*paso;
        p[2*i] = x + (int)(tam*cos(ang));
        p[2*i+1] = y + (int)(tam*sin(ang));
    }
    rellenaFigurasAleatorio(n, p);
    liberarPuntos(pts);
}

/* ------Hexagono------ */
void dibujarHexagono() {
    int tam = 20 + rand() % 30;
    int x = xAleatorio(tam);
    int y = yAleatorio(tam);

    int n = 6, i;
    int **pts = reservarPuntos(n);
    int *p = *pts;

    double paso = 2.0 * M_PI / n;
    double ang;

    if (!pts) return;

    for(i=0;i<n;i++){
        ang = -M_PI_2 + i*paso;
        p[2*i] = x + (int)(tam*cos(ang));
        p[2*i+1] = y + (int)(tam*sin(ang));
    }
    rellenaFigurasAleatorio(n, p);
    liberarPuntos(pts);
}

/* -------Circulo-------- */
void dibujarCirculo() {
    int radio = 10 + rand() % 20;
    int x = xAleatorio(radio);
    int y = yAleatorio(radio);

    int borde = colorAleatorio();
    int relleno = colorAleatorio();

    setcolor(borde);
    setfillstyle(SOLID_FILL, relleno);
    fillellipse(x, y, radio+5, radio);
    circle(x, y, radio);
}

/* -------Elipse-------- */
void dibujarElipse() {
    int rx = 10 + rand() % 20;
    int ry = 5 + rand() % 15;
    int x = xAleatorio(rx);
    int y = yAleatorio(ry);

    int borde = colorAleatorio();
    int relleno = colorAleatorio();

    setcolor(borde);
    setfillstyle(SOLID_FILL, relleno);
    fillellipse(x, y, rx, ry);
    ellipse(x, y, 0, 360, rx, ry);
}