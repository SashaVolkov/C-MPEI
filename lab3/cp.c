#include <stdio.h>
#include <fcntl.h>
#define PERMS 0666 
void error(char *, ...);
/* ср: копирование f1 в f2 */
main(int argc, char *argv[]){
	int f1, f2, n;
	char buf[BUFSIZ];

	if (argc != 3)
		printf("ср: откуда куда\n");
	if ((f1 = open(argv[1], O_RDONLY, 0)) == -1)
		printf("ср: не могу открыть файл %s\n", argv[1]);
	if ((f2 = creat(argv[2], PERMS)) == -1)
		printf("ср: не могу создать файл %s, режим %d\n",argv[2], PERMS);
	while ((n = read(f1, buf, BUFSIZ)) > 0)
		if (write(f2, buf, n) != n)
			printf("ср: ошибка при записи в файл %s\n", argv[2]);
	return 0;
}

