#include <omp.h>
#include <stdio.h>

int main() {
    // Thiet lap so luong
    omp_set_num_threads(10);
    // Khoi tao moi truong da luong
    #pragma omp parallel
    {
    // tra ve id cua luong omp_get_thread_num()
    // tra ve tong so luong vat ly dang co omp_get_num_threads()
        printf("Hello from thread %d, nthreads %d\n",omp_get_thread_num(), omp_get_num_threads());
    }
    return 0;
}

