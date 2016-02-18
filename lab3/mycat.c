#include <stdio.h>
#include <fcntl.h>
#define PERMS 0666 


void toggle_cat(){
	char buf[BUFSIZ];
	int n;
	n = read(0, buf, BUFSIZ);
	write(0, buf, n);
	toggle_cat();
}

main(int argc, char *argv[]){
	int fp, n;
	char buf[BUFSIZ];


	if (argc == 1){
		// n = read(0, buf, BUFSIZ);
		toggle_cat();
	}

	while(--argc > 0){
		if ((fp = open(argv[1], O_RDONLY, 0)) == -1)
			printf("сat: не могу открыть файл %s\n", argv[1]);

		while ((n = read(fp, buf, BUFSIZ)) > 0)
			if (write(0, buf, n) != n)
				printf("сat: ошибка при записи\n");
			close(fp);
	}


	return 0;
}
