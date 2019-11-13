#include <iostream>
#include <omp.h>
#include <algorithm>

using namespace std;

int thread = 1, threadId;
int n1, m1, n2, m2;
int m;

// Nhan ma tran su dung da luong chia block theo luoi caro 12x12

int main(){
  // Nhap so thread
  cout<<"Nhap so thread: ";
  cin>>thread;

  //Nhap ma tran A
  cout<<"Nhap kich thuoc ma tran A: ";
  cin>>n1>>m1;
  int **A = new int*[n1];
  for(int i=0;i<n1;i++) A[i] = new int[m1];
  cout<<"Nhap cac phan tu cua A:\n";
  for(int i=0;i<n1;i++)
      for(int j=0;j<m1;j++) cin>>A[i][j];

  // Nhap ma tran B
  cout<<"Nhap kich thuoc ma tran B (so hang cua B = so cot cua A): ";
  cin>>n2>>m2;
  n2 = m1;
  int **B = new int*[n2];
  for(int i=0;i<n2;i++) B[i] = new int[m2];
  for(int i=0;i<n2;i++)
      for(int j=0;j<m2;j++) cin>>B[i][j];

  // Tinh toan
  int **C = new int*[n1];
  for(int i=0;i<n1;i++) C[i] = new int[m2];   

  int startRow = 0, endRow = 0;
  int startCol = 0, endCol = 0;
  omp_set_num_threads(thread);
  int width = 

  #pragma omp parallel private(threadId, startRow, endRow, startCol, endCol)
  {   
      threadId = omp_get_thread_num();
      
      
      for(int i=start;i<end;i++) {
        for(int j=0;j<m2;j++){
            C[i][j]=0;
            for(int k=0;k<m1;k++) {
                C[i][j] += A[i][k]*B[k][j];
            }
        }
    }
  }

  for(int i=0;i<n1;i++) {
    for(int j=0;j<m2;j++) cout<<C[i][j]<<' ';
    cout<<"\n";
  }
  return 0;
}