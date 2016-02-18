// http://citforum.ru/programming/unix/sockets/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>

#define SOCK_NAME "socket.soc"
#define BUF_SIZE 1024

int main(int argc, char ** argv)
{
  struct sockaddr srvr_name, rcvr_name;
  char buf[BUF_SIZE];
  int   sock;
  int   namelen, bytes;

  sock = socket(AF_UNIX, SOCK_DGRAM, 0);
  //Дескриптор сокета можно получить с помощью функции socket(2)
  // Первый параметр этой функции – домен, к которому принадлежит сокет.
  // Домен сокета обозначает тип соединения
  // (а не доменное имя Интернета, как вы могли бы подумать).
  // Домен, обозначенный константой AF_UNIX, соответствует сокетам в
  // файловом пространстве имен. Второй параметр socket() определяет тип
  // сокета. значение SOCK_DGRAM указывает датаграммный сокет
  // Датаграммные сокеты осуществляют ненадежные соединения при передаче данных
  // по сети и допускают широковещательную передачу данных.
  // Другой часто используемый тип сокетов – SOCK_STREAM соответствует потоковым сокетам,
  // реализующим соединения «точка-точка» с надежной передачей данных.
  // Впрочем, в пространстве файловых имен датаграммные сокеты также надежны, как и потоковые сокеты.
  // Третий параметр функции socket() позволяет указать протокол, используемый для передачи данных.
  // Мы оставляем значение этого параметра равным нулю.
  // В случае ошибки функция socket() возвращает -1.
  if (sock < 0) 
  {
    perror("socket failed");
    return EXIT_FAILURE;
  }
  srvr_name.sa_family = AF_UNIX;
  strcpy(srvr_name.sa_data, SOCK_NAME);
  if (bind(sock, &srvr_name, strlen(srvr_name.sa_data) +
        sizeof(srvr_name.sa_family)) < 0) 
  {
    perror("bind failed");
    close(sock);
    unlink(SOCK_NAME);
    return EXIT_FAILURE;
  }
  bytes = recvfrom(sock, buf, sizeof(buf),  0, &rcvr_name, &namelen);
  if (bytes < 0) 
  {
    perror("recvfrom failed");
    close(sock);
    unlink(SOCK_NAME);
    return EXIT_FAILURE;
  }
  buf[bytes] = 0;
  rcvr_name.sa_data[namelen] = 0;
  printf("Client sent: %s\n", buf);
  close(sock);
  unlink(SOCK_NAME);
}
 
