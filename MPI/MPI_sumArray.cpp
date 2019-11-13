#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>
#include <mpi.h>

using namespace std;

int main(int argc, char *argv[]) {
  int rank, size;
  MPI_Init(&argc, &argv);
  //
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Status stat;
  int *A = new int[105];
  int *B = new int[105];
  int *C = new int[105];
  int Mc = 99 / size + 1;
  if (rank == 0) {
    srand(time(NULL));
    for (int i = 0; i < 100; i++) {
      A[i] = rand() % 1000 + 1;
      B[i] = rand() % 1000 + 1;
      // printf("%d ", A[i] + B[i]);
    }
    // printf("\n");
    for (int i = 1; i < size; i++) {
      int start = i * Mc;
      MPI_Send(A + start, Mc, MPI_INT, i, i, MPI_COMM_WORLD);
      MPI_Send(B + start, Mc, MPI_INT, i, i + size, MPI_COMM_WORLD);
    }
    for (int i = 0; i < Mc; i++)
      C[i] = A[i] + B[i];
    for (int i = 1; i < size; i++) {
      int start = i * Mc;
      MPI_Recv(C + start, Mc, MPI_INT, i, i + 2 * size, MPI_COMM_WORLD, &stat);
    }
    for (int i = 0; i < 100; i++)
      printf("%d ", C[i]);
    printf("\n");
  } else {
    MPI_Recv(A, Mc, MPI_INT, 0, rank, MPI_COMM_WORLD, &stat);
    MPI_Recv(B, Mc, MPI_INT, 0, rank + size, MPI_COMM_WORLD, &stat);
    for (int i = 0; i < Mc; i++)
      C[i] = A[i] + B[i];
    MPI_Send(C, Mc, MPI_INT, 0, rank + 2 * size, MPI_COMM_WORLD);
  }
  MPI_Finalize();
  return 0;
}