==== NEORETRO OS ====
== Guia de Instalacion ==

Buenas!!! Este es un tutorial rapido de implementar el sistema operativo NeoRetro OS a tu espacio de trabajo personal.

1. Para ello se debe descargar el sistema operativo a traves del repositorio de GitHub https://github.com/samxdev7/neoretro_os/tree/feature, o comunicarse con los autores para poder recibir un zip con el entorno de desarrollo del sistema operativo.

2. Una vez descargado se debe implementar en la carpeta TC20, la cual es el directorio del compilador Turbo C 2.0. Aunque este sistema operativo solo puede trabajar en el disco C:, y en la ruta C:\TC20, por lo cual se debe trabajar con el disco y ruta mencionadas.

3. Ahora para completar este proceso se deben mover todas las librerías (.h) de la carpeta library al directorio INCLUDE. Se deben mover todos los archivos binarios (.bin) al directorio BIN de TC20. Luego, se debe mover el driver SVGA256 al BIN de TC20 para desbloquear los 256 colores. Finalmente en el directorio C:\TC20 se debe crear un directorio llamado exactamente "Ficheros" que permitirán manejar ficheros de guardado como el bloc de notas, paint y bases de datos.

4. Abra su TC20, y acceda a la carpeta NeoRetro, la cual aparecerá el archivo main.c, este es el responsable de ejecutar el sistema operativo (se le recomienda no modificarlo para evitar conflictos). El siguiente paso es compilar el archivo main.c, la cual se realizara junto a las librerias .h que le dan soporte al sistema operativo.

5. Si la compilacion resulta ser un exito debe ejecutar el main.c a traves de seleccionar Run en el menu de TC20, caso contrario debe verificar si realizo los pasos anteriores de forma correcta (o verificar si hay algo mal en el TC20). Si se ejecuta el TC20 sin problema alguno ya puede utilizar el sistema operativo NeoRetro OS!!!