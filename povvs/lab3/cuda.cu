# include <stdio.h>

// # define n 4096

__global__ void Add( int n, float *A, float *B, float *C)
{
int idx = blockIdx.x * blockDim.x + threadIdx.x;
if(idx < n)
 C[idx]=A[idx]+B[idx];
}


int main ( int argc, char * argv [] ) {

	if (argc != 2) return 1;

	int n = atoi(argv[1]);
	printf("n = %d\n", n);

	// int i, steps;
	float *hA, *hB, *hC;
	float * devA, *devB, *devC;

	hA = (float *) malloc(n*sizeof(float));
	hB = (float *) malloc(n*sizeof(float));
	hC = (float *) malloc(n*sizeof(float));


	cudaMalloc ( &devA, n*sizeof(float));
	cudaMalloc ( &devB, n*sizeof(float));
	cudaMalloc ( &devC, n*sizeof(float));

	for (int i = 0; i < n; i++) {
		hA[i] = i;
		hB[i] = n-i;
	}

	// blocks=4; blocksize=64;
	// steps=(int)n/(blocks*blocksize);
	dim3 block(512);
	dim3 grid((n-1)/512 + 1);
	printf("block = %d, grid = %d, threads = %d\n", block.x, grid.x, block.x*grid.x);

	cudaMemcpy ( devA, hA, n*sizeof(float), cudaMemcpyHostToDevice);
	cudaMemcpy ( devB, hB, n*sizeof(float), cudaMemcpyHostToDevice);

	Add<<<grid, block>>> ( n, devA, devB, devC);

	cudaMemcpy ( hC, devC, n*sizeof(float), cudaMemcpyDeviceToHost );


	for (int i = n-10; i < n; i++) printf("devC[%d] = %.5f\n", i, hC[i]);

	cudaFree (devC);
	cudaFree (devA);
	cudaFree (devB);
	free(hA);
	free(hB);
	free(hC);

	return 0;
}