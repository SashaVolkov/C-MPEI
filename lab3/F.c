
#include "F1.h"
#include <stdio.h>
#define BUFSIZE 64


int main(int argc, char *argv[])
{
	FILE *f;
	int ess;	
	int i, p;
	char buf [BUFSIZE];

	if (argc!=2){
		printf("not enough parameters\n");
		return 1;
	}
	f=aopen(argv[1],2);

	if(f==NULL){
		printf("\nfile %s no file found\n",argv[1]);
		return 1;
	}

	for (i=BUFSIZE;i<((BUFSIZE*3)/2);i++){
		int j=i-BUFSIZE;
		buf[j]=agetb(f,i);
	}

	for (i=(BUFSIZE*3);i<((BUFSIZE*7)/2);i++){
		int j=i-(BUFSIZE*5)/2;
		buf[j]=agetb(f,i);	
	}

	for (i=0;i<BUFSIZE;i++){
		int x=aaddb(f,1,buf[i]);
	}

	p = aclose(f);


}