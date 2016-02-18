#include <stdio.h>
#include <time.h>
#include "omp.h" //Заголовочный файл OpenMP
#define N 128


int main(int argc, char const *argv[]){

	int i,j,r,rank,size, threads, l;
	float A[N][N], B[N][N], C[N][N], Y[N][N], T[N][N];
	double time_start, time_finish;

	time_start = clock();

	for (i = 0; i < N; i++)
	{
		for (j = 0; j < N; j++)
		{
			A[i][j] = B[i][j] = C[i][j] = 1;
		}
	}
	threads = (int)(*argv[1] - '0');
	printf("%d\t", threads);
	omp_set_num_threads(i);

	for (l = 0; l < 10; l++)
	{

		#pragma omp parallel private(rank,size,i,j,r) 
		{ // Начало параллельной области
			size = omp_get_num_threads(); //Получаем число запущенных потоков
			rank = omp_get_thread_num(); //Каждый поток получает свой номер
			for (i = rank*N/size; i < (rank+1)*N/size; i++)
				for (j = 0; j < N; j++){
					Y[i][j] = 0; // Этот массив нужно обнулить заранее
					T[i][j] = 0;

					T[i][j] = (A[i][j]+B[i][j]+C[i][j]/B[i][j]);

					for (r = 0; r < N; r++) Y[i][j]+=T[i][r]*C[r][j];
				}
		} // Конец параллельной области

	} //l loop

	// for (i = 0; i < N; i++)
	// {
	// 	printf("\n");
	// 	for (j = 0; j < N; j++)
	// 	{
	// 		printf("%d ", Y[i][j]);
	// 	}
	// }



	time_finish = clock();
	printf("%lf\n", ((double) (time_finish-time_start)/CLOCKS_PER_SEC));


	return 0;
}

// Y=(A+B+C/B)*C