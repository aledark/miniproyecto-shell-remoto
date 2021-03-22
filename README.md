# Miniproyecto Shell Remoto

Integrantes: Maria Alejandra Aguiar - 2001240 y Johan Sneyder Mera - 2004792

En este repositorio se encuentran los codigos en C que conforman la implementación del shell remoto que sigue el esquema cliente/servidor. La siguiente gráfica muestra la interacción entre el shell cliente y el servidor. 

<img src="figures/MiniShellRemoto.png" alt="Mini Shell Remoto Cliente/Servidor" width="650"/>

Al lado derecho de la gráfica se observa el servidor quien está esperando por peticiones de un cliente.
Este servidor espera recibir comandos a través de la red de parte de un proceso cliente.

El cliente es un proceso que está en espera de leer por teclado un comando y una vez recibido ese comando, se envía al servidor.
El cliente creará un hijo quien se encarga de interactuar con el servidor, enviar el comando y esperar por la respuesta del comando.
Una vez este proceso hijo reciba la respuesta del servidor, imprime la respuesta y termina su ejecución.
El cliente (padre) volverá a presentar el *prompt* (`>`) y quedará en espera del siguiente comando por parte del usuario.

El servidor al recibir una solicitud a través de la red, crea un proceso hijo quien se encargará de procesar dicha solicitud. 
El proceso hijo recibe la solicitud y la ejecuta con la función `execvp` o alguna de las funciones miembros de la familia `exev*`.
El proceso (padre) toma la salida del proceso hijo y la envía al lado del cliente.

---

A continuación se da una descripción de diferentes funciones que permitieron concentrarse en la lógica de la aplicación.

## `archivo.c` y `archivo.h`

Estos archivos contienen la definición de las funciones ([`archivo.h`](archivo.h)) y su correspondiente implementación ([`archivo.c`](archivo.c)).
Al interior de [`archivo.h`](archivo.h) se encuentra una descripción corta de las funciones y de los parámetros que estas reciben. 

## `leercadena.c` y `leercadena.h`

Estos archivos contienen la definición de las funciones ([`leercadena.h`](leercadena.h)) y su correspondiente implementación ([`leercadena.c`](leercadena.c)).
Al interior de [`leercadena.h`](leercadena.h) se encuntra una descripción corta de las funciones y de los parámetros que estas reciben.

## `tcp.c` y `tcp.h`

Estos archivos contienen la definición de las funciones ([`tcp.h`](tcp.h)) y su correspondiente implementación ([`tcp.c`](tcp.c)).
Al interior de [`tcp.h`](tcp.h) se encuntra una descripción corta de las funciones y de los parámetros que estas reciben. 

Estas funciones se usaron en el desarrollo del shell remoto cliente/servidor.

### `client` y `server` 

Para ver en operación el proyecto se debe ejecutar el comando `make client_server` que se encarga de compilar los archivos objeto que requiere este aplicativo (`tcp.o`), (`archivo.o`), (`leercadena.o`), el cliente (`client`) y el servidor (`server`). 

Para ejecutar el servidor ejecute el comando `./server 12358`. 

En una carpeta duplicada, abra otra terminal y en ella ejecute el cliente de la siguiente manera `./client localhost 12358`.

El cliente envía un comando y este es recibida por el servidor, el cual devuelve al cliente la respuesta del comando para que este pueda imprimirlo en su pantalla.

Para limpiar los archivos compilados, ejecute el comando `make clean`.


<!--
## Elementos que quedan pendientes

* Manejar los errores que se puedan presentar a la hora de ejecutar un comando al lado del servidor. El cliente debe ver ese error.

* Se debe implementar el cambio de directorio emitido desde el cliente y este se vea reflejado en el servidor. Es decir, que se posibilite el cambio de directorio.

* Brindar soporte para la ejecución de comandos compuestos y unidos por el *pipe* (`|`).

* Brindar soporte para la redirección de la salida de la ejecución de un comando. Esta redirección deberá residir al lado del cliente. Quizá se requiera un preprocesamiento del lado del cliente de modo que se haga conciencia que la salida no irá a pantalla sino a un archivo.
-->

