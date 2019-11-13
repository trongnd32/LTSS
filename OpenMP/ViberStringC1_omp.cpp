#include <algorithm>
#include <cmath>
#include <cstdio>
#include <ctime>
#include <iostream>
#include <malloc.h>
#include <omp.h>
#include <stdlib.h>

using namespace std;

#define C 1
#define m 20
#define T 5
#define dt 0.01
#define threadsNum 4
//=========================
void DisplayMatrix(float *A, int row, int col) {
  int i, j;
  for (i = 0; i < row; i++) {
    for (j = 0; j < col; j++)
      printf("  %f", *(A + i * col + j));
    printf("\n");
  }
  printf("\n");
}
//=========================
void Write2File(float *A, int row, int col) {
  FILE *result = fopen("ViberStringC1_omp.txt", "a");
  int i, j;
  for (i = 0; i < row; i++) {
    for (j = 0; j < col; j++) {
      fprintf(result, "%lf\t", *(A + i * col + j));
    }
    fprintf(result, "\n");
  }
  fclose(result);
}
//=========================
void KhoiTao(float *Phi_old, float *Phi_current) {
  int i;
  float dx, X;
  dx = 1.0 / (m - 1);
  for (i = 0; i < m; i++) {
    X = i * dx;
    *(Phi_old + i) = sin(2.0 * M_PI * X);
    *(Phi_current + i) = *(Phi_old + i);
  }
}
//=========================
/*void FD(float *Phi_current,float *dPhi)
{
  int i;
  float c,l,r;
  for (i = 0 ; i < m ; i++){
    c = *(Phi_current+i);
    l = (i==0)    ? - *(Phi_current+i+1) : *(Phi_current+i-1);
    //left neighbor = -r to dPhi=0
    r = (i==m-1) ? - *(Phi_current+i-1) : *(Phi_current+i+1);
    //right neighbor = -l to dPhi=0
    *(dPhi+i) = l - 2*c + r;
  }
}*/
//=========================
int main() {
  float dx, t, tau;
  t = 0.0;
  float *Phi_old, *Phi_current, *Phi_new, *dPhi;
  Phi_old = (float *)malloc(m * sizeof(float));
  Phi_current = (float *)malloc(m * sizeof(float));
  Phi_new = (float *)malloc(m * sizeof(float));
  dPhi = (float *)malloc(m * sizeof(float));
  KhoiTao(Phi_old, Phi_current);
  Write2File(Phi_current, m, 1);
  // printf("Gia tri khoi tao:\n");
  // DisplayMatrix(Phi_current, m, 1);
  dx = 1.0 / (m - 1);
  tau = C * dt / dx;
  //
  int start, end, threadId;
  int Mc = m / threadsNum;
  omp_set_num_threads(threadsNum);
  while (t <= T) {
//
#pragma omp parallel private(threadId, start, end)
    {
      threadId = omp_get_thread_num();
      start = threadId * Mc;
      end = min(start + Mc, m);
      // printf("From %d s = %d e = %d \n", threadId, start, end);
      for (int i = start; i < end; i++) {
        float c = *(Phi_current + i);
        float l = (i == 0) ? -*(Phi_current + i + 1) : *(Phi_current + i - 1);
        // left neighbor = -r to dPhi=0
        float r =
            (i == m - 1) ? -*(Phi_current + i - 1) : *(Phi_current + i + 1);
        // right neighbor = -l to dPhi=0
        *(dPhi + i) = l - 2 * c + r;
      }
#pragma omp barrier
      //
      for (int i = start; i < end; i++)
        *(Phi_new + i) = 2 * (*(Phi_current + i)) - *(Phi_old + i) +
                         tau * tau * (*(dPhi + i));
#pragma omp barrier
      for (int i = start; i < end; i++) {
        *(Phi_old + i) = *(Phi_current + i);
      }
#pragma omp barrier
      for (int i = start; i < end; i++) {
        *(Phi_current + i) = *(Phi_new + i);
      }
    }
    // DisplayMatrix(dPhi, m, 1);

    t = t + dt;
    Write2File(Phi_current, m, 1);
  }
  printf("Gia tri cuoi cung:\n");
  DisplayMatrix(Phi_current, m, 1);
  //
  return 0;
}
