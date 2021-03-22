#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <assert.h>
#include "tcp.h"
#include "leercadena.h"
#include "archivo.h"

#define MAX_LENGTH_STRING 7

int main(int argc, char* argv[]) 
{ 
	int socket, connfd;  
	int port;
	char command[BUFSIZ];
	char filename[MAX_LENGTH_STRING];

	if (argc != 2) {
		printf("Uso: %s <puerto>\n",argv[0]);
		return 1;
	}
	
	port = atoi(argv[1]);

	socket = TCP_Server_Open(port);
	connfd = TCP_Accept(socket);

    while(1){
        int pid, pid_temp;
		char **vector;
		int filefd;

		bzero(command,BUFSIZ);
		bzero(filename,MAX_LENGTH_STRING);
		// Recibo el comando del cliente
		TCP_Read_String(connfd, command, BUFSIZ);
		Send_ACK(connfd);
		printf("Se leyo %s\n",command);

		//Si cadena == "exit" salir del ciclo
		if (strcmp("exit", command) == 0){
			//Inicia desconexion
			TCP_Close(socket);
			printf("Bye... \n");
			break;
		}

		//Convierto el (string)comando a vector
		vector = de_cadena_a_vector(command);

		//Se genera un nombre de archivo
		generar_nombre_archivo(MAX_LENGTH_STRING, filename);

		pid = fork();
		if(pid < 0){
			fprintf(stderr, "Error al crear proceso hijo\n");
			exit(1);
		}else if(pid == 0){
			//Proceso hijo  
			
			//Re-direcciona la salida estandar (terminal) a un archivo
			//close(STDOUT_FILENO);
			filefd = open(filename, O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU);
			assert(filefd != -1);
			dup2(filefd, STDOUT_FILENO);

			//Ejecuta el comando con execvp
			execvp(vector[0], vector);
			perror("Respuesta de execvp() no esperada");
			exit(EXIT_FAILURE);
		}else{
			//Espera hasta que el proceso hijo termine de guardar la salida del comando 
			//en el archivo
			wait(NULL);

			//Cierra el archivo
			close(filefd);
			
			//Enviando el nombre del archivo que almacenara la salida del comando
			TCP_Write_String(connfd, filename);
			Recv_ACK(connfd);

			//Envia el archivo
			TCP_Send_File(connfd,filename);

			//Borra el archivo del lado del servidor
			borrar_archivo(filename);
		}
    }
	return 0;
} 