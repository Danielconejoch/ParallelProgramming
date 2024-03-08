# Tarea 4 Zippass_OMP_MPI, Daniel Conejo Chevez C12243

## Descripcion del problema resuelto
    El desarrollo de este proyecto es igual a las entregas previas, solo que ahora su implementacion fue hecha en c++ y hacemos uso de OMP y MPI para abarcar el area de la concurrencia, generando un vector con todas las contrasenas y posteriormente probandolas como ya hemos hecho anteriormente.
    Cabe destacar que se utiliza mapeo dinamico ya que en la mayoria de los casos dio mejores resultados.

### Manual de usuario
    Este proyecto incluye casos de prueba, en caso de querer correrlos habria que entrar en la carpeta zippass_serial desde la terminal y ejecutar el comando "make test" (o make clean asan test para verificar fugas de memoria) en cualquier otro caso, siga estos pasos:

    1. Utilice el comando "make" desde la terminal para generar el archivo ejecutable.
    2. Cuando tenga ese archivo, ejecute el comando .bin/zippass_serial < /tests/input.txt  . Donde ese txt es el archivo con la informacion necesaria para correr el código, lo que seria diccionario, tamaño de la contraseña y path del archivo .zip.
    3. Despues de eso el programa deberia enviar de salida en la terminar el path del zip junto con la contraseña, en el caso haya sido encontrada con la información recolectada previamente.

    Cabe destacar que tambien podemos ejecutar "make run" e ingresar la informacion desde la terminal nosotros mismos.

### Creditos

    Daniel Conejo Chevez, daniel.conejochevez@ucr.ac.cr


