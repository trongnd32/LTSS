#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>
#include <cmath>
#include <mpi.h>

using namespace std;

void Write2File(float *A, int row, int col) {
  FILE *result = fopen("ViberStringC1_MPI.txt", "a");
  int i, j;
  for (i = 0; i < row; i++) {
    for (j = 0; j < col; j++) {
      fprintf(result, "%8.5lf", *(A + i * col + j));
    }
    fprintf(result, " ");
  }
  fclose(result);
}

int main(int argc, char *argv[]) {
  int rank, size;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Status stat;
  
  // const
  int M = 200;
  float dt = 0.01;
  float Time = 0.15;
  float dx = 1.0 / (M - 1);
  float C = 1;
  float k = 1;
  int numThreads = 4;
  int Mc = M / numThreads;
  float tau = C * dt / dx;

  // Phi
  float *Phi_new = new float[Mc];
  float *Phi = new float[Mc];
  float *Phi_old = new float[Mc];
  float *result = new float[M];

  // init
  for (int i = 0; i < Mc; i++) {
    Phi[i] = sin(2.0 * k * M_PI * (rank * Mc + i) * dx);
    Phi_old[i] = Phi[i];
  }

  // print result to file
  FILE *resultFile = fopen("ViberStringC1_MPI.txt", "a");

  // calculate
  int N = Time/dt;

  if (rank < size - 1) MPI_Send(&Phi[Mc - 1], 1, MPI_FLOAT, rank + 1, 2 * (rank + 1), MPI_COMM_WORLD);
  if (rank > 0) MPI_Send(&Phi[0], 1, MPI_FLOAT, rank - 1, 2 * (rank - 1) + 1, MPI_COMM_WORLD);

  for(int t = 0; t < N; t++){
    float L = 0, R = 0;
    if(rank > 0) MPI_Recv(&L, 1, MPI_FLOAT, rank - 1, 2 * rank + t * size * 2, MPI_COMM_WORLD, &stat);
    if (rank < size - 1) MPI_Recv(&R, 1, MPI_FLOAT, rank + 1, 2 * rank + 1 + t * size * 2, MPI_COMM_WORLD, &stat);

    for (int i = 0; i < Mc; i++) {
      float l = L, r = R, c = Phi[i];
      if (i > 0) l = Phi[i - 1];
      if (i == 0 && rank == 0) l = -Phi[i + 1];
      if (i < Mc - 1) r = Phi[i + 1];
      if (i == Mc - 1 && rank == size-1) r = -Phi[i - 1];

      Phi_new[i] = 2 * c - Phi_old[i] + tau * tau * (l - 2 * c + r);
    }
    for(int i = 0; i < Mc; i++) {
      Phi_old[i] = Phi[i];
      Phi[i] = Phi_new[i];
    }

    if (rank < size - 1) MPI_Send(&Phi[Mc - 1], 1, MPI_FLOAT, rank + 1, 2 * (rank + 1) + (t + 1) * size * 2, MPI_COMM_WORLD);
    if (rank > 0) MPI_Send(&Phi[0], 1, MPI_FLOAT, rank - 1, 2 * (rank - 1) + 1 + (t + 1) * size * 2, MPI_COMM_WORLD);

    //write step by step to file
    MPI_Gather(Phi, Mc, MPI_FLOAT, result, Mc, MPI_FLOAT, 0, MPI_COMM_WORLD);
    if(rank == 0) {
      for (int i = 0; i < M; i++) {
        fprintf(resultFile, "%8.4lf ", *(result + i));
      }
      if(t < N - 1) fprintf(resultFile, "%s", ";\n");
    }
  }


  // print result
  // if(rank == 0){
  //   for(int i = 0; i < M;i++){
  //     printf("%f\n", result[i]);
  //   }
  //   printf("\n");
  //   Write2File(result,M,1);
  // }

  MPI_Finalize();
}