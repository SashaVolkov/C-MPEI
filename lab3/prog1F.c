#include "F1.h"
#include <stdio.h>
#define BSIZE    64 

int main(int argc, char *argv[])
{
char buf1[BSIZE/2], buf2[BSIZE/2];
int b = 0,j = 0, i = 1;
FILE* f=0;
if (argc==1) 
{

	printf("Not enough actual parametres\n");
	return 1;

}

f = aopen(argv[1],O_RDWR);
if (f==0) 
{
	printf("\nFile %s Not Found\n",argv[i]);
	return 1;
}
int size=asize(f);
if (size>4*BSIZE+BSIZE/2) /*если файл достаточно длинный*/
{
/*начало 2го блока (считая с 0) */
	alread(f,BSIZE*2,buf1,BSIZE/2);
	alread(f,BSIZE*4,buf2,BSIZE/2);
	awrite(f,size,buf1,BSIZE/2);
	awrite(f,size+BSIZE/2,buf2,BSIZE/2);
}
else
{
	printf("\nFile is too short for this operation");
	return 1;
}
aclose(f);
return 0;
}