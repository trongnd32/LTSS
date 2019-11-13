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
  int *b = new int[100];
  int *c = new int[100];
  int *A = new int[100];
  int *B = new int[100];
  int *C = new int[100];
  int m,n,Mc;

  if (rank == 0) {
    // Nhap kich thuoc ma tran
    cout << "Nhap so hang: ";
    cin >> m;
    cout << "\nNhap so cot: ";
    cin >> n;
    cout << "\n";
    for (int i = 1; i < size; i++) {
      MPI_Send(&m, 1, MPI_INT, i, i, MPI_COMM_WORLD);
      MPI_Send(&n, 1, MPI_INT, i, i + size, MPI_COMM_WORLD);
    }
    Mc = m / size;
    srand(time(NULL));
    for (int i = 0; i < m; i++) {
      for (int j = 0; j < n; j++) {
        *(A+i*n+j) = rand() % 1000 + 1;
        *(B+i*n+j) = rand() % 1000 + 1;
        //printf("%6d ", *(A+i*n+j)+*(B+i*n+j));
      }
      //printf("\n");
    }
    //printf("\n");
  } else {
    MPI_Recv(&m, 1, MPI_INT, 0, rank, MPI_COMM_WORLD, &stat);
    MPI_Recv(&n, 1, MPI_INT, 0, rank + size, MPI_COMM_WORLD, &stat);
    Mc = m / size;
  }
  MPI_Scatter(A, Mc*n, MPI_INT, a, Mc*n, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Scatter(B, Mc*n, MPI_INT, b, Mc*n, MPI_INT, 0, MPI_COMM_WORLD);
  for(int i=0;i<Mc*n;i++) C[i]=a[i]+b[i];
  MPI_Gather(C, Mc*n,MPI_INT, c, Mc*n, MPI_INT, 0, MPI_COMM_WORLD);
  if(rank==0) {
    for(int i=Mc*size*n;i<m*n;i++) c[i]=A[i]+B[i];
    for (int i = 0; i < m; i++) {
      for (int j = 0; j < n; j++) {
        printf("%6d ", *(c+i*n+j));
      }
      printf("\n");
    }
  }
}