/*
    FILEHND.C
    Manejo de archivos binarios para propietarios y mascotas
    Proyecto de base de datos simple
    
    Moises
 */

 /*
    =======================================
    Descripcion del modulo
    =========================================
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
    =========================================
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

/*
    =======================================
    Importacion de librerias propias
    =========================================
*/
#include "VALID.H"

/*
    =======================================
    Definicion de Constantes
    =========================================
*/

/* 
    convencion para su facil uso de true y false al igual que el tipo de datos bool
    ya que en C no existe por defecto.
*/
typedef int bool; 
#define true 1
#define false 0

#define ID_BASE 1000 /* ID base para propietarios y mascotas */

/*
    =======================================
    Definicion de Estructuras
    =========================================
*/


/* 
    estructura para usarse con funciones que retornen un int, codigos de errores comunes
    al manipular archivos.

    nota: los codigos de error son negativos para diferenciarlos de los posibles
    ademas, evitar usar estos codigos en funciones de tipo bool que retornan true o false.
*/
typedef enum {
    FILE_NOT_FOUND = -1, /* archivo no encontrado */
    FILE_READ_ERROR = -2, /* error al leer el archivo */
    FILE_WRITE_ERROR = -3, /* error al escribir en el archivo */
    FILE_BYTES_NOT_FOUNDED = -4 /* los bytes del archivo no coinciden con el tamaño esperado */
}File_states_t;
    
/* 
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
}Consts_vet_info_t;

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
}Formulario_Vet_t;

/*
    =======================================
    Declaracion de variables globales
    =========================================
*/

/*
    ruta de los archivos binarios para propietarios y mascotas
    en variables constantes para evitar errores de tipeo.
    puedes cambiar las rutas segun tu estructura de carpetas.
    esta es la estructura de carpetas recomendada:
    C:.
        TC20
            Ficheros
                ENTRYS_COUNT_PATH.count
                PELUDITOS_DB.bin
*/
const char ENTRYS_COUNT_PATH[40] = "C:\\TC20\\BIN\\Tot_entr.count";
const char PELUDITOS_DB[40] = "C:\\TC20\\BIN\\Main_DB.bin";

/*
    definimos una estructura global de tipo Formulario_Vet_t
    y la inicializamos con {0} para reservar espacio en memoria.
*/
Formulario_Vet_t formulario = {0}; /* usado para hacer append a la DB */
Formulario_Vet_t *db_info = NULL; /* usado para cargar propietarios desde la DB */

/* 
    variables globales usadas para funcion de load_db_info, global para poder ser usada en
    otras funciones que necesiten accesso a informacion de la db
*/
size_t size, count, i;

/*
    =======================================
    Declaracion de Prototipos
    =========================================
*/
bool verify_loaded_DB(void); /* Verificar si la base de datos ha sido cargada previamente */
bool verify_entry_iscorrect(Formulario_Vet_t *data); /* Verificar si los datos del formulario son correctos */
char* limpiar_cadena(char *cadena); /* limpia la cadena de espacios y convierte la cadena en minusculas */
bool buffers_isequal(const char *buf1, const char *buf2); /* compara dos buffers de caracteres */
bool save_new_entry(const char *file, Formulario_Vet_t *data); /* guardar en ficheros binarios la informacion de propietarios */
void load_db_info(const char *file); /* cargar desde ficheros binarios la informacion de propietarios */
void ver_db_info(void); /* ver la informacion cargada desde la base de datos */
bool save_db_info(const char *file); /* guardar la informacion cargada en memoria de vuelta a la base de datos */
void mostrar_menu(void);
void agregar_nueva_entrada(void); /* funcion principal para agregar propietarios, esta funcion usa la funcion save_new_entry */
void buscar_entrada(void); /* funcion para buscar propietarios por ID o Nombre */
void modificar_entrada(void); /* funcion para modificar una entrada existente en la base de datos */
long tamano_archivo(const char*); /* funcion para obtener el tamaño de un archivo en bytes */

/* 
Files_states_t get_next_owner_id(const char*, int);
Files_states_t get_next_pet_id(const char*, int);
*/

int main(){
    int opcion;
    textcolor(11);
    cprintf("Bienvenido a base de datos de prueba\n\r");
    cprintf("Seleccione una opcion:\n\r");
    textcolor(7);
    do{
        load_db_info(PELUDITOS_DB);
        clrscr();
        mostrar_menu();
        scanf("%d", &opcion);
        printf("\n");
        switch (opcion)
        {
        case 1:
            agregar_nueva_entrada();
            break;
        case 2:
            ver_db_info();
            break;
        case 3:
            buscar_entrada();
            break;
        case 4:
            modificar_entrada();
            break;
        case 5:
            printf("Saliendo del programa...\n");
            break;
        default:
            break;
        }
    }while(opcion != 5);

    return 0;
}

bool verify_loaded_DB(void)
{
    if (db_info == NULL) return false;

    return true;
}

char* limpiar_cadena(char *cadena)
{
    int len = 0, i, j;
    char *p;
    for (i = 0; cadena[i] != '\0'; i++)
    {
        if(isalpha(cadena[i])) len++;
    }

    p = (char*)malloc((len + 1) * sizeof(char));
    if (!p) 
    {
        printf("Error al asignar memoria para limpiar cadena...\n");
        getch();
        return NULL;
    }

    j = 0;
    for (i = 0; cadena[i] != '\0'; i++)
    {
        if (isalpha(cadena[i]))
            p[j++] = tolower((unsigned char)cadena[i]);
    }
    p[j] = '\0'; /* poner el caracter '\0' para indicar final de cadena */
    return p;
}

bool buffers_isequal(const char *buf1, const char *buf2)
{
    while (*buf1 && *buf2)
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
{
    FILE *p = fopen(file, "ab");
    if (!p) return false;
    if (fwrite(data, sizeof(Formulario_Vet_t), 1, p) != 1)
    {
        fclose(p);
        return false;
    }
    if(fclose(p) == EOF) return false;
    return true;
}

bool save_db_info(const char *file)
{
    FILE *p;
    if (!verify_loaded_DB()) /* verificar que la base de datos ha sido cargada previamente */
    {
        textcolor(12);
        cprintf("Error: La base de datos no ha sido cargada. No se puede guardar.\n\r");
        textcolor(7);
        return false;
    }

    p = fopen(file, "wb");
    if (!p)
    {
        printf("Error: No se pudo abrir el archivo para guardar la base de datos.\n");
        return false;
    }

    for (i = 0; i < count; i++)
    {
        if (fwrite(&db_info[i], sizeof(Formulario_Vet_t), 1, p) != 1)
        {
            fclose(p);
            printf("Error al escribir en la base de datos.\n");
            return false;
        }
    }

    if (fclose(p) == EOF)
    {
        printf("Error al cerrar el archivo de la base de datos.\n");
        return false;
    }

    return true;
}


long tamano_archivo(const char *file_address)
{
    long size;

    FILE *p = fopen(file_address, "rb");
    if (!p) return -1;

    fseek(p, 0, SEEK_END);
    size = ftell(p);
    fclose(p);
    return size;
}

/* ============================================================================================

    los numeros de registro o id, empiezan desde el numero 1000, es decir, 
    el primer registro de la base de datos que vendria a ser el 1 empezara en 1000.
    
    es asi entonces que 1 = 1000

    esto sera asi cada vez que esta funcion no encuentre el archivo "total_entrys.count"
    en su respectivo PATH(ruta o direccion del archivo).
    
    NOTA: El PATH puede ser cambiado para que se cree en la ruta que mas sea conveniente.
    si se desea cambiar el PATH solamente se cambia en la variable constante "ENTRYS_COUNT_PATH"
    agregando la nueva ruta donde el archivo sera leido/(creado si no existe)

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
        return FILE_READ_ERROR; 
    }
    
    bytes = (size_t)ftell(f); /* obtener tamano en bytes */

    if (bytes < 0) 
    { 
        fclose(f);
        return FILE_READ_ERROR;
    }

    /* Archivo vacío → primer ID base */
    if (bytes == 0) 
    {
        fclose(f);
        return ID_BASE;
    }

    /* Debe ser múltiplo exacto del tamaño del registro */
    if (bytes % sizeof(Formulario_Vet_t) != 0) 
    {
        fclose(f);
        return FILE_BYTES_NOT_FOUNDED;
    }

    /* Posicionarse al inicio del ÚLTIMO registro */
    count  = bytes / sizeof(Formulario_Vet_t);
    
    offset = (count - 1) * sizeof(Formulario_Vet_t);
    
    if (fseek(f, offset, SEEK_SET) != 0)
    {
        fclose(f);
        return FILE_READ_ERROR;
    }

    /* Leer el último registro y cerrar */
    if (fread(&ultimo, sizeof(ultimo), 1, f) != 1) 
    {
        fclose(f);
        return FILE_READ_ERROR;
    }
    fclose(f);

    return ultimo.id + 1;
}

void load_db_info(const char *file)
{
    FILE *p = fopen(file, "rb"); /* abrimos el archivo binario en modo lectura */
    
    if (!p)
    {
        clrscr();
        printf("Error: No se pudo abrir el archivo de la base de datos.\n");
        printf("Asegurese de que el archivo exista en la ruta especificada.\n");
        printf("Ruta del archivo: %s\n\n", file);
        printf("Si es la primera vez,\nagregue una entrada y la base de datos se creara automaticamente.\n");
        printf("Presione Enter para continuar...\n");
        getch();
        return;
    } 

    fseek(p, 0, SEEK_END); /* movemos el puntero al final del archivo binario */
    size = (size_t)ftell(p); /* obtenemos el tamano en bytes del archivo binario */
    fseek(p, 0, SEEK_SET); /* regresamos el puntero al inicio del archivo binario */

    if (size == 0) /* si ftell retorno un valor de 0 significa que el archivo binario esta vacio */
    {
        fclose(p); /* cerramos el archivo */
        printf("La base de datos esta vacia.\n");
        getch();
        return;
    }

    count = size / sizeof(Formulario_Vet_t); /* tamano completo del archivo / el tamnano de la estructura almacenada dentro */
    db_info = (Formulario_Vet_t*)malloc(size); /* asignar memoria para cargar los datos de la DB en memoria para su manipulacion */
    if (!db_info) /* si malloc falla alertar error */
    {
        fclose(p);
        printf("Error de memoria\n");
        getch();
        return;
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
            return;
        }
    }

    fclose(p);
    printf("Base de datos cargada con exito!.\n");
    printf("Se han cargado %d entradas desde la base de datos!\n\n", (int)count);
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
    if (!verify_loaded_DB())
    {
        printf("Error: La base de datos no ha sido cargada. Por favor, carguela primero.\n");
        getch();
        return;
    }

    clrscr();
    printf("Mostrando propietarios ACTIVOS de la base de datos:\n", (int)count);
    for (i = 0; i < count; i++)
    {
        if (db_info[i].activo == false)
            continue; /* saltar registros inactivos */
        
        printf("-------------------------------\n");
        printf("ID: %d\n", db_info[i].id);
        printf("Nombre: %s", db_info[i].nombre_propietario);
        printf("Direccion: %s", db_info[i].direccion);
        printf("Telefono: %s", db_info[i].telefono);
        printf("Nombre Mascota: %s", db_info[i].nombre_mascota);
        printf("Especie: %s\n", db_info[i].especie);
        printf("--------------------------------\n");
        textcolor(10);
        cprintf("Entero para continuar...\n\r");
        textcolor(7);
        getch();
    }
    printf("Fin de la base de datos. Presione Enter para continuar...\n");
    getch();
}

bool verify_entry_iscorrect(Formulario_Vet_t *data)
{
    size_t i, size;
    char *np, *dir, *tel, *nm, *esp;
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
        strlen(data->direccion) >= MAX_ADRESS ||
        strlen(data->telefono) >= MAX_PHONE ||
        strlen(data->nombre_mascota) >= MAX_PET_NAME ||
        strlen(data->especie) >= MAX_PET_SPECIE
        )
    {
        return false; /* algun campo obligatorio excede el maximo permitido */
    }

    np = data->nombre_propietario;
    dir = data->direccion;
    tel = data->telefono;
    nm = data->nombre_mascota;
    esp = data->especie;

    size = strlen(np);
    for (i = 0; i < size; i++)
    {
        if (np[i] == '\n')
            continue;

        if (!isalpha((unsigned char)np[i]) && np[i] != ' ' && np[i] != '.')
            return false; /* nombre propietario contiene caracteres invalidos */
    }

    size = strlen(dir);
    for (i = 0; i < size; i++)
    {
        if (dir[i] == '\n')
            continue;

        if (!isalpha((unsigned char)dir[i]) && dir[i] != ' ' && dir[i] != ',' && dir[i] != '.' &&
            dir[i] != '#' && dir[i] != '-' && dir[i] != '/' && !isdigit((unsigned char)dir[i]) && dir[i] != '(' && dir[i] != ')')
            return false; /* direccion contiene caracteres invalidos */
    }

    size = strlen(tel);
    for (i = 0; i < size; i++)
    {
        if (tel[i] == '\n')
            continue;

        if (!isdigit((unsigned char)tel[i]) && tel[i] != '+' && tel[i] != '-' && tel[i] != ' '
            && tel[i] != '(' && tel[i] != ')')
            return false; /* telefono contiene caracteres invalidos */
    }

    size = strlen(nm);
    for (i = 0; i < size; i++)
    {
        if (nm[i] == '\n')
            continue;

        if (!isalpha((unsigned char)nm[i]) && nm[i] != ' ')
            return false; /* nombre mascota contiene caracteres invalidos */
    }

    size = strlen(esp);
    for (i = 0; i < size; i++)
    {
        if (esp[i] == '\n')
            continue;

        if (!isalpha((unsigned char)esp[i]) && esp[i] != ' ' && esp[i] != '.')
            return false; /* especie contiene caracteres invalidos */
    }

    return true; /* todos los campos son correctos */
}

void buscar_entrada(void)
{
    int i;
    char *p_DB, *q_BUFF; /* p_DB(puntero a Nombres en estructura), p_BUFF(puntero a cadena introducida por user) */
    char busqueda[50]; /* cadena que guarda el input del usuario */
    if (!verify_loaded_DB())
    {
        printf("Error: La base de datos no ha sido cargada. Por favor, carguela primero.\n");
        getch();
        return;
    }
    textcolor(9);
    cprintf("Ingrese el [Nombre] del propietario a buscar: ");
    textcolor(7);
    limpiar_buffer();
    fgets(busqueda, sizeof(busqueda), stdin);

    q_BUFF = limpiar_cadena(busqueda);
    

    for (i = 0; i < count; i++)
    {
        p_DB = limpiar_cadena(db_info[i].nombre_propietario);
        if (buffers_isequal(p_DB, q_BUFF) == true)
        {
            printf("----- Entrada Encontrada -----\n");
            printf("ID: %d\n", db_info[i].id);
            printf("Nombre: %s", db_info[i].nombre_propietario);
            printf("Direccion: %s", db_info[i].direccion);
            printf("Telefono: %s", db_info[i].telefono);
            printf("Nombre Mascota: %s", db_info[i].nombre_mascota);
            printf("Especie: %s\n", db_info[i].especie);
            printf("------------------------------\n");
            free(p_DB);
            free(q_BUFF);
            textcolor(10);
            cprintf("Entero para continuar...\n\r");
            textcolor(7);
            getch();
            return;
        }
    }
    free(q_BUFF);
    free(p_DB);
    textcolor(12);
    cprintf("No se encontro ninguna entrada con ese nombre.\n\r");
    textcolor(7);
    getch();
}

void mostrar_menu(void){

    gotoxy(10,3);
    textcolor(11);
    cprintf("===== Menu Principal =====\n\r");
    textcolor(7);
    textcolor(7);
    cprintf("1. Agregar nueva entrada\n\r");
    cprintf("2. Ver Base de datos (Peluditos & CO.)\n\r");
    cprintf("3. Buscar informacion de propietario por nombre\n\r");
    cprintf("4. Modificar registro de la base de datos (Peluditos & CO.)\n\r");
    cprintf("5. Salir\n\r\n");
    cprintf("Elija una opcion: ");
}

void agregar_nueva_entrada(void)
{
    bool valid = false;
    clrscr();
    do
    {
        
        limpiar_buffer();
        textcolor(9);
        cprintf("Ingrese nombre del propietario: ");
        fgets(formulario.nombre_propietario, MAX_OWNER_NAME, stdin);
        cprintf("\n\rIngrese direccion del propietario: ");
        fgets(formulario.direccion, MAX_ADRESS, stdin);
        cprintf("\n\rIngrese telefono del propietario: ");
        fgets(formulario.telefono, MAX_PHONE, stdin);
        cprintf("\n\rIngrese el nombre de la mascota: ");
        fgets(formulario.nombre_mascota, MAX_PET_NAME, stdin);
        cprintf("\n\rIngrese la especie de la mascota: ");
        fgets(formulario.especie, MAX_PET_SPECIE, stdin);
        textcolor(7);

        if (!verify_entry_iscorrect(&formulario))
        {
            clrscr();
            textcolor(12);
            cprintf("Error: Algunos campos contienen caracteres invalidos.\n\rTodos los campos son requeridos para continuar.\n\r"); 
            cprintf("Por favor, intente de nuevo.\n\r");
            textcolor(7);
        }

        else valid = true;
    }while (!valid);
    formulario.id = get_next_entry_id();
    formulario.activo = true;
    if (save_new_entry(PELUDITOS_DB, &formulario))
    {
        textcolor(10);
        cprintf("Entrada guardada con exito en la base de datos!\n\r");
        textcolor(7);
    }
    else
        printf("Error al guardar la informacion del propietario.\n");
	getch();
}

void modificar_entrada(void)
{
    int i, opcion;
    bool valid, valid2;
    char *p_DB, *q_BUFF; /* p_DB(puntero a Nombres en estructura), p_BUFF(puntero a cadena introducida por user) */
    char busqueda[50]; /* cadena que guarda el input del usuario */
    if (!verify_loaded_DB())
    {
        textcolor(12);
        cprintf("Error: La base de datos no ha sido cargada. Por favor, carguela primero.\n\r");
        textcolor(7);
        getch();
        return;
    }
    textcolor(9);
    cprintf("Ingrese el [Nombre] del propietario a modificar: ");
    textcolor(7);
    limpiar_buffer();
    fgets(busqueda, sizeof(busqueda), stdin);

    q_BUFF = limpiar_cadena(busqueda);
    

    for (i = 0; i < count; i++)
    {
        p_DB = limpiar_cadena(db_info[i].nombre_propietario);
        if (buffers_isequal(p_DB, q_BUFF) == true)
            break; /* Si hay match termina la busqueda (Ineficiente si hay mas de un registro con el mismo nombre) */
    }

    if (i == count)
    {
        textcolor(12);
        cprintf("No se encontro ninguna entrada con ese nombre.\n\r");
        textcolor(7);
        getch();
        free(q_BUFF);
        free(p_DB);
        return;
    }
    
    valid = false;
    do
    {
        clrscr();
        textcolor(10);
        cprintf("----- Entrada Encontrada -----\n\r");
        textcolor(7);
        printf("Propietario: %s", db_info[i].nombre_propietario);
        printf("ID: %d\n", db_info[i].id);
        if (db_info[i].activo == false) 
            printf("Estado: INACTIVO\n");
        else 
            printf("Estado: ACTIVO\n");
        
        printf("\n");
        printf("------------------------------\n");
        printf("A continuacion elija la opcion segun la gestion que desea hacer:\n");
        printf("(1) Modificar datos del registro.\n");
        printf("(2) Eliminar datos de la base de datos de PELUDITOS & CO.\n");
        printf("(3) Cambiar estado a ACTIVO (Si el registro esta inactivo).\n");
        printf("(4) Cancelar/Salir.\n");
        printf("Elija una opcion: ");
        scanf("%d", &opcion);
        printf("\n");
        switch (opcion)
        {
        case 1:
            valid2 = false;
            clrscr();
            do
            {
                
                limpiar_buffer();
                textcolor(9);
                cprintf("Ingrese nombre del propietario: ");
                fgets(db_info[i].nombre_propietario, MAX_OWNER_NAME, stdin);
                cprintf("\n\rIngrese direccion del propietario: ");
                fgets(db_info[i].direccion, MAX_ADRESS, stdin);
                cprintf("\n\rIngrese telefono del propietario: ");
                fgets(db_info[i].telefono, MAX_PHONE, stdin);
                cprintf("\n\rIngrese el nombre de la mascota: ");
                fgets(db_info[i].nombre_mascota, MAX_PET_NAME, stdin);
                cprintf("\n\rIngrese la especie de la mascota: ");
                fgets(db_info[i].especie, MAX_PET_SPECIE, stdin);
                
                if (!verify_entry_iscorrect(&db_info[i]))
                {
                    clrscr();
                    textcolor(12);
                    cprintf("Error: Algunos campos contienen caracteres invalidos.\n\rTodos los campos son requeridos para continuar.\n\r"); 
                    cprintf("Por favor, intente de nuevo.\n\r");
                    textcolor(7);
                }
                else valid2 = true;
            }while (!valid2);
            if (save_db_info(PELUDITOS_DB))
            {
                textcolor(10);
                cprintf("Registro actualizado con exito en la base de datos!\n\r");
                textcolor(7);
            }
            else
            {
                textcolor(12);
                cprintf("Error al modificar el registro de la base de datos.\n\r");
                textcolor(7);
            }
            getch();
            valid = true;
            break;
        case 2:
            if (db_info[i].activo == false)
            {
                textcolor(12);
                printf("El registro ya esta INACTIVO. No se realizaron cambios.\n\r");
                textcolor(7);
                getch();
                valid = true;
                break;
            }
            db_info[i].activo = false;
            if (save_db_info(PELUDITOS_DB))
                printf("Registro eliminadoo de la base de datos!\n");
            else
                textcolor(12);
                cprintf("Error al eliminar el registro de la base de datos.\n\r");
                textcolor(7);
            getch();
            valid = true;
            break;
        case 3:
            if (db_info[i].activo == true)
            {
                textcolor(12);
                cprintf("El registro ya esta ACTIVO. No se realizaron cambios.\n\r");
                textcolor(7);
                getch();
                valid = true;
                break;
            }
            db_info[i].activo = true;
            if (save_db_info(PELUDITOS_DB))
            {
                textcolor(10);
                cprintf("Exito al reactivar el estado del propietario en la base de datos!\n\r");
                textcolor(7);
            }
            else
            {
                textcolor(12);
                cprintf("Error al cambiar el estado del pripietario.\n\r");
                textcolor(7);
                getch();
                valid = true;
            }    
            break;
        default:
            valid = true;
            break;
        }
    } while (!valid);
    free(q_BUFF);
	free(p_DB);
    
}

