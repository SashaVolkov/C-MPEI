#include "L.h"
#include <stdio.h>
int main(int argc, char *argv[])
{
	int f;
	int ess;	
	int i,n;
	char buf [BSIZE];
	if (argc!=2)
	{
		printf("not enough parameters\n");
		return 1;
	}
	f=aopen(argv[1], O_RDWR);
	if (f < 0)
	{
		printf("\nfile %s no file found\n",argv[1]);
		return 1;
	}
    
	for (i=0; i<BSIZE/2;i++)
	{
		buf[i]=agetb(f, BSIZE+i);
	}
	for (i=0; i<BSIZE/2;i++)
	{
		buf[i+(BSIZE/2)]=agetb(f, (BSIZE*3)+i);
	}

	n=lseek(f, 0, SEEK_END);
	printf("file size = %d\n", n);
	for (i=0; i<BSIZE;i++)
	{
		int ad=n+i;
		int k=asetb(f,ad,buf[i]);
	}
	printf("chkflush res = %d\n", chkflush(f));
	aclose(f);
}