#include<iostream>
#include<mpi.h>
#include<math.h>
#include<stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/time.h>
void initMatrixWithRV(float *A, int rows, int cols);
void matMultiplyWithSingleThread(float *A, float *B, float *matResult, int m, int p, int n);
void my_bcast(void* data, int count, MPI_Datatype datatype, int root, MPI_Comm communicator) ;
int main(int argc, char** argv)
{
    int m = atoi(argv[1]);
    int p = atoi(argv[2]);
    int n = atoi(argv[3]);
    
    float *A, *B, *C;
    float *bA, *bC;  

    int myrank, numprocs;

    MPI_Status status;
    time_t tt0=clock();  
    MPI_Init(&argc, &argv); 
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs); 
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank); 
    
    int bm = m / numprocs;

    bA = new float[bm * p];
    B  = new float[p * n];
    bC = new float[bm * n];

    if(myrank == 0){
        A = new float[m * p];
        C = new float[m * n];
        
        initMatrixWithRV(A, m, p);
        initMatrixWithRV(B, p, n);
    }

    MPI_Barrier(MPI_COMM_WORLD);

    MPI_Scatter(A, bm * p, MPI_FLOAT, bA, bm *p, MPI_FLOAT, 0, MPI_COMM_WORLD);
    my_bcast(B, p * n, MPI_FLOAT, 0, MPI_COMM_WORLD);

    matMultiplyWithSingleThread(bA, B, bC, bm, p, n);
    
    MPI_Barrier(MPI_COMM_WORLD);
    
    MPI_Gather(bC, bm * n, MPI_FLOAT, C, bm * n, MPI_FLOAT, 0, MPI_COMM_WORLD);
  
    int remainRowsStartId = bm * numprocs;
    if(myrank == 0 && remainRowsStartId < m){
        int remainRows = m - remainRowsStartId;
        matMultiplyWithSingleThread(A + remainRowsStartId * p, B, C + remainRowsStartId * n, remainRows, p, n);
    }
  
    delete[] bA;
    delete[] B;
    delete[] bC;
    
    if(myrank == 0){
        delete[] A;
        delete[] C;
    }
    
    MPI_Finalize(); 
    time_t tt1=clock();
    printf("rank %d        CPU 时间:%.6lf seconds\n", myrank, (double)(tt1-tt0)/CLOCKS_PER_SEC);
    return 0;
}

void initMatrixWithRV(float *A, int rows, int cols)
{
    srand((unsigned)time(NULL));
    for(int i = 0; i < rows*cols; i++){
        A[i] = (float)rand() / RAND_MAX;
    }
}
void matMultiplyWithSingleThread(float *A, float *B, float *matResult, int m, int p, int n)
{
    for(int i=0; i<m; i++){
        for(int j=0; j<n; j++){
            float temp = 0;
            for(int k=0; k<p; k++){
                temp += A[i*p+k] * B[k*n + j];
            }
            matResult[i*n+j] = temp;
        }
    }
}
void my_bcast(void* data, int count, MPI_Datatype datatype, int root, MPI_Comm communicator) {
  int world_rank;
  MPI_Comm_rank(communicator, &world_rank);
  int world_size;
  MPI_Comm_size(communicator, &world_size);

  if (world_rank == root) {
    // If we are the root process, send our data to everyone
    int i;
    for (i = 0; i < world_size; i++) {
      if (i != world_rank) {
        MPI_Send(data, count, datatype, i, 0, communicator);
      }
    }
  } else {
    // If we are a receiver process, receive the data from the root
    MPI_Recv(data, count, datatype, root, 0, communicator,
             MPI_STATUS_IGNORE);
  }
}