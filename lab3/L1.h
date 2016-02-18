/*#include <sys/param.h> */
#include <fcntl.h> 
#include <stdlib.h>
#include <unistd.h>

#define BSIZE 64
#define NOFILE 4 /* Number Of Files ?? а по смыслу число файлов?*/
 
#define DIRTY 1        /* буфер занят*/ 
#define EOF (-1)       /* конец файла*/ 
#define ERROR (-2)       /* ошибка в-в*/ 
 
typedef struct 
{ 
	long f_blkno;           /*обрабатываемый блок */ 
	int  f_valid;            /* возвращаемое значение*/ 
	char f_buffer[BSIZE];   /* буфер обмена*/ 
	char f_flag;            /* флаг занятости буфера */ 
} f_addr; 
 
f_addr *f_pntr[NOFILE];
 /* массив из NOFILE указателей на структуру f_addr  */


int asize(int fd) 
{ /*возвращает длину файла */
	/* сохранить текущее положение указателя */ 
	long curpos = lseek(fd,0,SEEK_CUR); 
	int size=lseek(fd,0,SEEK_END); 
	/* восстановить старое положение */ 
	lseek(fd,curpos,SEEK_SET); 
	return size; 
} 

int aopen(char *name, int flag) 
{ /*возвращает FD, если любая ошибка - (-1)*/
	int fd; 
	f_addr *fp; 

	fd = open(name,flag,0666); /*открыть или создать с правами на чтение и запись для всех*/
	if(fd>=0) /*если дескриптор выдан*/
	{ 
		f_pntr[fd] = malloc(sizeof(f_addr)); 
	/*Выдача памяти на 1 элемент f_addr*/
	/*странный ход, пришедший из исходника; расчитан на то, что
	значение дескриптора не превысит NOFILE, иначе ошибка памяти*/
		if (!f_pntr[fd]) /*если память не выделилась*/
			return(-1); 
		fp = f_pntr[fd]; 
		fp->f_blkno = -1; /*Стартовые значения*/
		fp -> f_flag = fp->f_valid = 0; 
	}
	return(fd); 
}

int aclose(int fd) 
{
	f_addr *fp = f_pntr[fd]; 

	chkflush(fd); 
	free(fp); 
	f_pntr[fd] = 0; /*высвобождение памяти + зануление указателя*/
	return(close(fd)); 
}

void copyfile (int fd1, int fd2) 
{ 
 /*дискрипторы файлов ввода и вывода, соответственно*/ 
/*функция изначально кривая, но потом исправлена*/ 
char buffer[BSIZE]; 
size_t n; 

/* сохранить текущее положение указателя */ 
long curpos1 = lseek(fd1,0,SEEK_CUR); 
long curpos2 = lseek(fd2,0,SEEK_CUR);
lseek(fd1,0,SEEK_SET); 
lseek(fd2,0,SEEK_SET); 

while ((n = read (fd1,buffer,BSIZE)) >0) 
{ 
write (fd2,buffer,n); 
} 
/* восстановить старое положение */ 
lseek(fd1,curpos1,SEEK_SET); 
lseek(fd2,curpos2,SEEK_SET); 

} 


void aseek(int fd,long address) 
{ /*выставление указтеля файла*/
	
	f_addr *fp = f_pntr[fd]; 
	int a_offset = address % BSIZE; 
	long a_blkno = address / BSIZE; 
	chkflush(fd); 
	fp->f_blkno = a_blkno; 
	lseek(fd,fp->f_blkno*BSIZE,SEEK_SET); 
		fp->f_valid = read (fd, fp->f_buffer,BSIZE);	 
} 


int agetb(int fd,long address) 
{ 
	f_addr *fp = f_pntr[fd]; 
	int    a_offset = address % BSIZE; 
	long   a_blkno = address / BSIZE; 
 
	if (fp->f_blkno != a_blkno) 
	{ 
		chkflush(fd); 
 
		fp->f_blkno = a_blkno; 	
		lseek(fd,fp->f_blkno*BSIZE,SEEK_SET); 
		fp->f_valid = read (fd, fp->f_buffer,BSIZE); 
 
		if (fp->f_valid <= 0) 
			return(fp->f_valid - 1);
 /*если прочитали от 0 и меньше - вернуть ошибку, при этом старшая часть int будет ненулевой и так можно увидеть ошибку*/
	} 
	if(a_offset < fp->f_valid) 
		return(fp->f_buffer[a_offset] & 0xFF); /*маскирование последнего байта*/
	else 
		return(EOF); 
} 
 
int  asetb (int fd, long address, long value) 
{ 
	f_addr *fp = f_pntr[fd]; 
	int a_offset = address % BSIZE; 
	long   a_blkno = address / BSIZE; 
 
	if(fp->f_blkno != a_blkno) 
	{ 
		chkflush(fd); 
		fp->f_blkno = a_blkno; 
		lseek(fd, fp->f_blkno*BSIZE,SEEK_SET); 
		fp->f_valid = read(fd, fp->f_buffer, BSIZE); 
		if(fp->f_valid <= 0) 
			return(fp->f_valid - 1); /*выдача ошибки*/
	} 
 
	fp->f_buffer[a_offset] = value&0xFF; 
	fp->f_flag |= DIRTY; 
	return(0); 
} 


void alread(int fd, long address,char *buf,int count)
{
	long i=0; 
	for (i=0;i<count;i++) 
	{ 
		 
		buf[i]=agetb(fd,address+i); 
				 
	} 	
}

 
void awrite(int fd, long address,char *buf,int count) 
{ 
	long i=0; 
	for (i=0;i<count;i++) 
	{ 
		 
		asetb(fd,address+i,buf[i]); 
		 
		 
	} 
 
} 
 
int chkflush(int fd) 
{ /*сброс буфера в файл*/
	f_addr *fp = f_pntr[fd]; 
 
	if(fp->f_flag & DIRTY) /*если в буфере что-то есть - тогда слить в файл, иначе ничего не делать*/
	{ 
		lseek(fd, fp->f_blkno*BSIZE,SEEK_SET); 
		return (write(fd, fp->f_buffer, BSIZE)); 
	} 
}