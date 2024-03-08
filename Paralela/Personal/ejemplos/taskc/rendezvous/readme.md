### rendezvous1
    Usa dos semáforos para asegurar que dos threads han llegado a cierto punto. En esta solución, los dos threads avisan apenas hayan terminado de ejecutar las instrucciones a1 o b1. En esta solución el nombre del semáforo indica el evento ocurrido, como a1_ready, y es lo que se prefiere. Otro nombre válido es indicar el manejador del evento o acción que debe realizarse cuando la señal ocurre, como can_run_b2.

### rendezvous2
    En esta versión un thread primero espera y el otro avisa. Es una solución correcta aunque ligeramente menos eficiente que la versión 1. Hay dos variantes equivalentes: a) El hilo a primero avisa y luego espera y el hilo b primero espera y luego avisa. b) El hilo a primero espera y luego avisa y el hilo b primero avisa y luego espera.

### rendezvous3
    En esta versión ambos threads esperan a que el otro haya terminado de ejecutar su instrucción. No es una solución al problema porque genera un bloqueo mutuo (deadlock).
