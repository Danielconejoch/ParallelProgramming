# Proyecto 2: Simulación de transferencia de calor

## Transferencia de calor

Se necesita una sencilla simulación por computadora que ayude a encontrar el momento de equilibro térmico de una lámina rectangular a la que se le inyecta calor constante por su borde. La lámina (en inglés, plate) corresponde a un rectángulo de dos dimensiones de un mismo material. Para efectos de la simulación, el rectángulo es dividido en R filas y C columnas ambas de igual alto y ancho h. Esto genera una matriz cuyas celdas son todas cuadradas, de ancho y alto h.

Se necesita que la solución concurrente sea invocada desde la línea de comandos con los siguientes argumentos:

1. El nombre de un archivo de trabajo (job). Es obligatorio. Si no se provee, se debe emitir un mensaje de error.

2. La cantidad de hilos de ejecución. Es opcional, y si se omite, se debe suponer la cantidad de CPUs disponibles en el sistema.

Por ejemplo, la siguiente invocación indica que se quiere realizar todas las simulaciones indicadas en el archivo de trabajo job001.txt con 16 hilos de ejecución.

~~~text
bin/heatsim jobs/job001.txt 16
~~~

El equipo desarrollará una solución concurrente en C++ de la simulación de calor usando la tecnología de paralelismo de datos OpenMP.

## Construcción de la solución

### Primera versión

Para la primera versión del proyecto, el equipo dearrolló una solución concurrente en C++ del problema usando la tecnología de paralelismo de datos OpenMP.

En primer lugar el equipo resolvió el problema con una solución serial enfocada en corrección y funcionalidad del mismo para luego proceder a la paralelización del mismo.

Una vez construida y funcional, se realizó la paralelización buscando el incremento en el desempeño usando OpenMP.

Finalmente se realizó una medición de tiempos para corroborar si verdaderamente hubo una optimización.

### Segunda versión

La segunda entrega del proyecto consistió en mejorar el rendimiento de la primera versión agregando distribución mediante la tecnología MPI. Esta versión es un avance que incrementó y mejoró el código existente.

Para esta nueva versión, se agregó a la solución concurrente (OpenMP) la capacidad de distribuir trabajo que incremente el desempeño mediante la tecnología MPI.

Finalmente, se realizaron de nuevo mediciones de rendimiento entre las versiones implementadas hasta el momento. Estas se realizaron de la siguiente manera:

1. Concurrente OpenMP un proceso con 8 hilos en un solo nodo.

2. Distribuida MPI con 24 procesos de 1 hilo en 3 nodos.

3. Híbrida OpenMP+MPI con 3 procesos de 8 hilos en 3 nodos.

[Aqui](Proyecto-2/report/README.md) puede ver las gráficas de comparación, además de un análisis a las deferencias entre los rendimientos obtenidos.

## Manual de Usuario

### Compilacion

1. Asegurese de tener instalado un compilador de C++ compatible instalado (probado con GCC) y tener instalada la librería para usar MPI
2. Abra un terminal y navegue al directorio donde se encuentra el archivo de codigo fuente. Deberia de seguir una ruta similar a la siguiente:

~~~text
../../los-paraplejicos-23b/Proyecto-2
~~~

3. Para compilar el programa ejecute los siguientes comandos desde la terminal:

~~~text
make clean; make
~~~

El comando `make clean` realiza una limpieza completa de todas las carpetas y archivos creadas por algun tipo de compilacion previa

Al finalizar la compilacion, la terminal no deberia de desplegar ningun tipo de mensaje de error.

### Ejecucion (Versión Híbrida (default))

1. Si el codigo compila de manera correcta, entonces para poner en ejecucion el programa debera de colocar el siguiente comando en su terminal:

~~~text
mpiexec -np 3 ./bin/Proyecto-2 tests/jobs/job###/job###.txt 8
~~~

En este ejemplo, la primer parte del argumento hace referencia al llamado de la funcion para correr la librería de MPI. El valor que le sigue (3, en este caso) hace referencia al número de procesos de ejecución para MPI.

Lo siguiente es la ruta del ejecutable, seguido de la ruta donde se encuentra el archivo de texto que se va a leer y que contiene todos los parámetros y plates de calor a probar. Cambie los ### por el número de job específico que desea probar.

Finalmente el último valor (8 para este ejemplo) hace referencia al número de hilos con los que trabajará Omp. Si este numero no se indica, el programa toma como numero de conexiones tantas como CPUs hay disponibles en la maquina.

2. Una vez el comando anterior sea ejecutado, el programa correrá

### Ejecución (Versión MPI)

1. Para esta versión se debe hacer un cambio dentro de propio código. Para ello abra el archivo src/Ordenador.cpp y modifique la siguiente línea de código:

~~~text
68   int k = heatIterations(datas[i], datas[i].matrix);
~~~

En la línea 68 del código, cambie el nombre de la función heatIterations(...) por hearIterations_serial(...). Debería verse de la siguiente manera:

~~~text
68   int k = heatIterations_serial(datas[i], datas[i].matrix);
~~~

2. Salve el archivo y vuelva a realizar el proceso de compilación

3. Si el codigo compila de manera correcta, entonces para poner en ejecucion el programa debera de colocar el siguiente comando en su terminal:

~~~text
mpiexec -np 3 ./bin/Proyecto-2 tests/jobs/job###/job###.txt
~~~

Note que la línea de ejecución es similar a la de la versión por default (Híbrida), por lo tanto aplican las mismas notas hechas anteriormente. La única diferencia es que no se requiere la cantidad de hilos como último argumento.

### Funcionalidad

Una vez el programa haya terminado de ejecutarse, aparecerá el siguiente mensaje en la terminal:

~~~text
Datos guardados exitosamente en tests/report
~~~

Este mensaje indica en donde se guardan los archivos con el reporte de cada job, junto a las plates resultantes del proceso realizado por el programa en la simulación

## Creditos

Curso: CI-0117b Programacion Paralela y Concurrente

Integrantes:
- Daniel Conejo Chevez, C12243
- Dariel Alvarez Gonzalez, C20413
- Marco Antonio Angulo Rodriguez, C10458
- Werner Naranjo Navarro, C15464
