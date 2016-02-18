/*3) Разработайте два процесса, из которых ПРОЦЕСС-ПРЕДОК  порождает ПРОЦЕСС-ПОТОМОК и через непоименованный канал взаимодействует с ним, передавая содержимое некоторого файла поблочно. Если предок не может войти в файл, он должен передать сообщение на вывод STDERR. Потомок получает блоки файла и их выводит на экран, используя STDOUT. По окончании файла предок передает потомку сигнал завершения и после окончания работы потомка (о чем получает сигнал) прекращает свою работу.
5) Решите задачу 3 при условии, что потомок снимает информацию из файла, а предок передает ее на экран.

*/

/* Если Вы используете водопроводы или FIFO, Вы должны разработать ваше приложение так, чтобы один процесс открыл водопровод для чтения перед тем как другой начал запиись. Если процесс считывания никогда не начинается, или завершается неожиданно, запись в водопровод или FIFO вызывает сигнал SIGPIPE. Если SIGPIPE блокирован, обрабатывается или игнорируется, дргие обращения вызывают EPIPE взамен. */

/* printf в потомке не пишет на стандартный вывод.*/

#include <stdio.h>
#include <fcntl.h>  /*открытие файлов*/
#include <signal.h> /*сигналы*/
#include <unistd.h> /*getppid()*/
#define blocksize 64

unsigned char buf[blocksize+1];
int pa[2];
int pidOr;/*id предка/потомка для потомка/предка*/

void  sigRcvHandler(int a)
{
  read(pa[0],buf,1);
  read(pa[0],&buf[1],buf[0]);
  write(1,&buf[1],buf[0]);
  kill(pidOr,SIGUSR1);
  return;
}


int main (int argc, char *argv[])
{
  /**/
  /*Предок передает информацию от потомка на экран,
    а значит, принимает информацию по pipe при получении сигнала
  */


  int fd1; /*файловый дескриптор передаваемого файла*/
  short int flag=1;
  struct sigaction newHandler;
  memset(&newHandler, 0, sizeof(newHandler));
  newHandler.sa_handler = sigRcvHandler;
  sigset_t   set; 
  sigemptyset(&set);                                                             
  sigaddset(&set, SIGUSR1);
  newHandler.sa_mask = set;
  
  if (argc<2)
    {
      printf("Не передано имя файла \n");
      exit(1);
    }
  pipe(pa);
  sigaction(SIGUSR1, &newHandler, 0);
  signal(SIGTRAP,SIG_IGN);
  if (pidOr=fork())
    {
      /* Это процесс предок */
      /* signal(SIGUSR1,sigRcvHandler); */
      close(pa[1]); /* только читает */
      printf("лололо");
      wait();
      return 0;
      
    }
  else
  {
      /* Это процесс потомок */
      /* Потомок получает информацию из файла, при неудаче выдает сообещение
      на STDERR, передает поблочно информацию предку, оповещая его сигналом
      , в конце передачи посылает ему сигнал завершения */
      pidOr=getppid();
      if((fd1=open(argv[1],O_RDONLY))==-1)
	{
	  write(2,"Файл не может быть открыт\n",52);
	   /* Конец передачи */
	  return 1;
	}
      else
	{
	  int number=0;
	  close(pa[0]); /* только пишет */
	  while(flag!=0)
	    {	      
	      if((buf[0]=read(fd1,&buf[1],blocksize))!=blocksize)
		{
		  /* Последний блок */
		  flag=0;
		  write(pa[1],buf,blocksize+1);
		  number++;
		  printf("%d  пересылка",number);
		  kill(pidOr,SIGUSR1);
		  kill(getpid(),SIGSTOP);
		}
	      else
		{
		  /* Непоследний блок */
		  write(pa[1],buf,blocksize+1);
		  number++;
		  printf("%d  пересылка",number);
		  kill(pidOr,SIGUSR1);
		  kill(getpid(),SIGSTOP);
		}
	    }
	  return 0;
	}
 }
}


