# Programacion paralela y concurrente Daniel Conejo

## Glosario:

### Programacion serial:
  Se refiere a la programacion que ejecuta el codigo de manera secuencial, es decir, una instruccion directamente despues de la otra, con un solo hilo.

### Programacion concurrente:
    
### Programacion paralela:

### Concurrencia de tareas:

### Paralelismo de datos:

### Hilo de ejecucion

### Indeterminismo:
	Quiere decir que el comportamiendo de la concurrencia no se puede predecir, un hilo puede ejecutar una instruccion antes que otro y nosotros no sabriamos esto con anticipacion.

### Memoria privada y compartida:
	La memoria privada se refiere a la memoria asignada a un hilo que no es accesible para otros hilos, mientras que la memoria compartida es accesible por múltiples hilos o procesos.

### Espera activa:
	Es un proceso por el cual un hilo verifica repetidamente una condición hasta que se cumpla y asi realizar las siguientes instrucciones despues de cumplir la condicion.

### Condición de carrera:
	Ocurre cuando múltiples hilos acceden a recursos compartidos de manera simultanea, lo que puede ocasionar a resultados que no se pueden predecir o errores.

### Control de concurrencia:
	Se refiere a las técnicas utilizadas para garantizar la coordinacion en contextos de concurrencia, evitando problemas como las condiciones de carrera.

### Seguridad condicional:
	Se puede definir con una caracteristica de un programa, la cual mantiene la seguridad de la concurrencia bajo una condicion especifica.

### Exclusión mutua:
	Recurso que garantiza que solo un hilo acceda a un recurso compartido a la vez para evitar condiciones de carrera.

### Semáforo:
	Un semáforo es un recurso que se utiliza para sincronizar el acceso a datos compartidos. Puede utilizarse para liberal o bloquear el acceso de hilos a cierta informacion.

### Barrera:
	Es un punto de sincronización donde varios hilos deben esperar hasta que todos los hilos hayan alcanzado ese punto antes de continuar.
### Variable de condición:

### Candado de lectura y escritura:
	Un candado de lectura y escritura es un mecanismo que permite a múltiples hilos tener acceso simultáneo a datos para lectura, pero garantiza la exclusión mutua para la escritura.
### Descomposición:
	La descomposición se refiere a dividir una tarea en partes más pequeñas y procesar esas partes en paralelo para mejorar el rendimiento.
### Mapeo:
	El mapeo se refiere a asignar o dividir tareas o datos a hilos específicos en un sistema paralelo.
### Incremento de velocidad:
	El incremento de velocidad o speedup se refiere a la mejora en el rendimiento de un programa al utilizar programación concurrente.
### Comunicación punto a punto entre procesos:
	La comunicación punto a punto implica la transferencia de datos entre dos procesos específicos.
### Comunicación colectiva entre procesos:
	La comunicación colectiva implica la interacción entre múltiples procesos para transferencia o manejo de los datos
### Reducción:
	La reducción es una operación que combina resultados de múltiples hilos o procesos en un único resultado, como la suma o el producto, para hacerlo mas rapido.