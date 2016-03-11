# include <stdio.h>
# include <math.h>

int main ( int argc, char * argv [] ) {

	if (argc != 2) return 1;

	int n = atoi(argv[1]);
	printf("n = %d\n", n);

	// int i, steps;
	float *A, *B, *C;
	float S1, S2, elapsedTime;
	cudaEvent_t start, stop;

	cudaEventCreate(&start);
	cudaEventCreate(&stop);
	elapsedTime = 0.0f;
	cudaEventRecord(start, 0);

	A = (float *) malloc(n*sizeof(float));
	B = (float *) malloc(n*sizeof(float));
	C = (float *) malloc(n*sizeof(float));


	for (int i = 0; i < n; i++) {
		A[i] = i;
		B[i] = n-i;
	}
	S1 = 15.4;
	S2 = 4.4;


	for (int i = 0; i < n; i++) C[i]=(powf(S1,3.0)- 3*B[i])/(A[i] + S2);


	for (int i = n-10; i < n; i++) printf("devC[%d] = %.3f\n", i, C[i]);

	free(A);
	free(B);
	free(C);

	cudaEventRecord(stop, 0);
	cudaEventSynchronize(stop);
	cudaEventElapsedTime(&elapsedTime, start, stop);

	printf("\n\ntime = %.5f millisec\n", elapsedTime);

	return 0;
}

// Yi =(S1^m - 3Bi)/(Ai + S2)