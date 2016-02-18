#include <stdio.h>
#include "mpi.h"
const int S1 = 5;
const int S2 = 6;
const int N1=4096;
float Y[4096] ;
float A[4096] ;
float B[4096] ;
float C[4096] ;

void initializeArray(float arr[N1],float k)
 {
	for (int i=0;i<N1;i++)
	{
		arr[i]=i+k;
	}
 }
 void main(int argc, char ** argv)
{
	int rank, size, cicle;
	int i;
	double time,time1,time2;
	MPI_Status status; // �������� ���������� � �������� ���������: ��� �������������, ����� ������-�����������, ��� ���������� � ���������� ���������� ��������� ������
	MPI_Init (&argc, &argv);       
	MPI_Comm_rank (MPI_COMM_WORLD, &rank);  // ����� ������
	MPI_Comm_size (MPI_COMM_WORLD, &size);  //���������� �����
	printf("Hello(%d)-%d\n",rank,size);
	if(rank==0)
	{
		//������������� �������� ������
			initializeArray(&A[N1],3);
			initializeArray(&B[N1],2);
			initializeArray(&C[N1],1);
		time1=MPI_Wtime(); //����������� ������� ������ ���������
		for (cicle=0;cicle<10000; cicle++) // ����� ���������
		{
		for (i=1;i<size;i++) // ������������� ��������� ����� 
		{
		MPI_Send(&B[(int)i*N1/size],(int)N1/size,MPI_FLOAT,i,0,MPI_COMM_WORLD);
		MPI_Send(&A[(int)i*N1/size],(int)N1/size,MPI_FLOAT,i,0,MPI_COMM_WORLD);
		MPI_Send(&C[(int)i*N1/size],(int)N1/size,MPI_FLOAT,i,0,MPI_COMM_WORLD);
		}

		//����������
		for (i=0;i<N1/size;i++)
		{	
			Y[i]=0;
			Y[i]+=(S1+S2)*B[i]+A[i]/C[i];
		}
		
			for (i=1;i<size;i++) // ���� ����������� � ������ C
				MPI_Recv(&Y[(int)i*N1/size],(int)N1/size,MPI_FLOAT,i,0,MPI_COMM_WORLD, &status);		
			} // ����� ����� ���������
	time2=MPI_Wtime();//����������� ������� ����� ���������
		time=time2-time1;

		for (i=0;i<N1;i++) printf("%f\n",Y[i]); 	//����� �����������
		if (rank==0)printf("\nTIME=%f\n",time); 	// ����� ������� ����������
	}
	else
	{
		initializeArray(&A[N1],3);
		initializeArray(&C[N1],1);
		for (cicle=0;cicle<10000; cicle++) // ����� ���������
		{
			MPI_Recv(&B[(int)rank*N1/size],(int)N1/size,MPI_FLOAT,0,0,MPI_COMM_WORLD, &status); // ���� ��������� ������� B �� �������� ��������
			MPI_Recv(&A[(int)rank*N1/size],(int)N1/size,MPI_FLOAT,0,0,MPI_COMM_WORLD, &status);
			MPI_Recv(&C[(int)rank*N1/size],(int)N1/size,MPI_FLOAT,0,0,MPI_COMM_WORLD, &status);

			for (i=rank*N1/size;i<(rank+1)*N1/size;i++)
			{
				////����������		
				Y[i]=0;
				Y[i]+=(S1+S2)*B[i]+A[i]/C[i];
			}
			// �������� ����������� �������� ��������
			MPI_Send(&Y[(int)rank*N1/size],(int)N1/size,MPI_FLOAT,0,0,MPI_COMM_WORLD);		
		}
	}
	MPI_Finalize();
}
