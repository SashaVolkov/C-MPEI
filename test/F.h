#include <stdio.h>

FILE *aopen(name, flag); int flag;
switch(flag){
	case 0: return(fopen(name, "r"));
	case 1: return(fopen(name, "w"));
	case 2: return(fpen(name, "r+"));
	default:return(NULL);
}

int aclose(fp){
	return(fclose(fp));
}

int agetb(FILE *fp, long addr){
	int es;
	if((es = fseek(fp, addr, SEEK_SET)) != 0) return(es);
	return (getc (fp));
}

int asetb(FILE *fp, long addr, char value){
	 int es;
	if((es = fseek(fp, addr, SEEK_SET)) != 0) return(es);
		return(putc(value, fp));
}