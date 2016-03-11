# include <stdio.h>
# include <math.h>

__global__ void Add( int n, float *A, float *B, float *C, float S1, float S2, int steps) {
int idx = blockIdx.x * blockDim.x + threadIdx.x;
	if(idx < n){
		for (int i = 0; i < steps; i++){
			C[steps*idx+i]=(powf(S1,3.0)- 3*B[steps*idx+i])/(A[steps*idx+i] + S2); //powf(S1,3.0)
		}
	}
}


int main ( int argc, char * argv [] ) {

	if (argc != 4){
		printf("To few args\n");
		return 1;
	}

	int n = atoi(argv[1]);
	printf("n = %d\n", n);

	int steps;
	float *hA, *hB, *hC;
	// float hS1, hS2;
	float *devA, *devB, *devC;
	float devS1, devS2, elapsedTime;
	cudaEvent_t start, stop;

	cudaEventCreate(&start);
	cudaEventCreate(&stop);
	elapsedTime = 0.0f;
	cudaEventRecord(start, 0);

	hA = (float *) malloc(n*sizeof(float));
	hB = (float *) malloc(n*sizeof(float));
	hC = (float *) malloc(n*sizeof(float));


	cudaMalloc ( &devA, n*sizeof(float));
	cudaMalloc ( &devB, n*sizeof(float));
	cudaMalloc ( &devC, n*sizeof(float));

	// cudaMalloc (&devS1, sizeof(float));
	// cudaMalloc (&devS2, sizeof(float));

	for (int i = 0; i < n; i++) {
		hA[i] = i;
		hB[i] = n-i;
	}
	devS1 = 15.4;
	devS2 = 4.4;


	// blocks=4; blocksize=64;
	dim3 block(atoi(argv[3]));
	dim3 grid(atoi(argv[2]));
	steps=(int)n/(atoi(argv[3])*atoi(argv[2]));

	// dim3 block(512);
	// dim3 grid((n-1)/512 + 1);

	printf("block = %d, grid = %d, threads = %d, steps = %d\n", block.x, grid.x, block.x*grid.x, steps);




	cudaMemcpy ( devA, hA, n*sizeof(float), cudaMemcpyHostToDevice);
	cudaMemcpy ( devB, hB, n*sizeof(float), cudaMemcpyHostToDevice);


	Add<<<grid, block>>> ( n, devA, devB, devC, devS1, devS2, steps);

	cudaMemcpy ( hC, devC, n*sizeof(float), cudaMemcpyDeviceToHost );


	for (int i = n-10; i < n; i++) printf("devC[%d] = %.3f\n", i, hC[i]);

	cudaFree (devC);
	cudaFree (devA);
	cudaFree (devB);
	free(hA);
	free(hB);
	free(hC);



	cudaEventRecord(stop, 0);
	cudaEventSynchronize(stop);
	cudaEventElapsedTime(&elapsedTime, start, stop);

	printf("\n\ntime = %.5f millisec\n", elapsedTime);

	return 0;
}

// Yi =(S1^m - 3Bi)/(Ai + S2)