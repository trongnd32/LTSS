#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>
#include <mpi.h>

using namespace std;

int main(int argc, char *argv[]) {
  int rank, size;

  // MPI
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Status stat;

  int *a = new int[100];
  int *c = new int[100];
  int *A = new int[100];
  int *B = new int[100];
  int *C = new int[100];
  int m = 5, n = 6, k = 4;
  // m*n, n*k => m*k
  int Mc = m / size;

  if (rank == 0) {
    // Nhap kich thuoc ma tran

    srand(time(NULL));
    for (int i = 0; i < m; i++) {
      for (int j = 0; j < n; j++) {
        *(A + i * n + j) = rand() % 15 + 1;
      }
    }

    for (int i = 0; i < n; i++) {
      for (int j = 0; j < k; j++) {
        *(B + i * k + j) = rand() % 15 + 1;
      }
    }
    for (int i = 1; i < size; i++)
      MPI_Send(B, n * k, MPI_INT, i, i, MPI_COMM_WORLD);
  } else {
    MPI_Recv(B, n * k, MPI_INT, 0, rank, MPI_COMM_WORLD, &stat);
  }

  MPI_Scatter(A, Mc * n, MPI_INT, a, Mc * n, MPI_INT, 0, MPI_COMM_WORLD);

  // Tinh
  for (int i = 0; i < Mc; i++) {
    for (int j = 0; j < k; j++) {
      *(c + i * k + j) = 0;
      for (int l = 0; l < n; l++)
        *(c + i * k + j) += (*(a + i * n + l)) * (*(B + l * k + j));
    }
  }
  //
  MPI_Gather(c, Mc * k, MPI_INT, C, Mc * k, MPI_INT, 0, MPI_COMM_WORLD);

  if (rank == 0) {
    for (int i = Mc * size; i < m; i++) {
      for (int j = 0; j < k; j++) {
        *(C + i * k + j) = 0;
        for (int l = 0; l < n; l++)
          *(C + i * k + j) += (*(A + i * n + l)) * (*(B + l * k + j));
      }
    }

    // In kq
    for (int i = 0; i < m; i++) {
      for (int j = 0; j < k; j++) {
        printf("%6d ", *(C + i * k + j));
      }
      printf("\n");
    }
  }
}