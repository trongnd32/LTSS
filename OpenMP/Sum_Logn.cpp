#include <cmath>
#include <iostream>
#include <omp.h>

using namespace std;

int n;
int *a = new int[100];

int main() {
  ios::sync_with_stdio(false);
  cout << "Nhap so phan tu cua mang: ";
  cin >> n;
  for (int i = 1; i <= n; i++)
    cin >> a[i];

  int threadId;
  int m = log2(n - 1) + 1;
  int numThreads = (n - 1) / 2 + 1;
  int prevNumThreads = n;
  for (int i = 1; i <= m; i++) {
    omp_set_num_threads(numThreads);
#pragma omp parallel private(threadId)
    {
      threadId = omp_get_thread_num() + 1;
      if (threadId + numThreads <= prevNumThreads)
        a[threadId] += a[threadId + numThreads];
    }
    prevNumThreads = numThreads;
    numThreads = max((numThreads - 1) / 2 + 1, 1);
  }
  cout << a[1] << "\n";
  return 0;
}