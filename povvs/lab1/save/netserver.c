#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <strings.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>

#define BUF_SIZE 1024

int main(int argc, char ** argv)
{
	int n;
	int sock, newsock, port, clen;
	char buf[BUF_SIZE];
	FILE *fp;
	struct sockaddr_in serv_addr, cli_addr;
	if (argc < 2) 
	{
		fprintf(stderr,"usage: %s <port_number>\n", argv[0]);
		return EXIT_FAILURE;
	}
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (socket < 0)
	{
		printf("socket() failed: %d\n", errno);
		return EXIT_FAILURE;
	}
	memset((char *) &serv_addr, 0, sizeof(serv_addr));
	port = atoi(argv[1]);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(port);  //Функция htons() переписывает двухбайтовое значение порта так, чтобы порядок байтов соответствовал принятому в Интернете
	// printf("%d\n", serv_addr.sin_port);
	if (bind(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) // связывание сервер-клиент
	{
		printf("bind() failed: %d\n", errno);
		close(sock);
		return EXIT_FAILURE;
	}
	listen(sock, 1);
	clen = sizeof(cli_addr);
	newsock = accept(sock, (struct sockaddr *) &cli_addr, &clen); //функция accept() возвращает новый сокет, открытый для обмена данными с клиентом
	if (newsock < 0) 
	{
		printf("accept() failed: %d\n", errno);
		close(sock);
		close(newsock);
		return EXIT_FAILURE;
	}
	memset(buf, 0, BUF_SIZE);


	creat("mycat1", 0777);
	if (!(fp = fopen("mycat1", "wb")))
		printf("server: не могу открыть файл\n");

	while (1){
		int nbytes = recv( newsock, buf, sizeof(buf), 0 );
		printf("%d\n", nbytes);
		if ( nbytes <= 0) break;

		if (fwrite(buf, 1, nbytes,fp) != nbytes)
			printf("server: ошибка при записи\n");
	 }

	 // write(fp, EOF, 1);

	 fclose(fp);

	close(newsock);
	close(sock);
}
