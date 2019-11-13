// Bai toan phuong trinh nhiet

#include <cstring>
#include <iostream>
#include <omp.h>

#define M 20
#define Time 1
#define dt 0.01
#define dx 0.1
#define D 0.1
#define threadsNum 4

using namespace std;

float *T = new float[1000];
float *dT = new float[1000];

void DHB2(float *T, float *dT, int start, int end) {
  int i;
  float c, l, r;

  for (i = start; i < end && i < M - 1; i++) {
    c = *(T + i);
    l = (i == 0) ? 100 : *(T + i - 1);
    r = (i == M - 1) ? 25 : *(T + i + 1);
    *(dT + i) = D * (l - 2 * c + r) / (dx * dx);
  }
}

int main() {
  for (int i = 0; i < M; i++)
    T[i] = 25;
  int NTime = Time / dt;
  int Mc = M / threadsNum;
  int start, end, threadId;
  omp_set_num_threads(threadsNum);
  for (int t = 0; t <= NTime; t++) {
#pragma omp parallel private(start, end, threadId)
    {
      threadId = omp_get_thread_num();
      start = threadId * Mc;
      end = min(start + Mc, M);
      DHB2(T, dT, start, end);
#pragma omp barrier
      for (int i = start; i < end; i++)
        *(T + i) = *(T + i) + *(dT + i) * dt;
    }
    for (int i = 0; i < M; i++)
      cout << T[i] << ' ';
    cout << endl;
  }
}