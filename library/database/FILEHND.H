#ifndef FILEHND_H
#define FILEHND_H
/*
    FILEHND.H
    Manejo de archivos binarios para propietarios y mascotas
    Proyecto de base de datos simple
    
    Carlos Torres
    Carlos Valdivia
    Johanna Duarte
    Moises Aleman
 */

 /*
    =========================================================================================================================
    Descripcion del modulo
    =========================================================================================================================
    Este modulo contiene las funciones necesarias para manejar
    archivos binarios que almacenan informacion de propietarios
    y mascotas. Incluye funciones para guardar y cargar datos, hay
    funciones que aun no estan del todo implementada, por ahora las funciones
    que estan completadas son las de agregar propietario y mascota.
    
    el motivo del porque el propietario tiene su propio id y las mascotas tambien tienen
    tambien su propia estructura es, porque, esta pensado para que un propietario pueda tener
    mas de una mascota, facilitando asi los tiempos de carga, y no tener que agregar datos de un 
    propietario que parezca como repetido en la base de datos de los propietarios, evitando duplicados.

    NOTA: EN LA PARTE DE FICHEROS TRABAJAR CON FICHEROS BINARIOS PARA OPTIMIZAR TIEMPOS DE LECTURA Y ESCRITURA.
    AL USAR FICHEROS BINARIOS, SE REDUCE EL ESPACIO DE ALMACENAMIENTO Y SE MEJORA LA EFICIENCIA EN LA GESTION DE DATOS.
    =========================================================================================================================
  */

/*
    =======================================
    Importacion de librerias estandar
    ========================================= 
*/

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <ctype.h>
#include <stddef.h>
#include <string.h>
#include <dos.h>

/*
    =======================================
    Importacion de librerias propias
    =========================================
*/
#include "VALID.H"  /* Libreria de validaciones de entrada */
#include "boolso.h" /* Libreria de simulacion del booleano */

/*
    =======================================
    Definicion de Constantes
    =========================================
*/

/* 
    convencion para su facil uso de true y false al igual que el tipo de datos bool
    ya que en C no existe por defecto.
*/
#define ID_BASE 1000 /* ID base para propietarios y mascotas */

/*
    =======================================
    Definicion de Enums
    =========================================
*/
/* 
    enum File_states_t
    Estructura para usarse con funciones que retornen un int, codigos de errores comunes
    al manipular archivos.

    nota: los codigos de error son negativos para diferenciarlos de los posibles
    ademas, evitar usar estos codigos en funciones de tipo bool que retornan true o false.
*/
typedef enum {
    FILE_NOT_FOUND = -1, /* archivo no encontrado */
    FILE_READ_ERROR = -2, /* error al leer el archivo */
    FILE_WRITE_ERROR = -3, /* error al escribir en el archivo */
    FILE_BYTES_NOT_FOUNDED = -4 /* los bytes del archivo no coinciden con el tamaño esperado */
} File_states_t;
    
/* 
    enum Consts_vet_info_t
    Estructura para usarse con funciones que retornen constantes que estan relacionadas con 
    la informacion de los propietarios y mascotas.

    IMPORTANTE: estas constantes definen los tamaños maximos de los campos de texto en la estructura Formulario_Vet_t
    cambiar estos puede causar errores de memoria al leer o escribir en archivos binarios.

    Si se desea cambiar y ya tiene una version de la base de datos creada, se recomienda crear una nueva base de datos
    es decir, borrar los archivos binarios existentes y crear nuevos archivos con las nuevas constantes.
*/
typedef enum {
    MAX_OWNER_NAME = 50, /* max cantidad de bytes para el nombre del propietario */
    MAX_ADRESS = 256, /* max cantidad de bytes para la direccion */
    MAX_PHONE = 25, /* max cantidad de bytes para el telefono */
    MAX_PET_NAME = 40, /* max cantidad de bytes para el nombre de la mascota */
    MAX_PET_SPECIE = 40 /* max cantidad de bytes para la especie de la mascota */
} Consts_vet_info_t;

/*
    =======================================
    Definicion de Estructuras
    =========================================
*/
/* 
    Estructura para almacenar informacion de propietarios
*/
typedef struct {
    bool activo;
    int id;
    char nombre_propietario[MAX_OWNER_NAME];
    char direccion[MAX_ADRESS];
    char telefono[MAX_PHONE];
    char nombre_mascota[MAX_PET_NAME];
    char especie[MAX_PET_SPECIE];
} Formulario_Vet_t;

/*
    =======================================
    Declaracion de Variables Globales
    =========================================
*/
/*
    Ruta de los archivos binarios para propietarios y mascotas
    en variables constantes para evitar errores de tipeo.
    puedes cambiar las rutas segun tu estructura de carpetas.
    esta es la estructura de carpetas recomendada:
    C:.
        TC20
            Ficheros
                ENTRYS_COUNT_PATH.count
                PELUDITOS_DB.bin
*/
const char PELUDITOS_DB[40] = "C:\\TC20\\Ficheros\\Main_DB.bin";

/*
    Definimos una estructura global de tipo Formulario_Vet_t
    y la inicializamos con {0} para reservar espacio en memoria.
*/
Formulario_Vet_t formulario = {0}; /* usado para hacer append a la DB */
Formulario_Vet_t *db_info = NULL; /* usado para cargar propietarios desde la DB */

/* 
    Variables globales usadas para funcion de load_db_info, global para poder ser usada en
    otras funciones que necesiten accesso a informacion de la db
*/
size_t size, count, i;
const char diccionario[22][20] = { /* especies validas en la veterinaria */
        "canino",
        "felino",
        "lagomorfo",
        "roedor",
        "huron",
        "equino",
        "bovino",
        "porcino",
        "ovino",
        "caprino",
        "ave",
        "reptil",
        "anfibio",
        "pez"
    };

/*
    =======================================
    Declaracion de Prototipos
    =========================================
*/
bool verify_loaded_DB(void); /* Verificar si la base de datos ha sido cargada previamente */
void print_info(size_t i); /* imprimir informacion de un propietario por indice */
<<<<<<< HEAD
bool end(void); /* Verificar si la base de datos ha sido cargada previamente e imprime un msj de error si no se ha cargado*/
=======
bool end(void); /* Verificar si la base de datos ha sido cargada previamente */
>>>>>>> 3118c7333c33e2702d8f10b39c40ccabff258da4
bool verify_entry_iscorrect(Formulario_Vet_t *data); /* Verificar si los datos del formulario son correctos */
char* limpiar_cadena(char *cadena); /* limpia la cadena de espacios y convierte la cadena en minusculas */
bool buffers_isequal(const char *buf1, const char *buf2); /* compara dos buffers de caracteres */
bool save_new_entry(const char *file, Formulario_Vet_t *data); /* guardar en ficheros binarios la informacion de propietarios */
void load_db_info(const char *file); /* cargar desde ficheros binarios la informacion de propietarios */
void ver_db_info(void); /* ver la informacion cargada desde la base de datos */
bool save_db_info(const char *file); /* guardar la informacion cargada en memoria de vuelta a la base de datos */
void mostrar_menu(void); /* mostrar el menu principal */
void agregar_nueva_entrada(void); /* funcion principal para agregar propietarios, esta funcion usa la funcion save_new_entry */
void buscar_entrada(void); /* funcion para buscar propietarios por ID o Nombre */
void modificar_entrada(void); /* funcion para modificar una entrada existente en la base de datos */
long tamano_archivo(const char*); /* funcion para obtener el tamaño de un archivo en bytes */
void db_stats_active_owners(void); /* funcion para ver usuarios activos e inactivos */
void db_stats_species_count(void); /* funcion para ver conteo de especies en la base de datos */
void view_stats(void); /* menu para ver estadisticas de la base de datos */
int get_next_entry_id(void) ; /* obtener el siguiente ID disponible para un nuevo propietario */
void app_database(void); /* funcion principal de la aplicacion de base de datos */

<<<<<<< HEAD
/*
    =======================================
    Inicializacion de Funciones
    =========================================
*/
bool verify_loaded_DB()
{ /* verificar si la base fue cargada exitosamente */
=======
bool verify_loaded_DB()
{
>>>>>>> 3118c7333c33e2702d8f10b39c40ccabff258da4
    if (db_info == NULL) return false;
    return true;
}

bool end()
<<<<<<< HEAD
{ /* similar a verify_loaded_db a diferencia que esta pensada para manejar mensaje de error en pantalla */
    if (!verify_loaded_DB())
    {
        clrscr(); 
        textcolor(LIGHTRED); /* texto con cprintf para hacer contraste al mostrar este mensaje de error */
=======
{
    if (!verify_loaded_DB())
    {
        clrscr();
        textcolor(LIGHTRED);
>>>>>>> 3118c7333c33e2702d8f10b39c40ccabff258da4
        cprintf("Error: La base de datos no ha sido cargada. Por favor, carguela primero.\n\r");
        textcolor(WHITE);
        delay(1000);
        return false;
    }
    return true;
}

<<<<<<< HEAD
void print_info(size_t i) /* imprimir los datos en el inciso n, encapsulado ya que se utiliza en muchas funciones */
{
    printf("ID: %d\n", db_info[i].id); 
=======
void print_info(size_t i)
{
    printf("ID: %d\n", db_info[i].id);
>>>>>>> 3118c7333c33e2702d8f10b39c40ccabff258da4
    printf("Nombre: %s\n", db_info[i].nombre_propietario);
    printf("Direccion: %s\n", db_info[i].direccion);
    printf("Telefono: %s\n", db_info[i].telefono);
    printf("Nombre Mascota: %s\n", db_info[i].nombre_mascota);
    printf("Especie: %s\n", db_info[i].especie);
}

char* limpiar_cadena(char *cadena)
{
    int len = 0, i, j;
    char *p;

    /* cuenta solo los valores alfanumericos en la cadena recibida, acumulando en 'len' */
    for (i = 0; cadena[i] != '\0'; i++) if(isalpha(cadena[i])) len++;

    p = (char*)malloc((len + 1) * sizeof(char)); /* puntero a char de la cantidad de 'len' */
    if (!p) /* manejo de error al asignar espacio en memoria */
    {
        textcolor(LIGHTRED);cprintf("Error al asignar memoria para limpiar cadena...\n\r");
        textcolor(WHITE);
        getch();
        return NULL;
    }

    j = 0; /* variable para aritmetica de punteros */
    for (i = 0; cadena[i] != '\0'; i++)
    {
        if (isalpha(cadena[i]))/* eliminar espacios y en minuscula */
            p[j++] = tolower((unsigned char)cadena[i]);
    }
    p[j] = '\0'; /* poner el caracter '\0' para indicar final de cadena */
    return p; /* regresamos un puntero a la cadena 'limpia' */
}

int leer_linea_segura(char *buffer, size_t tam)
{
    char *nl; 
    char *res = fgets(buffer, tam, stdin);
    if (!res) { /* si encuentra '\n' error */
        return -1;  /* error de lectura */
    }

    nl = strchr(buffer, '\n');
    if (nl) {
        *nl = '\0'; /* eliminar el salto de linea */
        return 1;  /* lectura exitosa */
    }

    /* No hay '\n' -> el usuario escribió más de lo que cabe */
    limpiar_buffer();

    return 0;  /* overflow */
}

void mostrar_msj_err(void)
{
    clrscr(); 
    textcolor(LIGHTRED);
    cprintf("Error: Entrada invalida. Por favor, intente de nuevo.\n\r");
    textcolor(WHITE);
}

/* para que esta funcion sea efectiva se tiene que 'limpiar' la cadena mediante la funcion 'limpiar_cadena()' */
bool buffers_isequal(const char *buf1, const char *buf2)
{
    while (*buf1 && *buf2) /* compara dos cadenas */
    {
        if (*buf1 != *buf2)
            return false; /* se encontro un caracter distinto en las cadenas! */
        buf1++;
        buf2++;
    }
    if (*buf1 == '\0' && *buf2 == '\0')
        return true; /* las cadenas son iguales! */
    return false; /* error inesperado */
}


bool save_new_entry(const char *file, Formulario_Vet_t *data)
{/* funcion para guardar una nueva entrada en el archivo */
    FILE *p = fopen(file, "ab"); /* abrir archivo en modo append binario */
    if (!p) return false;
    if (fwrite(data, sizeof(Formulario_Vet_t), 1, p) != 1) /* escribir la estructura en el archivo */
    {
        fclose(p);
        return false;
    }
    if(fclose(p) == EOF) return false; /* cerrar el archivo */
    return true; /* exito */
}

bool save_db_info(const char *file)
{
    FILE *p;
    if (!end()) /* verificar que la base de datos ha sido cargada previamente */
<<<<<<< HEAD
        return false; 
=======
        return false;
>>>>>>> 3118c7333c33e2702d8f10b39c40ccabff258da4

    p = fopen(file, "wb"); /* abrir archivo en modo escritura binaria */
    if (!p) 
    { 
        textcolor(LIGHTRED);
        cprintf("Error: No se pudo abrir el archivo para guardar la base de datos.\n\r");
        textcolor(WHITE);
        return false; /* error al abrir archivo */
    }

    for (i = 0; i < count; i++)
    {
        if (fwrite(&db_info[i], sizeof(Formulario_Vet_t), 1, p) != 1)
        {
            fclose(p);
            textcolor(LIGHTRED);
            cprintf("Error al escribir en la base de datos.\n\r");
            textcolor(WHITE);
            return false; /* error al escribir en el archivo */
        }
    }

    if (fclose(p) == EOF)
    {
        textcolor(LIGHTRED);
        cprintf("Error al cerrar el archivo de la base de datos.\n\r");
        textcolor(WHITE);
        return false; /* error al cerrar el archivo */
    }

    return true; /* exito */
}

long tamano_archivo(const char *file_address)
{
    long size; /* variable para almacenar el tamaño del archivo */

    FILE *p = fopen(file_address, "rb"); /* abrir archivo en modo lectura binaria */
    if (!p) return -1; /* si no se pudo abrir el archivo retornar -1 */

    fseek(p, 0, SEEK_END); /* mover el puntero al final del archivo */
    size = ftell(p); /* obtener el tamaño en bytes del archivo */
    fclose(p); /* cerrar el archivo */
    return size; /* retornar el tamaño del archivo */
}

/* ============================================================================================

    los numeros de registro o id, empiezan desde el numero 1000, es decir, 
    el primer registro de la base de datos que vendria a ser el 1 empezara en 1000.
    
    es asi entonces que 1 = 1000

===============================================================================================
*/
int get_next_entry_id(void)
{
    const char *path = PELUDITOS_DB; /* direccion de la base de datos */
    FILE *f = fopen(path, "rb"); /* abrir archivo binario en modo lectura */
    Formulario_Vet_t ultimo; /* declarar una estructura del formulario */
    size_t bytes, count, offset;

    /* Si el archivo no existe, primer ID base */
    if (!f) return ID_BASE; 

    /* Ir al final y medir tamaño */
    if (fseek(f, 0, SEEK_END) != 0) 
    { 
        fclose(f); 
        return FILE_READ_ERROR;  /* error al leer el archivo */
    }
    
    bytes = (size_t)ftell(f); /* obtener tamano en bytes */
    if (bytes < 0) 
    { 
        fclose(f);
        return FILE_READ_ERROR;  /* error al leer el archivo */
    }

    /* Archivo vacío → primer ID base */
    if (bytes == 0) 
    {
        fclose(f);
        return ID_BASE; /* primer ID base */
    }

    /* Debe ser múltiplo exacto del tamaño del registro */
    if (bytes % sizeof(Formulario_Vet_t) != 0) 
    {
        fclose(f);
        return FILE_BYTES_NOT_FOUNDED;
    }

    /* Posicionarse al inicio del ÚLTIMO registro */
    count  = bytes / sizeof(Formulario_Vet_t); /* cantidad de registros en el archivo */
    
    offset = (count - 1) * sizeof(Formulario_Vet_t); /* calcular offset del ultimo registro */
    
    if (fseek(f, offset, SEEK_SET) != 0) /* posicionar el puntero al inicio del último registro */
    {
        fclose(f); /* cerrar el archivo */
        return FILE_READ_ERROR;  /* error al leer el archivo */
    }

    /* Leer el último registro y cerrar */
    if (fread(&ultimo, sizeof(ultimo), 1, f) != 1) 
    {
        fclose(f); /* cerrar el archivo */
        return FILE_READ_ERROR;  /* error al leer el archivo */
    }
    fclose(f); /* cerrar el archivo */

    return ultimo.id + 1; /* retornar el siguiente ID disponible */
}

void load_db_info(const char *file) 
{
    FILE *p = fopen(file, "rb"); /* abrimos el archivo binario en modo lectura */
    
    if (!p) /* si no se pudo abrir el archivo */
    {
        clrscr();
        printf("Error: No se pudo abrir el archivo de la base de datos.\n\r");
        getch();
        return; /* salir de la funcion si no se pudo abrir el archivo */
    } 

    fseek(p, 0, SEEK_END); /* movemos el puntero al final del archivo binario */
    size = (size_t)ftell(p); /* obtenemos el tamano en bytes del archivo binario */
    fseek(p, 0, SEEK_SET); /* regresamos el puntero al inicio del archivo binario */

    if (size == 0) /* si ftell retorno un valor de 0 significa que el archivo binario esta vacio */
    {
        fclose(p); /* cerramos el archivo */
        printf("La base de datos esta vacia.\n");
        getch();
        return; /* salir de la funcion si el archivo esta vacio */
    }

    count = size / sizeof(Formulario_Vet_t); /* tamano completo del archivo / el tamnano de la estructura almacenada dentro */
    db_info = (Formulario_Vet_t*)malloc(size); /* asignar memoria para cargar los datos de la DB en memoria para su manipulacion */
    if (!db_info) /* si malloc falla alertar error */
    {
        fclose(p);
        printf("Error de memoria\n");
        getch();
        return; /* salir de la funcion si no se pudo asignar memoria */
    }

    for (i = 0; i < count; i++)
    {
        /* Carga la DB en la memoria previamente asignada; Del archivo binario a db_info */
        if (fread(&db_info[i], sizeof(Formulario_Vet_t), 1, p) != 1)
        {
            free(db_info);
            fclose(p);
            printf("Error al leer la base de datos.\n");
            getch();
            return; /* salir de la funcion si no se pudo leer el archivo */
        }
    }

    fclose(p); /* cerrar el archivo */
    printf("Base de datos cargada con exito!.\n");
    printf("Se han cargado %d entradas desde la base de datos!\n\n", (int)count); /* mostrar cantidad de entradas cargadas */
}

/* 
    ===================================================================================
    Esta funcion se encarga de mostrar por completo todos los registros ACTIVOS de 
    la base de datos, util para ver en orden los registros y su respectivo ID.

    Nota: Para que esta funcion muestre los datos correctamente; la base de datos tiene
    que ser montada en memoria previamente.
    ===================================================================================
*/
void ver_db_info(void)
{

    clrscr(); 
    printf("Mostrando propietarios ACTIVOS de la base de datos:\n", (int)count); /* mostrar mensaje inicial */
    for (i = 0; i < count; i++)
    {
        if (db_info[i].activo == false) /* verificar si el registro esta activo */
            continue; /* saltar registros inactivos */
        
        printf("-------------------------------\n\r");
<<<<<<< HEAD
        print_info(i); /* imprimir informacion del propietario en el indice i */
=======
        print_info(i);
>>>>>>> 3118c7333c33e2702d8f10b39c40ccabff258da4
        printf("--------------------------------\n\r");
        printf("Entero para continuar...\n\r");
        textcolor(WHITE);
        getch(); /* esperar a que el usuario presione una tecla */
    }
    printf("Fin de la base de datos...\n");
<<<<<<< HEAD
    delay(1000); /* esperar 1 segundo antes de continuar */
=======
    delay(1000);
>>>>>>> 3118c7333c33e2702d8f10b39c40ccabff258da4
}

bool validate_specie(const char *especie)
{
    size_t i; 
    const char *clr_str = limpiar_cadena((char*)especie); /* limpiar la cadena de especie */
    
    for (i = 0; i < 22; i++)
    {
        if (buffers_isequal(diccionario[i], clr_str) == true)
        {
            free(clr_str);
            return true; /* especie valida */
        }
    }
    free(clr_str);
    return false; /* especie no valida */
}

bool verify_op_input(char *input, char max_option, char min_option)
{
    size_t i, len = strlen(input);
    for (i = 0; i < len; i++)
    {
        if (!isdigit((unsigned char)input[i]))
            return false; /* input contiene caracteres no numericos */

    }
    if (atoi(input) < min_option || atoi(input) > max_option)
        return false; /* input fuera de rango valido */

    return true; /* input es valido */
}

bool verify_entry_iscorrect(Formulario_Vet_t *data)
{
    size_t i, size;
    const char *np, *dir, *tel, *nm, *esp;
    if (buffer_isempty(data->nombre_propietario) == BUF_EMPTY ||
        buffer_isempty(data->direccion) == BUF_EMPTY ||
        buffer_isempty(data->telefono) == BUF_EMPTY ||
        buffer_isempty(data->nombre_mascota) == BUF_EMPTY ||
        buffer_isempty(data->especie) == BUF_EMPTY
        )
    {
        return false; /* algun campo obligatorio esta vacio */
    }

    if (strlen(data->nombre_propietario) >= MAX_OWNER_NAME ||
        strlen(data->direccion) >= MAX_ADRESS || /* verificar longitud maxima de direccion */
        strlen(data->telefono) >= MAX_PHONE ||
        strlen(data->nombre_mascota) >= MAX_PET_NAME ||
        strlen(data->especie) >= MAX_PET_SPECIE
        )
    {
        return false; /* algun campo obligatorio excede el maximo permitido */
    }

    np = data->nombre_propietario; /* puntero a nombre propietario */
    dir = data->direccion; /* puntero a direccion */
    tel = data->telefono; /* puntero a telefono */
    nm = data->nombre_mascota; /* puntero a nombre mascota */
    esp = data->especie; /* puntero a especie */

    size = strlen(np); /* obtener longitud de nombre propietario */
    for (i = 0; i < size; i++)
    {
        if (np[i] == '\n')
            continue;

        if (!isalpha((unsigned char)np[i]) && np[i] != ' ' && np[i] != '.')
            return false; /* nombre propietario contiene caracteres invalidos */
    }

    size = strlen(dir); /* obtener longitud de direccion */
    for (i = 0; i < size; i++)
    {
        if (dir[i] == '\n')
            continue;

        if (!isalpha((unsigned char)dir[i]) && dir[i] != ' ' && dir[i] != ',' && dir[i] != '.' &&
            dir[i] != '#' && dir[i] != '-' && dir[i] != '/' && !isdigit((unsigned char)dir[i]) && dir[i] != '(' && dir[i] != ')')
            return false; /* direccion contiene caracteres invalidos */
    }

    size = strlen(tel); /* obtener longitud de telefono */
    for (i = 0; i < size; i++)
    {
        if (tel[i] == '\n')
            continue;

        if (!isdigit((unsigned char)tel[i]) && tel[i] != '+' && tel[i] != '-' && tel[i] != ' '
            && tel[i] != '(' && tel[i] != ')')
            return false; /* telefono contiene caracteres invalidos */
    }

    size = strlen(nm); /* obtener longitud de nombre mascota */
    for (i = 0; i < size; i++)
    {
        if (nm[i] == '\n')
            continue;

        if (!isalpha((unsigned char)nm[i]) && nm[i] != ' ')
            return false; /* nombre mascota contiene caracteres invalidos */
    }

    size = strlen(esp); /* obtener longitud de especie */
    for (i = 0; i < size; i++)
    {
        if (esp[i] == '\n')
            continue;

        if (!isalpha((unsigned char)esp[i]) && esp[i] != ' ' && esp[i] != '.')
            return false; /* especie contiene caracteres invalidos */
        
        if (!validate_specie(esp)) /* verificar si la especie es valida comparando en el diccionario */
        {
<<<<<<< HEAD
            printf("Especie no reconocida. \n"); /* especie no reconocida */
            delay(1000); /* esperar 1 segundo antes de continuar */
=======
            printf("Especie no reconocida. \n");
            delay(1000);
>>>>>>> 3118c7333c33e2702d8f10b39c40ccabff258da4
            return false; /* especie no reconocida */
        }
    }

    return true; /* todos los campos son correctos */
}

void buscar_entrada(void)
{
    int i;
    char *p_DB, *q_BUFF; /* p_DB(puntero a Nombres en estructura), p_BUFF(puntero a cadena introducida por user) */
    char busqueda[50]; /* cadena que guarda el input del usuario */
<<<<<<< HEAD
    if (!end()) /* verificar que la base de datos ha sido cargada previamente */
    {
        clrscr();
        printf("Error: La base de datos no ha sido cargada. Por favor, carguela primero.\n");
        delay(1000); /* esperar 1 segundo antes de continuar */
=======
    if (!end())
    {
        clrscr();
        printf("Error: La base de datos no ha sido cargada. Por favor, carguela primero.\n");
        delay(1000);
>>>>>>> 3118c7333c33e2702d8f10b39c40ccabff258da4
        return;
    }
    limpiar_buffer();
    printf("\nIngrese el Nombre del propietario a buscar: ");
    if(leer_linea_segura(busqueda, sizeof(busqueda)) != 1)
    { mostrar_msj_err(); return; }

    q_BUFF = limpiar_cadena(busqueda);
    
    for (i = 0; i < count; i++)
    {
        p_DB = limpiar_cadena(db_info[i].nombre_propietario);
        if (buffers_isequal(p_DB, q_BUFF) == true)
        {
            clrscr();
            printf("----- Entrada Encontrada -----\n");
<<<<<<< HEAD
            print_info(i); /* imprimir informacion del propietario en el indice i */
=======
            print_info(i);
>>>>>>> 3118c7333c33e2702d8f10b39c40ccabff258da4
            printf("------------------------------\n\n");
            free(p_DB);
            free(q_BUFF);
            printf("Enter para continuar...\n");
<<<<<<< HEAD
            getch(); /* esperar a que el usuario presione una tecla */
=======
            getch();
>>>>>>> 3118c7333c33e2702d8f10b39c40ccabff258da4
            return;
        }
    }
    free(q_BUFF);
    free(p_DB);
    printf("No se encontro ninguna entrada con ese nombre.\n");
    delay(1000);
}

void mostrar_menu(void){
    int i;

    clrscr();

    /* ---------------- CABECERA ---------------- */

	textcolor(14);

    /*Línea superior: ╔══════════╗ -> ASCII: 201 + 205 + 187*/
    printf("   %c", 201);                          /* ╔*/
    for(i = 0; i < 46; i++) printf("%c", 205); /* ═*/
    printf("%c\n", 187);                       /* ╗*/

    /* Título*/
    printf("   %c", 186);  /* ║*/
    textcolor(YELLOW);
    cprintf("                MENU PRINCIPAL                ");
    textcolor(WHITE);
    printf("%c\n", 186);  /* ║*/

    /* Separador: ╠═══════╣*/
    printf("   %c", 204);                          /* ╠ */
    for(i = 0; i < 46; i++) printf("%c", 205); /* ═*/
    printf("%c\n", 185);                       /* ╣*/


    /* ---------------- OPCIONES ---------------- */
    printf("   %c  1. ", 186);
    printf("Agregar nueva entrada                    %c\n", 186);
    printf("   %c  2. ", 186);
    printf("Ver Base de Datos                        %c\n", 186);
    printf("   %c  3. ", 186);
    printf("Buscar propietario                       %c\n", 186);
    printf("   %c  4. ", 186);
    printf("Modificar entrada                        %c\n", 186);
    printf("   %c  5. ", 186);
    printf("Ver estadisticas                         %c\n", 186);
    printf("   %c  6. ", 186);
    printf("Salir                                    %c\n", 186);
    /* ------------- RELLENO (líneas vacías) ------------- */
    for(i = 0; i < 2; i++){
        printf("   %c                                              %c\n",
               186, 186);
    }
    /* ---------------- PIE DEL MENU ---------------- */
    printf("   %c", 200);
	for(i = 0; i < 46; i++) printf("%c", 205);
	printf("%c\n", 188);
    printf("\n");
    /* ---------------- TEXTO DE SELECCIÓN ---------------- */
    printf("   Seleccione una opcion: ");
}

void agregar_nueva_entrada(void)
{
    bool valid = false;
    
    clrscr();
    do
    {
        limpiar_buffer(); /* limpiar buffer antes de leer entradas */
        printf("Ingrese nombre del propietario: "); 

        /* si algun campo no es una cadena valida reiniciar la entrada */
        if(leer_linea_segura(formulario.nombre_propietario, MAX_OWNER_NAME) != 1){ mostrar_msj_err(); continue; }
        printf("\nIngrese direccion del propietario: "); 
        if(leer_linea_segura(formulario.direccion, MAX_ADRESS) != 1){ mostrar_msj_err(); continue; }
        printf("\nIngrese telefono del propietario: "); 
        if(leer_linea_segura(formulario.telefono, MAX_PHONE) != 1){ mostrar_msj_err(); continue; }
        printf("\nIngrese el nombre de la mascota: "); 
        if(leer_linea_segura(formulario.nombre_mascota, MAX_PET_NAME) != 1){ mostrar_msj_err(); continue; }
        printf("\nIngrese la especie de la mascota: "); 
        if(leer_linea_segura(formulario.especie, MAX_PET_SPECIE) != 1){ mostrar_msj_err(); continue; }
        if (!verify_entry_iscorrect(&formulario))
        {
            clrscr();
            textcolor(LIGHTRED);
            cprintf("Error: Algunos campos contienen caracteres invalidos.\nTodos los campos son requeridos para continuar.\n");
            textcolor(WHITE);
        }

        else valid = true;
    }while (!valid);
    formulario.id = get_next_entry_id();
    formulario.activo = true;
    if (save_new_entry(PELUDITOS_DB, &formulario))
    {
        printf("Entrada guardada con exito en la base de datos!\n");
    }
    else 
    {
        printf("Error al guardar la informacion del propietario.\n");
    }
	delay(1000);
}

void modificar_entrada(void)
{
    int i;
    bool valid, valid2;
    char *p_DB, *q_BUFF; /* p_DB(puntero a Nombres en estructura), p_BUFF(puntero a cadena introducida por user) */
    char busqueda[50], opcion[16]; /* cadena que guarda el input del usuario */
    if (!end()) return;

    textcolor(YELLOW);
    cprintf("Ingrese el Nombre del propietario a modificar: ");
    textcolor(WHITE);
    limpiar_buffer();
    if (leer_linea_segura(busqueda, sizeof(busqueda)) != 1)
    {
        mostrar_msj_err();
        return;
    }

    q_BUFF = limpiar_cadena(busqueda);
    
    for (i = 0; i < count; i++)
    {
        p_DB = limpiar_cadena(db_info[i].nombre_propietario); /* limpiar el nombre del propietario en la DB */
        if (buffers_isequal(p_DB, q_BUFF) == true) /* comparar cadenas */
            break; /* entrada encontrada */
    }

    if (i == count) /* si se llego al final del ciclo sin encontrar la entrada */
    {
        printf("\n");
        textcolor(LIGHTRED);
        cprintf("No se encontro ninguna entrada con ese nombre.\n\r");
        textcolor(WHITE);
        getch();
        free(q_BUFF);
        free(p_DB);
        return;
    }
    
    valid = false;
    do
    {
        clrscr();
        textcolor(LIGHTBLUE);
        cprintf("----- Entrada Encontrada -----\n\r");
<<<<<<< HEAD
        print_info(i); /* imprimir informacion del propietario en el indice i */
        printf("------------------------------\n\r");
        if (db_info[i].activo == false) /* verificar si el registro esta activo */
=======
        print_info(i);
        printf("------------------------------\n\r");
        if (db_info[i].activo == false) 
>>>>>>> 3118c7333c33e2702d8f10b39c40ccabff258da4
        {
            cprintf("Estado: ");
            textcolor(LIGHTRED);
            cprintf("INACTIVO\n");
        }
        else /* registro activo */
        {
            cprintf("Estado: ");
            textcolor(LIGHTGREEN);
            cprintf("ACTIVO\n");
        }
        textcolor(WHITE);
        
        printf("\n");
        printf("------------------------------\n");
        printf("A continuacion elija la opcion segun la gestion que desea hacer:\n");
        printf("(1) Modificar datos del registro.\n");
        printf("(2) Eliminar datos de la base de datos de PELUDITOS & CO.\n");
        printf("(3) Cambiar estado a ACTIVO (Si el registro esta inactivo).\n");
        printf("(4) Cancelar/Salir.\n\n");
        printf("Elija una opcion: ");
        scanf("%s", opcion);
        if (!verify_op_input(opcion, 4, 1)) /* verificar que la opcion sea valida */
        {
            textcolor(LIGHTRED);
            cprintf("Error: Entrada invalida. Por favor, ingrese un numero valido.\n\r");
            textcolor(WHITE);
            getch();
            limpiar_buffer();
            continue;
        }
        printf("\n");
        switch (atoi(opcion)) /* convertir la opcion a entero */
        {
            case 1:
                valid2 = false;
                clrscr();
                do
                {
                    limpiar_buffer();
                    printf("Ingrese nombre del propietario: ");
<<<<<<< HEAD
                    /* leer entradas del usuario, reiniciar entrada si la entrada no es valida */
=======
>>>>>>> 3118c7333c33e2702d8f10b39c40ccabff258da4
                    if(leer_linea_segura(db_info[i].nombre_propietario, MAX_OWNER_NAME) != 1){ mostrar_msj_err(); continue; }
                    printf("\n\rIngrese direccion del propietario: ");
                    if(leer_linea_segura(db_info[i].direccion, MAX_ADRESS) != 1){ mostrar_msj_err(); continue; }
                    printf("\n\rIngrese telefono del propietario: ");
                    if(leer_linea_segura(db_info[i].telefono, MAX_PHONE) != 1){ mostrar_msj_err(); continue; }
                    printf("\n\rIngrese el nombre de la mascota: ");
                    if(leer_linea_segura(db_info[i].nombre_mascota, MAX_PET_NAME) != 1){ mostrar_msj_err(); continue; }
                    printf("\n\rIngrese la especie de la mascota: ");
                    if(leer_linea_segura(db_info[i].especie, MAX_PET_SPECIE) != 1){ mostrar_msj_err(); continue; }
                    
                    if (!verify_entry_iscorrect(&db_info[i])) /* verificar si los datos del formulario son correctos */
                    {
                        clrscr();
                        printf("Error: Algunos campos contienen caracteres invalidos.\nTodos los campos son requeridos para continuar.\n"); 
                        printf("Por favor, intente de nuevo.\n");
                    }
                    else valid2 = true; /* datos correctos, salir del ciclo */
                }while (!valid2);
<<<<<<< HEAD
                save_db_info(PELUDITOS_DB); /* guardar los cambios en la base de datos */
                break; /* fin case 1 */
=======
                save_db_info(PELUDITOS_DB);
                break;
>>>>>>> 3118c7333c33e2702d8f10b39c40ccabff258da4
            case 2:
                if (db_info[i].activo == false) /* verificar si el registro ya esta inactivo */
                {
                    printf("El registro ya esta INACTIVO. No se realizaron cambios.\n");
                    getch();
                    valid = true; /* salir del ciclo */
                    break;
                }
<<<<<<< HEAD
                db_info[i].activo = false; /* cambiar estado a inactivo */
                save_db_info(PELUDITOS_DB); /* guardar los cambios en la base de datos */
                break; /* fin case 2 */
=======
                db_info[i].activo = false;
                save_db_info(PELUDITOS_DB);
                break;
>>>>>>> 3118c7333c33e2702d8f10b39c40ccabff258da4
            case 3:
                if (db_info[i].activo == true) /* verificar si el registro ya esta activo */
                {
                    printf("El registro ya esta ACTIVO. No se realizaron cambios.\n");
                    delay(1000);
<<<<<<< HEAD
                    valid = true; /* salir del ciclo */
                    break;
                }
                db_info[i].activo = true; /* cambiar estado a activo */
                save_db_info(PELUDITOS_DB); /* guardar los cambios en la base de datos */
                break; /* fin case 3 */
=======
                    valid = true;
                    break;
                }
                db_info[i].activo = true;
                save_db_info(PELUDITOS_DB);
                break;
>>>>>>> 3118c7333c33e2702d8f10b39c40ccabff258da4
            default:
                valid = true; /* salir del ciclo */
                break;
        }
    } while (!valid);
    free(q_BUFF); /* liberar memoria asignada */
	free(p_DB); /* liberar memoria asignada */
    
}

void db_stats_active_owners(void)
{
    size_t activos = 0, inactivos = 0;
    if (!end()) return;

    for (i = 0; i < count; i++)
    {
        if (db_info[i].activo == true)
            activos++;
        else
            inactivos++;
    }
  
    printf("   %c %26s%-19d%c\n", /* imprimir total de entradas */
               186, "Total entradas: ", (int)count, 186);

    printf("   %c %26s%-19d%c\n", /* imprimir total de entradas activas */
        186, "Total entradas activas: ", (int)activos, 186);
        
    printf("   %c %26s%-19d%c\n", /* imprimir total de entradas inactivas */
        186, "Total entradas inactivas: ", (int)inactivos, 186);
}

void db_stats_species_count(void)
{
    size_t i, j;
    size_t especie_count[14] = {0}; /* arreglo para contar especies */
    

<<<<<<< HEAD
    if (!end())return; /* verificar si la base de datos está vacía */
=======
    if (!end())return;
>>>>>>> 3118c7333c33e2702d8f10b39c40ccabff258da4

    for (i = 0; i < count; i++)
    {
        for (j = 0; j < 14; j++)
        {
            if (buffers_isequal(limpiar_cadena(db_info[i].especie), diccionario[j]) == true) /* comparar especies */
            {
                especie_count[j]++;
                break;
            }
        }
    }

    printf("   %c   Conteo de especies en la Base de Datos:    %c\n",
               186, 186);
    for (j = 0; j < 14; j++)
    {
        printf("   %c   %-9s: %-32d%c\n", /* imprimir conteo de especies */
               186,diccionario[j], (int)especie_count[j], 186);
    }
}

void view_stats(void)
{
    clrscr();

    /*Línea superior: ╔══════════╗ -> ASCII: 201 + 205 + 187*/
    printf("   %c", 201);                          /* ╔*/
    for(i = 0; i < 46; i++) printf("%c", 205); /* ═*/
    printf("%c\n", 187);                       /* ╗*/

    /* Título*/
    printf("   %c", 186);  /* ║*/
    printf("        Estadisticas de Peluditos & Co.       ");
    printf("%c\n", 186);  /* ║*/
    /* Separador: ╠═══════╣*/
    printf("   %c", 204);/* ╠ */
    for(i = 0; i < 46; i++) printf("%c", 205); /* ═*/
    printf("%c\n", 185);/* ╣*/
    db_stats_active_owners();
    printf("   %c", 204);   /* ╠ */
    for(i = 0; i < 46; i++) printf("%c", 205);  /* ═*/
    printf("%c\n", 185);    /* ╣*/
    db_stats_species_count();
    printf("   %c", 200);                      /* ╠ */
    for(i = 0; i < 46; i++) printf("%c", 205); /* ═*/
    printf("%c\n", 188);                       /* ╣*/
    textcolor(10); /* verde*/
    printf("Presione Enter para continuar...\n");
    getch();
    textcolor(WHITE);
}

<<<<<<< HEAD
void app_database(){ /* funcion principal de la aplicacion de base de datos */
    char opcion[16]; /* cadena para almacenar la opcion del usuario */
=======
void app_database(){
    char opcion[16];
>>>>>>> 3118c7333c33e2702d8f10b39c40ccabff258da4
    textcolor(YELLOW);
    cprintf("Bienvenido a base de datos de prueba\n\r");
    cprintf("Seleccione una opcion:\n\r");
    textcolor(WHITE);
    do{
        load_db_info(PELUDITOS_DB);
        clrscr();
        mostrar_menu();
        scanf("%s", opcion);
        if (!verify_op_input(opcion, 6, 1)) {
            textcolor(LIGHTRED);
            cprintf("Error: Entrada invalida. Por favor, ingrese un numero valido.\n\r");
            textcolor(WHITE);
            getch();
            limpiar_buffer();
            continue;
        }
        printf("\n");
        switch (atoi(opcion))
        {
<<<<<<< HEAD
        case 1: agregar_nueva_entrada(); break; /* agregar nueva entrada */
        case 2: ver_db_info(); break; /* ver base de datos */
        case 3: buscar_entrada(); break; /* buscar entrada */
        case 4: modificar_entrada(); break; /* modificar entrada */
        case 5: view_stats(); break; /* ver estadisticas */
        case 6: /* salir */
=======
        case 1: agregar_nueva_entrada(); break;
        case 2: ver_db_info(); break;
        case 3: buscar_entrada(); break;
        case 4: modificar_entrada(); break;
        case 5: view_stats(); break;
        case 6:
>>>>>>> 3118c7333c33e2702d8f10b39c40ccabff258da4
            textcolor(LIGHTGREEN);
            cprintf("Saliendo del programa...\n\r");
            delay(1000);
            break;
        default:
            break;
        }
    }while(atoi(opcion) != 6);
}
#endif
