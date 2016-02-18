#include <stdio.h>
#include "omp.h" //Заголовочный файл OpenMP
#include <time.h>
#include <malloc.h>
#define N 128
int main(int argc, char const *argv[]){

	int i,j,r,rank,size,threads, l;
	float A[N][N], B[N][N], C[N][N], Y[N][N], T[N][N];
	// float **A, **B, **C, **Y, **T;
	double time_start, time_finish;
	float *a_storage, *b_storage, *c_storage, *t_storage, *y_storage;
	
	// // a_storage = (float*)malloc(N*M); //выделяем память для массива размером N*M байт
	// // a = (char**)malloc(N*sizeof(char*)); //массив указателей на строки матрицы
	// // for (i = 0; i < N; ++i)
	// //     a[i] = a_storage + i*M; //инициализируем строки

	// a_storage = (float*)malloc(N*N);
	// b_storage = (float*)malloc(N*N);
	// c_storage = (float*)malloc(N*N);
	// t_storage = (float*)malloc(N*N);
	// y_storage = (float*)malloc(N*N);

	// A = (float**) malloc(N*N*sizeof(float*));
	// B = (float**) malloc(N*N*sizeof(float*));
	// C = (float**) malloc(N*N*sizeof(float*));
	// T = (float**) malloc(N*N*sizeof(float*));
	// Y = (float**) malloc(N*N*sizeof(float*));

	// for (i = 0; i < N; ++i){
	// 	A[i] = a_storage + i*N; //инициализируем строки
	// 	B[i] = b_storage + i*N; //инициализируем строки
	// 	C[i] = c_storage + i*N; //инициализируем строки
	// 	T[i] = t_storage + i*N; //инициализируем строки
	// 	Y[i] = y_storage + i*N; //инициализируем строки
	// }


	for (i = 0; i < N; ++i)
	{
		for (j = 0; j < N; ++j)
		{
			A[i][j] = B[i][j] = C[i][j] = 1;
		}
	}

	threads = (int)(*argv[1] - '0');
	printf("%d\t", threads);
	omp_set_num_threads(i);

	time_start = clock();

	for (l = 0; l < 10; l++)
	{

	#pragma omp parallel private(rank,size,i,j,r) 
	{ // Начало параллельной области
	#pragma omp for
		for (i = 0; i < N; i++){
			for (j = 0; j < N; j++){
				Y[i][j] = 0; // Этот массив нужно обнулить заранее
				T[i][j] = 0;

				T[i][j] = (A[i][j]+B[i][j]+C[i][j]/B[i][j]);

				for (r = 0; r < N; r++) Y[i][j]+=T[i][r]*C[r][j];
			}
		}

	} // Конец параллельной области


	
	// for (i = 0; i < N; i++)
	// {
	// 	printf("\n");
	// 	for (j = 0; j < N; j++)
	// 	{
	// 		printf("%d ", Y[i][j]);
	// 	}
	// }

	} //l loop

	time_finish = clock();
	printf("%lf\n", ((double) (time_finish-time_start)/CLOCKS_PER_SEC));

	return 0;
}



// Y=(A+B+C/B)*C