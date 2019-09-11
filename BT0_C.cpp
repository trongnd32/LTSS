#include <iostream>

using namespace std;

int n;

int main(){
    printf("Nhap so phan tu cua mang: ");
    scanf("%d",&n);
    int *A = new int[n];
    int *B = new int[n];
    printf("Nhap cac phan tu mang A: ");
    for(int i=0;i<n;i++) scanf("%d",&A[i]);
    printf("Nhap cac phan tu mang B: ");
    for(int i=0;i<n;i++) scanf("%d",&B[i]);
    int *C = new int[n];
    for(int i=0;i<n;i++) C[i]=A[i]+B[i];
    printf("Ket qua mang C: ");
    for(int i=0;i<n;i++) printf("%d ",C[i]);
    printf("\n");    
    delete A;
    delete B;
    delete C;
}