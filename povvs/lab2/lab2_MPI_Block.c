# include <stdio.h>
# include <mpi.h>
# include <math.h>
#include <fcntl.h>
#define PERMS 0666

int main ( int argc, char * argv [] ) {

	// if (argc != 2) return 1;

	long n, k;
	int id, np, i, prev, next, fp;
	double *A, *B, *C, *Y;
	double S1, S2, start, stop, elapsedTime;
	MPI_Status status;


	n = atoi(argv[1]);


	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &np);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	if (id == 0)
	{
		creat("recv", PERMS);
		fp = open("recv", O_WRONLY, 0);
	}

	S1 = 15.56;
	S2 = 4.33;
	k=n/np;

	A = (double *) malloc(n*sizeof(double));
	B = (double *) malloc(n*sizeof(double));
	C = (double *) malloc(n*sizeof(double));
	Y = (double *) malloc(n*sizeof(double));


	start = MPI_Wtime();


	for (i = id*k; i < (id+1)*k; i++) {
		A[i] = (double)i*i;
		B[i] = (double)pow(i, 7);
		C[i] = (double)i*(n-i);
		Y[i] =(A[i] + S1*B[i])*C[i] + S2;
	}

	if (id != 0){
		MPI_Send(&Y[id*k], k, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
	}

	if (id == 0){
		for (i = 1; i < np; i++)
		{
			MPI_Recv(&Y[i*k], k, MPI_DOUBLE, i, id, MPI_COMM_WORLD, &status);
		}
	}

	stop = MPI_Wtime();

	// int MPI_File_write(MPI_File fh, void *buf,
	//     int count, MPI_Datatype datatype,
	//     MPI_Status *status)



	MPI_Finalize();


	if (id == 0)
	{
		elapsedTime = stop - start;
		write(0, &Y[n-1], sizeof(double));
		// fwrite(&np, sizeof(int), 1,fp);
		// fwrite(&Y[n-1], 1, sizeof(double),fp);
		// fwrite(&elapsedTime, sizeof(double), 1,fp);
		// printf("Y[n-1] =  %.4f\n", Y[n-1]);
		// printf("time = %lf\n", stop - start);
		close(fp);

		free(A);
		free(B);
		free(C);
		free(Y);
	}



	return 0;
}

// Y i =(A i +S1*B i )*C i +S2



// MPI_Bsend(&A[n-1], 1, MPI_DOUBLE, next, next, MPI_COMM_WORLD);
// MPI_Bsend(&B[n-1], 1, MPI_DOUBLE, next, next, MPI_COMM_WORLD);
// MPI_Bsend(&C[n-1], 1, MPI_DOUBLE, next, next, MPI_COMM_WORLD);


// MPI_Recv(&A[0], 1, MPI_DOUBLE, prev, id, MPI_COMM_WORLD, &status);
// MPI_Recv(&B[0], 1, MPI_DOUBLE, prev, id, MPI_COMM_WORLD, &status);
// MPI_Recv(&C[0], 1, MPI_DOUBLE, prev, id, MPI_COMM_WORLD, &status);