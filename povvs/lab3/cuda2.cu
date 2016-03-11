int main ( int argc, char * argv [] ){

	int i,blocks,blocksize,steps;
	float vect1[N],vect2[N];
	FILE *f;
	float * devA, *devB, elapsedTime;
	cudaEvent_t start, stop; //Идентификаторы событий

	cudaEventCreate(&start); //Инициализация события start
	cudaEventCreate(&stop); //Инициализация события stop

	for (i=0; i<N; i++) { vect1[i]=i; vect2[i]=i;}

	cudaMalloc ( (void**)&devA, N * sizeof ( float ) ) ;
	cudaMalloc ( (void**)&devB, N * sizeof ( float ) ) ;

	blocks=16; blocksize=128;
	steps=(int)N/(blocks*blocksize);

	cudaEventRecord(start,0); // Фиксация события start
	cudaMemcpy ( devA, vect1, N * sizeof ( float ), cudaMemcpyHostToDevice);
	cudaMemcpy ( devB, vect2, N*sizeof ( float ), cudaMemcpyHostToDevice);

	FUN_KERNEL<<<blocks,blocksize>>> ( devA, devB, steps);

	cudaMemcpy ( vect1, devA, N * sizeof ( float ), cudaMemcpyDeviceToHost );
	cudaFree ( devA );
	cudaEventRecord(stop,0); // Фиксация события stop
	cudaEventSynchronize(stop); // Синхронизация host и device по событию stop
	// Определение времени (в миллисекундах) между событиями start и stop
	cudaEventElapsedTime(&elapsedTime,start,stop);

	printf("time1 = %f\n", elapsedTime); //Вывод времени
	for (i = 0; i < N; i++) printf("vect1[%d] = %.5f\n", i, vect1[i]);
		
	return 0;
}
