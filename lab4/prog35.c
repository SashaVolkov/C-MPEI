/*3) Разработайте два процесса, из которых ПРОЦЕСС-ПРЕДОК  порождает ПРОЦЕСС-ПОТОМОК и через непоименованный канал взаимодействует с ним, передавая содержимое некоторого файла поблочно. Если предок не может войти в файл, он должен передать сообщение на вывод STDERR. Потомок получает блоки файла и их выводит на экран, используя STDOUT. По окончании файла предок передает потомку сигнал завершения и после окончания работы потомка (о чем получает сигнал) прекращает свою работу.
5) Решите задачу 3 при условии, что потомок снимает информацию из файла, а предок передает ее на экран.
*/

/* printf в потомке не пишет на стандартный вывод. - решилось, все дело в строчной буферизации терминала*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>/*memset*/
#include <errno.h> /*perror, errno*/
#include <fcntl.h>  /*открытие файлов*/
#include <signal.h> /*сигналы*/
#include <unistd.h> /*getppid()*/
#define blocksize 64

unsigned char buf[blocksize+1];
int pa[2];
int status;
int pid_parent;/*id предка/потомка для потомка/предка*/
short int flag=1;
short int flag2=1;

void  sigRcvHandler(int a){
	if(a==SIGUSR1){
		read(pa[0],buf,1);
		if (errno!=EINTR && errno){
			perror("err3");
			fflush(stderr);
			exit(3);
		}

		read(pa[0],&buf[1],buf[0]);
		if (errno!=EINTR && errno){
			perror("err4");
			fflush(stderr);
			exit(4);
		}
		write(1,&buf[1],buf[0]);
		fflush(stdout);
		kill(pid_parent,SIGUSR2);
	}
	else{
		flag2=0;    
	}
	return;
}

void sigContHandler(int a)
{
	/* printf("\nПродолжение, flag = %d\n",flag);*/
	/* Просто обработчик для того, чтобы выполнение продолжалось после сигнала */
	return;
}


int main (int argc, char *argv[])
{
	/**/
	/*Предок передает информацию от потомка на экран,
		а значит, принимает информацию по pipe при получении сигнала
	*/

	int fd1; /*файловый дескриптор передаваемого файла*/
	struct sigaction newHandler;
	memset(&newHandler, 0, sizeof(newHandler));
	newHandler.sa_handler = sigRcvHandler;
	sigset_t   set1,set2; 
	sigemptyset(&set1);
	sigemptyset(&set2);
	sigaddset(&set1,SIGUSR1);
	sigaddset(&set1,SIGUSR2);
	newHandler.sa_mask = set1;
	sigaction(SIGUSR1,&newHandler,0);
	sigaction(SIGUSR2,&newHandler,0);
	newHandler.sa_handler=sigContHandler;
	sigaddset(&set2,SIGUSR2); 
	newHandler.sa_mask=set2;
	
	if (argc<2)
		{
			printf("Не передано имя файла \n");
			exit(1);
		}
	pipe(pa);
	if (pid_parent=fork())
		{
			/* Это процесс предок */
			close(pa[1]); /* только читает */
			while(flag2){};
			return 0;
			
		}
	else
	{
			/* Это процесс потомок */
			/* Потомок получает информацию из файла, при неудаче выдает сообещение
			на STDERR, передает поблочно информацию предку, оповещая его сигналом
			, в конце передачи посылает ему сигнал завершения */
			pid_parent=getppid();
			sigaction(SIGUSR2,&newHandler,0);
			if((fd1=open(argv[1],O_RDONLY))==-1)
	{	  
		write(2,"Файл не может быть открыт\n",52);
		 /* Конец передачи */
		kill(pid_parent,SIGUSR2);
		return 1;
	}
			else
	{
		int number=0;
		close(pa[0]); /* только пишет */
		while(flag){
			if((buf[0]=read(fd1,&buf[1],blocksize))!=blocksize)
		{
			/* Последний блок */
			flag=0;
			write(pa[1],buf,blocksize+1);
			if (errno!=EINTR && errno) {perror("error2");
				fflush(stderr);
				exit(2);}
			number++;
			printf("\n%d  пересылка\n",number);
			kill(pid_parent,SIGUSR1);
			pause();
			kill(pid_parent,SIGUSR2); /*послать завершение передачи*/
		}
		else{
			/* Непоследний блок */
			write(pa[1],buf,blocksize+1);
			if (errno!=EINTR && errno) {perror("error1");
				fflush(stderr);
				exit(1);}
			number++;
			printf("\n%d  пересылка\n",number);
			kill(pid_parent,SIGUSR1);
			pause(); /* Проблема здесь */
			/* kill(getpid(),SIGSTOP); */
			/* pause выставляет errno в EINTR как нормальное поведение */
		}
			}
		return 10;
	}
 }
}


