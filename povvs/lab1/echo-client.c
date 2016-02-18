#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <strings.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>


#define BUF_SIZE 1024
#define PERMS 0666


int main()
{
	int sock, newsock, clen, n, port;
	char bufer[BUF_SIZE];
	struct sockaddr_in serv_addr, addr, cli_addr;
	FILE *fp;

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0){
		perror("socket");
		exit(1);
	}

	addr.sin_family = AF_INET;
	addr.sin_port = htons(3425); // или любой другой порт...
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	if(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0){
		perror("connect");
		exit(2);
	}

	if (!(fp = fopen("hi", "rb")))
		printf("сlient: не могу открыть файл\n");

	while ((n = fread(bufer, 1, BUF_SIZE,fp)) > 0){
		send(sock, bufer, n, 0);
		printf("%d\n", n);
	}
	fclose(fp);

	close(sock);


	sock = socket(AF_INET, SOCK_STREAM, 0);

	if (socket < 0){
		printf("socket() failed: %d\n", errno);
		return EXIT_FAILURE;
	}

	port = 3426;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(port);  //Функция htons() переписывает двухбайтовое значение порта так, чтобы порядок байтов соответствовал принятому в Интернете
	// printf("%d\n", serv_addr.sin_port);
	if (bind(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) { // связывание сервер-клиент
		printf("bind() failed: %d\n", errno);
		close(sock);
		return EXIT_FAILURE;
	}

	listen(sock, 1);
	clen = sizeof(cli_addr);
	newsock = accept(sock, (struct sockaddr *) &cli_addr, &clen); //функция accept() возвращает новый сокет, открытый для обмена данными с клиентом

	if (newsock < 0) {
		printf("accept() failed: %d\n", errno);
		close(sock);
		close(newsock);
		return EXIT_FAILURE;
	}


	creat("textrecv.txt", PERMS);
	if (!(fp = fopen("textrecv.txt", "w"))) printf("client: не могу открыть файл\n");

	while (1){
		int nbytes = recv( newsock, bufer, sizeof(bufer), 0 );
		printf("%d\n", nbytes);
		if ( nbytes <= 0) break;

		if (fwrite(bufer, 1, nbytes,fp) != nbytes) printf("client: ошибка при записи\n");
	 }
	 fclose(fp);



	close(newsock);
	close(sock);

	return 0;
}