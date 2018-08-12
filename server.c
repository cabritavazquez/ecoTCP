#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <errno.h>

#define N 256
#define PUERTO 4888

int main (){
	
	struct sockaddr_in dirServer,dirClient;
	socklen_t longDirClient;
	int dSocket, ndSocket;
	int pid, leidos, enviados;
	char buffer[N];
	
	dSocket = socket(PF_INET,SOCK_STREAM ,0);
	
	if (dSocket < 0){
		perror("error al crear socket");
		exit(1);
	}
	
	memset(&dirServer,0,sizeof(dirServer));
	
	dirServer.sin_family = PF_INET;
	dirServer.sin_port = PUERTO;
	dirServer.sin_addr.s_addr = INADDR_ANY;
	
	if ( (bind(dSocket,(struct sockaddr*)&dirServer,sizeof(dirServer))) < 0){
		perror("error en el bind");
		close(dSocket);
		exit(1);
	}
	
	if ( (listen(dSocket,5)) < 0){
		perror("error en el listen");
		close(dSocket);
		exit(1);
	}
	
	while (1){
		longDirClient = sizeof(dirClient);
		if ( (ndSocket = accept(dSocket,(struct sockaddr*)&dirClient,&longDirClient)) < 0){
			perror("error en el accept");
			close(dSocket);
			exit(1);
		}
		printf("ECO server\n\n");
		/// a partir de ahora cerramos ambos dSockets
		pid = fork();
		if (pid == 0){ /// codigo del hijo
			while (1){
				// leemos al cliente
				leidos = read(ndSocket,buffer,N);
				if (leidos < 0){
					perror("error al leer");
					close(dSocket);
					close(ndSocket);
					exit(1);
				}
				else if (leidos == 0){
					printf("Cliente desconectado.\n");
					close(ndSocket);
					exit(1);
				}
				else {// le hacemos el eco al cliente
					enviados = write(ndSocket,buffer,leidos);
					if (enviados < 0){
						perror("error al enviar");
					}
					else {
						buffer[leidos] = '\0';
						printf("Se ha recibido e enviado la cadena: %s\n",buffer);
					}
				}
			}
		}
		else /// el padre no lo necesita
			close(ndSocket);
	}
	
	
	return 0;
}
