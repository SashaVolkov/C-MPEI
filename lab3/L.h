#include <sys/param.h>
#include <fcntl.h>
#include <unistd.h>
#define BSIZE 64
#define NOFILE 4
#define DIRTY 1
/* буфер занят*/
#define EOF (-1)
/* конец файла*/
#define ERROR (-2)
/* ошибка в-в*/
struct f_addr{
	long f_blkno;
	/*обрабатываемый блок */
	int f_valid;
	/* возвращаемое значение*/
	char f_buffer[BSIZE];
	/* буфер обмена*/
	char f_flag;
	/* флаг занятости буфера */
};

struct f_addr *f_pntr[NOFILE];

aopen(char *name,int flag){
	int fd;
	struct f_addr *fp;
	fd=open(name,flag);

	if(fd>=0){
		f_pntr[fd] = malloc(sizeof(struct f_addr));

		if(!f_pntr[fd])
			return(-1);
		fp = f_pntr[fd];
		fp -> f_blkno = -1;
		fp -> f_flag = fp->f_valid = 0;
	}

	return(fd);
}

aclose(int fd){
	struct f_adr *fp = f_pntr[fd];
	chkflush(fd);
	free(fp);
	f_pntr[fd] = 0;
	return(close(fd));
}

agetb(int fd, long address){
	struct f_addr *fp = f_pntr[fd];
	int a_offset = address%BSIZE;
	long a_blkno = address/BSIZE;	fp->f_blkno = a_blkno;
		lseek(fd, fp->f_buffer,BSIZE);
		fp->f_valid = read(fd,fp->f_buffer,BSIZE);
		lseek(fd,fp->f_blkno*BSIZE,0);
		fp->f_valid = read (fd, fp->f_buffer,BSIZE);

		if (fp->f_valid <= 0)
		

	if (fp->f_blkno != a_blkno){
		chkflush(fd);
		fp->f_blkno = a_blkno;
		// lseek(fd, fp->f_buffer,BSIZE);
		// fp->f_valid = read(fd,fp->f_buffer,BSIZE);
		lseek(fd,fp->f_blkno*BSIZE,0);
		fp->f_valid = read (fd, fp->f_buffer,BSIZE);

		if (fp->f_valid <= 0)
			return(fp->f_valid - 1);
	}

	if(a_offset < fp->f_valid)
		return(fp->f_buffer[a_offset]&0xFF);
	else
		return(EOF);
}

asetb (int fd, long address, int value){
	
	struct f_addr *fp = f_pntr[fd];
	int a_offset = address%BSIZE;
	long a_blkno = address/BSIZE;
	if(fp->f_blkno != a_blkno){
		chkflush(fd);
		fp->f_blkno = a_blkno;
		lseek(fd, fp->f_blkno*BSIZE,0);
		fp->f_valid = read(fd, fp->f_buffer, BSIZE);

		if(fp->f_valid <= 0)
			return(fp->f_valid - 1);
	}
	fp->f_buffer[a_offset] = value&0xFF;
	fp->f_flag = DIRTY;
	return(0);
}

chkflush(int fd){
	struct f_addr *fp = f_pntr[fd];
	if(fp->f_flag == DIRTY){
		lseek(fd, fp->f_blkno*BSIZE,0);
		return(write(fd, fp->f_buffer, BSIZE));
	}
	return 0;

}