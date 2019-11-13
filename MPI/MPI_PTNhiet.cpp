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

  // const
  int M = 20;
  int Time = 1;
  float dt = 0.01;
  float dx = 0.1;
  float D = 0.1;
  //=======================
  int NTime = Time / dt;
  int Mc = M / size;

  float T[25];
  float TT[25];
  float dT[25];
  float kq[25];
  for (int i = 0; i < M; i++)
    T[i] = 25, dT[i] = 0;

  if (rank < size - 1) MPI_Send(&T[Mc - 1], 1, MPI_FLOAT, rank + 1, 2 * (rank + 1), MPI_COMM_WORLD);
  if (rank > 0) MPI_Send(&T[0], 1, MPI_FLOAT, rank - 1, 2 * (rank - 1) + 1, MPI_COMM_WORLD);

  for (int t = 0; t <= NTime; t++) {
    float c, l, r, L = 100, R = 25;
    if (rank > 0) MPI_Recv(&L, 1, MPI_FLOAT, rank - 1, 2 * rank + t * size * 2, MPI_COMM_WORLD, &stat);
    if (rank < size - 1) MPI_Recv(&R, 1, MPI_FLOAT, rank + 1, 2 * rank + 1 + t * size * 2, MPI_COMM_WORLD, &stat);

    for (int i = 0; i < Mc; i++) {
      c = T[i];
      if (i == 0) l = L;
      else l = T[i - 1];
      if (i == Mc - 1) r = R;
      else r = T[i + 1];

      dT[i] = 1.0 * D * (l - 2 * c + r) / (dx * dx);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    for (int i = 0; i < Mc; i++)
      T[i] = T[i] + 1.0 * dT[i] * dt;

    if (rank < size - 1) MPI_Send(&T[Mc - 1], 1, MPI_FLOAT, rank + 1, 2 * (rank + 1) + (t + 1) * size * 2, MPI_COMM_WORLD);
    if (rank > 0) MPI_Send(&T[0], 1, MPI_FLOAT, rank - 1, 2 * (rank - 1) + 1 + (t + 1) * size * 2, MPI_COMM_WORLD);

    MPI_Gather(T, Mc, MPI_FLOAT, kq, Mc, MPI_FLOAT, 0, MPI_COMM_WORLD);
    if (rank == 0) {
      for (int i = 0; i < M; i++) printf("%.2f ", kq[i]);
      printf("\n");
    }
  }

  MPI_Finalize();
}