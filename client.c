#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <errno.h>

#define N 256
#define PUERTO 4888

int main() {
	
	struct sockaddr_in dirServer;
	int dSocket, escritos,leidos;
	char buffer[N];
	
	dSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (dSocket < 0){
		perror("error al crear el socket");
		exit(1);
	}
	
	memset(&dirServer,0,sizeof(dirServer));
	
	dirServer.sin_family = PF_INET;
	dirServer.sin_addr.s_addr = INADDR_ANY;
	dirServer.sin_port = PUERTO;
	
	if ( (connect(dSocket,(struct sockaddr*)&dirServer,sizeof(dirServer))) < 0){
		perror("error en el connect");
		close(dSocket);
		exit(1);
	}
	
	printf ("Welcome to ECO clients. . .\n\n");
	while (1){
		
		leidos = read(0,buffer,N);
		if (leidos < 0){
			perror("error al leer");
			close(dSocket);
			exit(1);
		}
		else if (leidos == 0){
			printf("cliente desconectado\n");
			exit(1);
		}
		buffer[leidos] = '\0';
		escritos = write(dSocket,buffer,sizeof(buffer));
		if (escritos < 0){
			perror("error al enviar");
			close(dSocket);
			exit(1);
		}
		else if (escritos == 0){
			printf("cliente desconectado\n");
			exit(1);
		}
		leidos = read(dSocket,buffer,N);
		if (leidos < 0){
			perror("error al leer eco");
			close(dSocket);
			exit(1);
		}
		else if (leidos == 0){
			printf("cliente desconectado\n");
			exit(1);
		}
		printf("\nRecibido ECO:  %s\n",buffer);
	
	}
	return 0;
}
