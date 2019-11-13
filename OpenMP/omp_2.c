#include <omp.h>
#include <stdio.h>

int main(){
    int id, x;
    omp_set_num_threads(100);
    #pragma omp parallel
    {
        id = omp_get_thread_num();
        x = id*10;
        printf("\n");
        printf("Hello from thread %d, x = %d", id,x);
        printf("\n");
    }
    return 0;
}