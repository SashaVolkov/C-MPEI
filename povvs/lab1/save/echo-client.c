#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>


#define BUF_SIZE 1024

int main()
{
	int sock, n;
	char bufer[BUF_SIZE];
	struct sockaddr_in addr;
	FILE *fp;

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0){
		perror("socket");
		exit(1);
	}

	addr.sin_family = AF_INET;
	addr.sin_port = htons(3425); // или любой другой порт...
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	// printf("Hi\n");
	if(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0){
		perror("connect");
		exit(2);
	}

	if (!(fp = fopen("mycat", "rb")))
		printf("сlient: не могу открыть файл\n");

	while ((n = fread(bufer, 1, BUF_SIZE,fp)) > 0){
		send(sock, bufer, n, 0);
		printf("%d\n", n);
	}


	fclose(fp);

	close(sock);

	return 0;
}