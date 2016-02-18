#include <stdio.h>
#include <time.h>
#include "omp.h" //Заголовочный файл OpenMP
#define N 128

int main(int argc, char const *argv[]){

	int i,j,r,rank,size, threads, l;
	float A[N][N], B[N][N], C[N][N], Y[N][N], T[N][N];
	double time_start, time_finish;




	threads = (int)(*argv[1] - '0');
	printf("%d\t", threads);


		/* code */
	

	for (i = 0; i < N; i++)
	{
		for (j = 0; j < N; j++)
		{
			A[i][j] = B[i][j] = C[i][j] = 1;
		}
	}

	omp_set_num_threads(i);
	time_start = clock();

	for (l = 0; l < 10; l++)
	{

	#pragma omp parallel private(rank,size,i,j,r) 
	{ // Начало параллельной области
		#pragma omp sections
		{
			#pragma omp section

			for (i = 0; i < N/4; i++){
				for (j = 0; j < N; j++){
					Y[i][j] = 0; // Этот массив нужно обнулить заранее
					T[i][j] = 0;

					T[i][j] = (A[i][j]+B[i][j]+C[i][j]/B[i][j]);

					for (r = 0; r < N; r++) Y[i][j]+=T[i][r]*C[r][j];
				}
			}

			#pragma omp section

			for (i = N/4; i < N/2; i++){
				for (j = 0; j < N; j++){
					Y[i][j] = 0; // Этот массив нужно обнулить заранее
					T[i][j] = 0;

					T[i][j] = (A[i][j]+B[i][j]+C[i][j]/B[i][j]);

					for (r = 0; r < N; r++) Y[i][j]+=T[i][r]*C[r][j];
				}
			}

			#pragma omp section

			for (i = N/2; i < 3*N/4; i++){
				for (j = 0; j < N; j++){
					Y[i][j] = 0; // Этот массив нужно обнулить заранее
					T[i][j] = 0;

					T[i][j] = (A[i][j]+B[i][j]+C[i][j]/B[i][j]);

					for (r = 0; r < N; r++) Y[i][j]+=T[i][r]*C[r][j];
				}
			}

			#pragma omp section

			for (i = 3*N/4; i < N; i++){
				for (j = 0; j < N; j++){
					Y[i][j] = 0; // Этот массив нужно обнулить заранее
					T[i][j] = 0;

					T[i][j] = (A[i][j]+B[i][j]+C[i][j]/B[i][j]);

					for (r = 0; r < N; r++) Y[i][j]+=T[i][r]*C[r][j];
				}
			}
		}

	} // Конец параллельной области



	// for (i = 0; i < N; i++)
	// {
	// 	printf("\n");
	// 	for (j = 0; j < N; j++)
	// 	{
	// 		printf("%lf ", Y[i][j]);
	// 	}
	// }

	} //l loop


	time_finish = clock();
	printf("%lf\n", ((double) (time_finish-time_start)/CLOCKS_PER_SEC));


	return 0;
}

// Y=(A+B+C/B)*C