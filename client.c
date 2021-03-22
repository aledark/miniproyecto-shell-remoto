#include "leercadena.h"
#include "tcp.h"
#include "archivo.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_LENGTH_STRING 7

int main(int argc, char *argv[]) {

	int sockfd, port; 
	char *host;
	char comando[BUFSIZ];

	if (argc != 3){
		printf("Uso: %s <host> <puerto>\n", argv[0]);
		return 1;
	}

	host = argv[1];
	port = atoi(argv[2]);

	printf("Intentando conectar con <%s,%d>\n",host,port);

	sockfd = TCP_Open(Get_IP(host),port);

	while(1){
		int pid;

		//printf("Pid <%d>:", getpid());
		//Cambiar el color y estilo de fuente para el texto a continuación
		printf("\033[1m\033[33m");
		printf("prompt@miniserver:/>");
		//Reestablece los valores de color y estilo de fuente
		printf("\033[0m");

		//Lee de teclado el comando
		//bzero(comando, BUFSIZ);
		leer_de_teclado(BUFSIZ,comando);

		//Si cadena == "exit" salir del ciclo
		if (strcmp("exit", comando) == 0){
			//Inicia desconexion
			TCP_Write_String(sockfd, comando); 
			Recv_ACK(sockfd);
			TCP_Close(sockfd);
			printf("Bye... \n");
			break;
		}

		pid = fork();
		if(pid < 0){
			fprintf(stderr, "Error al crear proceso hijo\n");
			exit(1);
		}else if(pid == 0){
			//Proceso hijo
			char filename[MAX_LENGTH_STRING] = {0};

			//Envia el comando al servidor
			TCP_Write_String(sockfd, comando); 
			Recv_ACK(sockfd);

			//Espera por el nombre del archivo que almacenara la salida del comando
			TCP_Read_String(sockfd,filename,MAX_LENGTH_STRING); 
			Send_ACK(sockfd);

			//Recibe el archivo
			TCP_Recv_File(sockfd, filename);

			//Muestra en pantalla el contenido del archivo
			cat_archivo(filename);

			//Borra el archivo del lado del cliente
			borrar_archivo(filename);
			bzero(filename, MAX_LENGTH_STRING);
			break;
		}else{
			//Espera hasta que el proceso hijo termine para continuar recibiendo comandos
			wait(NULL);
		}
	}
	return 0;
}
