#include <iostream>
#include <omp.h>
using namespace std;
float D = 0.1, dx = 0.1, M = 20, Time = 1, dt = 0.01;
float *T = new float[1000];
float *dT = new float[1000];
void DHB2(float *T, float *dT)
{
    int i;
    float c, l, r;
    for (i = 0; i < M - 1; i++)
    {
        c = *(T + i);
        l = (i == 0) ? 100 : *(T + i - 1);
        r = (i == M - 1) ? 25 : *(T + i + 1);
        *(dT + i) = D * (l - 2 * c + r) / (dx * dx);
    }
}
void print(float* a, int num, FILE *fp) {
	for (int i = 0; i < num; i++){
		fprintf(fp, "%.2f", a[i]);
	}
	fprintf(fp, "\n");
}
int main()
{
    FILE *f = fopen("RESULT.txt", "w");
    for (int i = 0; i < M; i++) {
        T[i] = 25;
    }
    int t, Ntime = Time / dt, i;
    for (t = 0; t <= Ntime; t++)
    {
        DHB2(T, dT);
        for (i = 0; i < M; i++) {
            cout << *(dT + i) << " ";
            *(T + i) = *(T + i) + *(dT + i) * dt * 0.1;
        }
        cout << endl;
        print(T, (int) M, f);
    }
}