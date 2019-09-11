#include <omp.h>
#include <iostream>
#include <algorithm>
#include <cmath>

using namespace std;

int main(){
    int thread = 1;
    int n;
    printf("Nhap so luong: ");
    scanf("%d",&thread);
    omp_set_num_threads(thread);

    printf("So phan tu cua mang: ");
    scanf("%d", &n);
    int m = int(n/thread);
    m = (n%thread) ? m+1:m;

    int *A = new int[n];
    int *B = new int[n];
    int *C = new int[n];

    printf("Nhap cac phan tu mang A: ");
    for(int i=0;i<n;i++) scanf("%d",&A[i]);

    printf("Nhap cac phan tu mang B: ");
    for(int i=0;i<n;i++) scanf("%d",&B[i]);

    int threadId, start, end;
    #pragma omp parallel private(start, end, threadId)
    {   
        threadId = omp_get_thread_num();
        start = m*threadId;
        end = min(n,m*(threadId+1));
        // printf("from %d %d %d\n",threadId, start, end);
        for(int i=start;i<end;i++) {
            C[i]=A[i]+B[i];
        }
    }
    for(int i=0;i<n;i++) cout<<C[i]<<' ';
    return 0;
}