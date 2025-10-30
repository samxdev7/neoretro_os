#ifndef GEOMETRY_H
#define GEOMETRY_H

/*
	GEOMETRY_H

	Libreria que contiene funciones necesarias para hacer 
	transformaciones geometricas. 
	
	- Moises Aleman
*/

/*
    =======================================
    Importacion de Librerias Estandar
    =========================================
*/
#include <math.h>

/*
    =======================================
    Declaración de Directivas
    =========================================
*/
#define PI 3.1415926535897932						/* Constante PI */
#define ANG_TO_RAD(angulo) (angulo * PI / 180.0)	/* Macro de Conversion de Angulos a Radianes */

/*
    =======================================
    Declaracion de Prototipos
    =========================================
*/
void escalar_vector(int *vector_original, int *vector_destino, int vertices, float escalar);
/* XOR LINE, agregar */
void trasladar_xy_inplace(int *xy, int n, int ox, int oy);
void trasladar_y(int *vector, int vertices, int n);
void trasladar_x(int *vector, int vertices, int n);
void flip_horiz_inplace_flat(int *xy, int n, int x_ref);
void flip_vert_inplace_flat(int *xy, int n, int y_ref) ;
void rotation(int *original, int size_arr, float angulo, int p_rot_x, int p_rot_y);

/*
    =======================================
    Inicializacion de Funciones
    =========================================
*/

/* 
	escalar_vector()
	Funcion para escalar un vector de vertices.

	Parametros:
	- vector_original: vector a escalar
	- vector_destino: vector donde se guarda el resultado
	- vertices: numero de vertices del vector
	- escalar: factor de escala 
*/

void escalar_vector(int *vector_original, int *vector_destino, int vertices, float escalar)
{
	int i;
	float tempx = 0, tempy = 0;
	float centro_x = 0, centro_y = 0;
	float dx, dy;
	for(i = 0; i < vertices; i++)
	{
		centro_x += vector_original[i*2];
		centro_y += vector_original[i*2+1];
	}

	centro_x /= vertices;
	centro_y /= vertices;

	for(i = 0; i < vertices; i++)
	{
		dx = vector_original[i*2] - centro_x;
		dy = vector_original[i*2+1] - centro_y;
		
		tempx = centro_x + (dx * escalar);
		tempy = centro_y + (dy * escalar);

		vector_destino[i * 2] = (int)(tempx + 0.5f);
		vector_destino[i * 2 + 1] = (int)(tempy + 0.5f);
	}
}

/* 
	trasladar_xy_inplace()
	
	- Descripcion
*/

void trasladar_xy_inplace(int *xy, int n, int ox, int oy) 
{ 
    int *p = xy;
    int i;
    for ( i = 0; i < n; ++i)
    {
        *p++ += ox; /* x */
        *p++ += oy; /* y */
    }
}

/* 
	trasladar_y()
	Funcion para trasladar un vector de vertices verticalmente.

	Parametros:
	- vector: vector a trasladar
	- vertices: numero de vertices del vector
	- n: numero de pixeles a trasladar
*/

void trasladar_y(int *vector, int vertices, int n)
{
	int i;
	for(i = 0; i < vertices; i++)
	{
		vector[i*2+1] += n;
	}
}

/* 
	trasladar_x()
	Funcion para trasladar un vector de vertices horizontalmente.

	Parametros:
	- vector: vector a trasladar
	- vertices: numero de vertices del vector
	- n: numero de pixeles a trasladar
*/

void trasladar_x(int *vector, int vertices, int n)
{
	int i;

	for(i = 0; i < vertices; i++)
	{
		vector[i*2] += n;
	}
}

/* 
	trasladar_xy_inplace()
	
	- Descripcion
*/

void flip_horiz_inplace_flat(int *xy, int n, int x_ref) 
{
    int *p = xy;
    int i;
    for (i = 0; i < n; ++i) {
        *p = 2*x_ref - *p;  /* reflejar x */
        p++;                /* avanzar a y */
        p++;                /* siguiente par */
    }
}

/* 
	flip_vert_inplace_flat()
	
	- Descripcion
*/

void flip_vert_inplace_flat(int *xy, int n, int y_ref) 
{
    int *p = xy;
    int i;
    for (i = 0; i < n; ++i) {
        p++;
        *p = 2*y_ref - *p;
        p++;
    }
}

/*
	rotation()
	Rota respecto al origen, la imagen que se recibira se trasladara al 
	origen antes de aplicar rotacion.
*/

void rotation(int *original, int size_arr, float angulo, int p_rot_x, int p_rot_y)
{
	int i;
	float angulo_rad = ANG_TO_RAD(angulo),
		cos_tetha = cos(angulo_rad)
		,
		sin_tetha = sin(angulo_rad);
	int x, y;
	float x_temp, y_temp, new_x, new_y;

	for(i = 0; i < size_arr; i++)
	{
		x = original[i*2];
		y = original[i*2+1];

		/* trasladar al origen */
		x_temp = x - p_rot_x;
		y_temp = y - p_rot_y;

		/* aplicar rotacion */
		new_x = x_temp * cos_tetha - y_temp * sin_tetha;
		new_y = x_temp * sin_tetha + y_temp * cos_tetha;

		/* trasladar de vuelta a su posicion inicial */
		original[i*2] = (int)(new_x + p_rot_x + 0.5f);
		original[i*2+1] = (int)(new_y + p_rot_y + 0.5f);
	}
}

#endif
