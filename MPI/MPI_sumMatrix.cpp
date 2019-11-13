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

  if (rank == 0) {
    int m,n;
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
    int Mc = m / size;

    // Tao 2 ma tran A, B
    int *A = new int[m*n];
    int *B = new int[m*n];

    srand(time(NULL));
    for (int i = 0; i < m; i++) {
      for (int j = 0; j < n; j++) {
        *(A+i*n+j) = rand() % 1000 + 1;
        *(B+i*n+j) = rand() % 1000 + 1;
        printf("%6d ", *(A+i*n+j)+*(B+i*n+j));
      }
      printf("\n");
    }
    printf("\n");

    // Khoi tao C
    int *C = new int[m*n];

    // Tinh 0 -> Mc0
    int Mc0 = m - (size - 1) * Mc;
    for (int i = 0; i < Mc0; i++)
      for (int j = 0; j < n; j++)
        *(C+ i*n + j) = *(A+i*n+j) + *(B+i*n+j);

    // debug
    // for(int i=0;i<Mc0;i++){
    //   for(int j=0;j<n;j++) printf("%6d ",C[i][j]);
    //   printf("\n");
    // }
    // printf("\n");


    // Gui du lieu toi thread khac
    for (int i = 1; i < size; i++) {
      MPI_Send(A + (Mc0 + (i - 1) * Mc) * n, Mc * n, MPI_INT, i, i + size * 2, MPI_COMM_WORLD);
      MPI_Send(B + (Mc0 + (i - 1) * Mc) * n, Mc * n, MPI_INT, i, i + size * 3, MPI_COMM_WORLD);
    }

    // Nhan C tu thread khac
    for (int i = 1; i < size; i++) {
      MPI_Recv(C + (Mc0 + (i - 1) * Mc) * n, Mc * n, MPI_INT, i, i + size * 4, MPI_COMM_WORLD, &stat);
    }
    // In ket qua
    for (int i = 0; i < m; i++) {
      for (int j = 0; j < n; j++) {
        printf("%6d ", *(C+i*n+j));
      }
      printf("\n");
    }
  } else {
    int n, m;
    MPI_Recv(&m, 1, MPI_INT, 0, rank, MPI_COMM_WORLD, &stat);
    MPI_Recv(&n, 1, MPI_INT, 0, rank + size, MPI_COMM_WORLD, &stat);
    int Mc = m / size;
    // Tao A,B
    int *A = new int[Mc*n];
    
    int *B = new int[Mc*n];
    // Tao C
    int *C = new int[Mc*n];

    // Nhan du lieu tu thread 0
    MPI_Recv(A, Mc * n, MPI_INT, 0, rank + size * 2, MPI_COMM_WORLD, &stat);
    MPI_Recv(B, Mc * n, MPI_INT, 0, rank + size * 3, MPI_COMM_WORLD, &stat);

    for (int i = 0; i < Mc; i++) {
      for (int j = 0; j < n; j++){
        *(C+i*n+j) = *(A+i*n+j) + *(B+i*n+j);
        // printf("%6d \n",*(A+i*n+j) + *(B+i*n+j));
      }
    }
    MPI_Send(C, Mc * n, MPI_INT, 0, rank + size * 4, MPI_COMM_WORLD);
  }

  MPI_Finalize();
}