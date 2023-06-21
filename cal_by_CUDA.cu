#include <iostream>
#include <chrono>

using namespace std;
using namespace chrono;

#define n 100

float h_a[n][n]={0};
float h_b[n][n]={0};
float h_c[n][n]={0};

__global__ void mul(float *d_a, float *d_b, float *d_c, int N){
    int i=blockIdx.x*blockDim.x+threadIdx.x;
    int j=blockIdx.y*blockDim.y+threadIdx.y;
    for(int p=0;p<N;p++){
        d_c[i*N + j] = d_c[i*N + j] + d_a[i*N + p]*d_b[p*N + j];
    }
}

int main() {
    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);
    cudaEventRecord(start);
    const float s= sizeof(float)*n*n;

    float *d_a;
    float *d_b;
    float *d_c;

    cudaMalloc((void**)&d_a,s);
    cudaMalloc((void**)&d_b,s);
    cudaMalloc((void**)&d_c,s);

    cudaMemcpy(d_a,h_a,s,cudaMemcpyHostToDevice);
    cudaMemcpy(d_b,h_b,s,cudaMemcpyHostToDevice);

    mul<<<dim3(1,1,1),dim3(n,n,1)>>>(d_a,d_b,d_c,n);

    cudaMemcpy(h_c,d_c,s,cudaMemcpyDeviceToHost);

    cudaFree(d_a);
    cudaFree(d_b);
    cudaFree(d_c);

    cudaEventRecord(stop);
    cudaEventSynchronize(stop);
    float milliseconds = 0;
    cudaEventElapsedTime(&milliseconds, start, stop);
    cout << milliseconds;

    return 0;
}