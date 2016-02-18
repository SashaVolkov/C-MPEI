#include <stdio.h>
#include <time.h>

/* cat: конкатенация файлов, версия 1 */
main(int argc, char *argv[]){

	time_t *time1, *time2;


	FILE *fp;
	void filecopy(FILE *, FILE *);


	time(time1);


	if (argc == 1) /* нет аргументов; копируется стандартный ввод */
		filecopy(stdin, stdout);
	else
		while (--argc > 0)
		if ((fp = fopen(*++argv, "r")) == NULL) {
			printf("cat: не могу открыть файл %s\n", *argv);
			return 1;
		}
		else {
			filecopy(fp, stdout);
			fclose(fp);
		}


		time(time2);

		printf("%f\n", difftime(*time2,*time1));

	return 0;
	}



	/* filecopy: копирует файл ifp в файл ofp */
	
void filecopy(FILE *ifp, FILE *ofp){
	int c;
	while ((c = getc(ifp)) != EOF)
		putc(c, ofp);
}