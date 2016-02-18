/*3) Разработайте два процесса, из которых ПРОЦЕСС-ПРЕДОК  порождает ПРОЦЕСС-ПОТОМОК и через непоименованный канал взаимодействует с ним, передавая содержимое некоторого файла поблочно. Если предок не может войти в файл, он должен передать сообщение на вывод STDERR. Потомок получает блоки файла и их выводит на экран, используя STDOUT. По окончании файла предок передает потомку сигнал завершения и после окончания работы потомка (о чем получает сигнал) прекращает свою работу.
5) Решите задачу 3 при условии, что потомок снимает информацию из файла, а предок передает ее на экран.
6) Решите задачу 5 при условии, что два потомка снимают информацию из двух разных файлов, а предок передает информацию на экран, снабдив идентификатором процесса-потомка, приславшего ее.
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
int pa1[2],pa2[2];
int status;
int pid_parent1,pid_parent2;/*id предка/потомка для потомка/предка*/
short int flag=2;
short int flag1=1;
short int flag2=1;

void  sigRcvHandler(int a,siginfo_t* b, void* c)
{
  switch(a)
    { 
    case SIGUSR1:
      read(pa1[0],buf,1);
      if (errno!=EINTR && errno) 
	{
	  perror("error31");
	  fflush(stderr);
	  exit(3);
	}
      read(pa1[0],&buf[1],buf[0]);
      if (errno!=EINTR && errno) 
	{
	  perror("error41");
	  fflush(stderr);
	  exit(4);
	}
      printf("\n1 потомок: \n");
      write(1,&buf[1],buf[0]);
      fflush(stdout);
      kill(pid_parent1,SIGUSR1);
      break;
    case SIGUSR2:
      read(pa2[0],buf,1);
      if (errno!=EINTR && errno) 
	{
	  perror("error32");
	  fflush(stderr);
	  exit(3);
	}
      read(pa2[0],&buf[1],buf[0]);
      if (errno!=EINTR && errno) 
	{
	  perror("error42");
	  fflush(stderr);
	  exit(4);
	}
      printf("\n2 потомок: \n"); 
      write(1,&buf[1],buf[0]);
      fflush(stdout);
      kill(pid_parent2,SIGUSR1);
      break;
    case SIGHUP:
      flag--;
      break;
    case SIGURG:
      flag--;
      break;
    default:
      printf("Неверный сигнал вызвал данный обработчик\n");
      break;
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
{/*0*/
  /**/
  /*Предок передает информацию от потомка на экран,
    а значит, принимает информацию по pipe при получении сигнала
  */

  int fd1; /*файловый дескриптор передаваемого файла*/
  struct sigaction newHandler;
  memset(&newHandler, 0, sizeof(newHandler));
  newHandler.sa_sigaction = sigRcvHandler;
  sigset_t   set1,set2; 
  sigemptyset(&set1);
  sigemptyset(&set2);
  sigaddset(&set1,SIGUSR1);
  sigaddset(&set1,SIGUSR2); 
  sigaddset(&set1,SIGURG); 
  sigaddset(&set1,SIGHUP); 
  newHandler.sa_mask = set1;
  newHandler.sa_flags = SA_SIGINFO | SA_RESTART;
  sigaction(SIGUSR1,&newHandler,0);
  sigaction(SIGUSR2,&newHandler,0);
  sigaction(SIGURG,&newHandler,0);
  sigaction(SIGHUP,&newHandler,0);
  newHandler.sa_handler=sigContHandler;
  sigaddset(&set2,SIGUSR1);
  newHandler.sa_mask=set2;
  
  if (argc<3)
    {
      printf("Не переданы имена всех файлов \n");
      exit(1);
    }
  pipe(pa1);
  pipe(pa2);
  if (pid_parent1=fork())
    {/*1*/
      /* Это процесс предок */
      close(pa1[1]); /* только читает */
      if (pid_parent2=fork())
	{/*2*/
	  /* Это процесс предок */
	  close(pa2[1]);
	  /*
	    int returnval=waitpid(pid_parent,&status,0);
	    perror("waitpid return: ");
	    printf("wait вернул : %d, status = %d, exit status of child:%d",
	    returnval,status,WEXITSTATUS(status));*/
	  while(flag){};
	  return 0;
	}/*2*/
      else
	{/*2*/
	  /*сюда надо умудриться впихнуть функцию потомка2*/
	  /* Это процесс потомок2 HUP */
	  /* Потомок получает информацию из файла, при неудаче выдает сообещение
	     на STDERR, передает поблочно информацию предку, оповещая его сигналом
	     , в конце передачи посылает ему сигнал завершения */
	  pid_parent2=getppid();
	  sigaction(SIGUSR1,&newHandler,0);
	  if((fd1=open(argv[2],O_RDONLY))==-1)
	    {/*3*/	  
	      write(2,"Файл не может быть открыт\n",52);
	      /* Конец передачи */
	      kill(pid_parent2,SIGHUP);
	      return 10;
	    }/*3*/
	  else
	    {/*3*/
	      close(pa2[0]); /* только пишет */
	      while(flag2)
		{/*4*/	      
		  if((buf[0]=read(fd1,&buf[1],blocksize))!=blocksize)
		    {/*5*/
		      /* Последний блок */
		      flag2=0;
		      write(pa2[1],buf,blocksize+1);
		      if (errno!=EINTR && errno) {perror("error2");
			fflush(stderr);
			exit(2);}
		      kill(pid_parent2,SIGUSR2);
		      pause();
		      kill(pid_parent2,SIGHUP); /*послать завершение передачи*/
		    }/*5*/
		  else
		    {/*5*/
		      /* Непоследний блок */
		      write(pa2[1],buf,blocksize+1);
		      if (errno!=EINTR && errno) {perror("error1");
			fflush(stderr);
			exit(1);}
		      kill(pid_parent2,SIGUSR2);
		      pause(); /* Проблема здесь */
		      /* kill(getpid(),SIGSTOP); */
		      /* pause выставляет errno в EINTR как нормальное поведение */
		    }/*5*/
		}/*4*/
	      return 11;
	    }/*3*/
	}/*2*/
    }/*1*/
  else
    {/*1*/
      /* Это процесс потомок1 URG*/
      /* Потомок получает информацию из файла, при неудаче выдает сообещение
	 на STDERR, передает поблочно информацию предку, оповещая его сигналом
	 , в конце передачи посылает ему сигнал завершения */
      pid_parent1=getppid();
      sigaction(SIGUSR1,&newHandler,0);
      if((fd1=open(argv[1],O_RDONLY))==-1)
	{/*2*/	  
	  write(2,"Файл не может быть открыт\n",52);
	  /* Конец передачи */
	  kill(pid_parent1,SIGURG);
	  return 10;
	}/*2*/
      else
	{/*2*/
	  close(pa1[0]); /* только пишет */
	  while(flag1)
	    {/*3*/	      
	      if((buf[0]=read(fd1,&buf[1],blocksize))!=blocksize)
		{/*4*/
		  /* Последний блок */
		  flag1=0;
		  write(pa1[1],buf,blocksize+1);
		  if (errno!=EINTR && errno) {perror("error2");
		    fflush(stderr);
		    exit(2);}
		  kill(pid_parent1,SIGUSR1);
		  pause();
		  kill(pid_parent1,SIGURG); /*послать завершение передачи*/
		}/*4*/
	      else
		{/*4*/
		  /* Непоследний блок */
		  write(pa1[1],buf,blocksize+1);
		  if (errno!=EINTR && errno) {perror("error1");
		    fflush(stderr);
		    exit(1);}
		  kill(pid_parent1,SIGUSR1);
		  pause(); /* Проблема здесь */
		  /* kill(getpid(),SIGSTOP); */
		  /* pause выставляет errno в EINTR как нормальное поведение */
		}/*4*/
	    }/*3*/
	  return 11;
	}/*2*/
    }/*1*/
}/*0*/


