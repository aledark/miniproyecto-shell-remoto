all: client_server

# ---
# Para compilar el protocolo TCP
#

tcp.o:
	gcc -c tcp.c
	
# ---

# ---
# Para compilar leercadena
#

leercadena.o:
	gcc -c leercadena.c

# ---

# ---
# Para compilar archivo
#

archivo.o:
	gcc -c archivo.c

# ---
# Para compilar el cliente
#
client: leercadena.o archivo.o tcp.o
	gcc -o client client.c leercadena.o archivo.o tcp.o
# ---
# Para compilar el servidor
#
server: leercadena.o archivo.o tcp.o
	gcc -o server server.c leercadena.o archivo.o tcp.o

client_server: client server

clean:
	rm -f tcp.o client server leercadena.o archivo.o
