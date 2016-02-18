#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <strings.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
// #include <process.h>

#define BUF_SIZE 1024
#define PERMS 0777

int main(int argc, char ** argv)
{
	int n;
	char *my_args[3];
	pid_t pid;
	int rv;
	int sock, newsock, port, clen;
	char buf[BUF_SIZE];
	FILE *fp;
	struct sockaddr_in serv_addr, addr, cli_addr;

	if (argc < 2) {
		fprintf(stderr,"usage: %s <port_number>\n", argv[0]);
		return EXIT_FAILURE;
	}

	sock = socket(AF_INET, SOCK_STREAM, 0);

	if (socket < 0){
		printf("socket() failed: %d\n", errno);
		return EXIT_FAILURE;
	}

	port = atoi(argv[1]);
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


	switch(pid=fork()) { // Этот цирк с конями(форками) нужен для creat с возможностью последующего открытия созданного файла
	case -1:
	        perror("fork"); /* произошла ошибка */
	        exit(1); /*выход из родительского процесса*/
	case 0:
			creat("hi1", PERMS);
			exit(rv);
	default:
			wait();
	}


	if (!(fp = fopen("hi1", "wb+"))) printf("server: не могу открыть файл\n");

	while (1){
		int nbytes = recv( newsock, buf, sizeof(buf), 0 );
		printf("%d\n", nbytes);
		if ( nbytes <= 0) break;

		if (fwrite(buf, 1, nbytes,fp) != nbytes) printf("server: ошибка при записи\n");
	 }
	 fclose(fp);




	  switch(pid=fork()) {
	  case -1:
	          perror("fork"); /* произошла ошибка */
	          exit(1); /*выход из родительского процесса*/
	  case 0:
	          my_args[0] = "hi1";
	          execlp ("/home/sasha/C/povvs/lab1/hi1", my_args[0], NULL);
	          exit(rv);
	  default:
	          printf("PARENT: Это процесс-родитель!\nPARENT: Мой PID %d, PID моего потомка %d\n", getpid(),pid);
	          printf("PARENT: Я жду, пока потомок не вызовет exit()...\n\n");
	          wait();
	          printf("\nPARENT: Код возврата потомка:%d\nPARENT: Выход!\n", WEXITSTATUS(rv));
	  }
	  close(newsock);
	close(sock);



	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0){
		perror("socket");
		exit(1);
	}

	addr.sin_family = AF_INET;
	addr.sin_port = htons(3426); // или любой другой порт...
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	if(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0){
		perror("connect");
		exit(2);
	}

	if (!(fp = fopen("text.txt", "rb")))
		printf("server: не могу открыть файл\n");

	while ((n = fread(buf, 1, BUF_SIZE,fp)) > 0){
		send(sock, buf, n, 0);
		printf("%d\n", n);
	}
	fclose(fp);

	close(sock);





}
