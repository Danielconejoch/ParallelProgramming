# Tarea 3 zippass_serial_optimized, Daniel Conejo Chevez C12243
Hipervinculo al archivo "readme.md" de la parte de diseño: [Readme.md diseño](./design/readme.md)

## Descripcion del problema resuelto
    El desarrollo de este proyecto se desarrolla partiendo de la premisa de que tenemos archivos zip con informacion la cual queremos acceder. Sin embargo no recordamos la contraseña, por lo cual hay que diseñar un codigo el cual nos permita accesar a ese archivo por medio de la fuerza bruta, metodo el cual prueba todas las contraseñas posibles y la intenta en el archivo hasta abrirlo. Cabe destacar que este metodo funciona solo en contraseñas que no tengan un limite específico de intentos, en dado caso es posible que los intentos se acaben antes que encontremos la contraseña.

### Manual de usuario
    Este proyecto incluye casos de prueba, en caso de querer correrlos habria que entrar en la carpeta zippass_serial desde la terminal y ejecutar el comando "make test" (o make clean asan test para verificar fugas de memoria) en cualquier otro caso, siga estos pasos:

    1. Utilice el comando "make" desde la terminal para generar el archivo ejecutable.
    2. Cuando tenga ese archivo, ejecute el comando .bin/zippass_serial < /tests/input.txt . Donde ese txt es el archivo con la informacion necesaria para correr el código, lo que seria diccionario, tamaño de la contraseña y path del archivo .zip.
    3. Despues de eso el programa deberia enviar de salida en la terminar el path del zip junto con la contraseña, en el caso haya sido encontrada con la información recolectada previamente.

    Cabe destacar que tambien podemos ejecutar "make run" e ingresar la informacion desde la terminal nosotros mismos.

### Creditos

    Daniel Conejo Chevez, daniel.conejochevez@ucr.ac.cr


