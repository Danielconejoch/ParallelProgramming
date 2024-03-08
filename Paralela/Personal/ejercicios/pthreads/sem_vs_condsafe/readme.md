# Ejercicio 16 [sem_vs_condsafe]
    Ejecute al menos tres veces los códigos de Ejemplo 7 (imprimir en orden con semáforos) y Ejemplo 8 (imprimir en orden con seguridad condicional) con tantos hilos como su sistema operativo permite. Reporte en su readme.md las tres duraciones de cada solución. Tome la menor duración de las tres corridas de cada versión. ¿Cuál solución realiza su trabajo en menor tiempo? Discuta en máximo un párrafo cuál podría ser la causa de este comportamiento.

## Resultados de los ejemplos 7 y 8

    Segun el en enunciado, debemos correr el ejercicio con la mayor cantidad de hilos que la maquina permita, para efectos de este ejercicio, usamos 32752.

### Ejemplo 7 hello_order_semaphor
    1. 1.596632402s // Menor
    2. 1.720758435s
    3. 1.713607002s

### Ejemplo 8 hello_order_cond_safe
    1. 1.360186804s 
    2. 1.205029327s // Menor
    3. 1.257894573s 

### ¿Cuál solución realiza su trabajo en menor tiempo?
    La solucion del conditionally safe realiza el trabajo en menor tiempo, esto debido a que no utilza mecanismos de espera como lo hace el semaforo o incluso el mutex.

   