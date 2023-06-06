
Para compilar el recorrido en anchura:

$ gcc -Wall -std=c99 -osalida.out main_bfs.c List.c Queue.c


Para compilar el recorrido en profundidad:

$ gcc -Wall -std=c99 -osalida.out main.c List.c Queue.c


(Recuerda que el símbolo $ no debe ser introducido, éste sólo indica el prompt de la terminal.)

El archivo main.c incluye los dos recorridos; el de en anchura está desactivado en la función main().
