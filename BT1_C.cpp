#include <iostream>

using namespace std;

int n1,m1, n2,m2;

int main(){
    cout<<"Nhap kich thuoc ma tran A: ";
    cin>>n1>>m1;
    int **A = new int*[n1];
    for(int i=0;i<n1;i++) A[i] = new int[m1];
    cout<<"Nhap cac phan tu cua A:\n";
    for(int i=0;i<n1;i++)
        for(int j=0;j<m1;j++) cin>>A[i][j];

    cout<<"Nhap kich thuoc ma tran B (so hang cua B = so cot cua A): ";
    cin>>n2>>m2;
    n2 = m1;
    int **B = new int*[n2];
    for(int i=0;i<n2;i++) B[i] = new int[m2];
    for(int i=0;i<n2;i++)
        for(int j=0;j<m2;j++) cin>>B[i][j];
    
    // Tinh C
    int **C = new int*[n1];
    for(int i=0;i<n1;i++) C[i] = new int[m2];    

    for(int i=0;i<n1;i++) {
        for(int j=0;j<m2;j++){
            C[i][j]=0;
            for(int k=0;k<m1;k++) {
                C[i][j] += A[i][k]*B[k][j];
            }
        }
    }

    for(int i=0;i<n1;i++) {
        for(int j=0;j<m2;j++) cout<<C[i][j]<<' ';
        cout<<"\n";
    }
    return 0;
}