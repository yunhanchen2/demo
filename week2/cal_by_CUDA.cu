#include <iostream>
#include <chrono>
#include <cstdlib>
using namespace std;
using namespace chrono;

__global__ void mul(float *d_a, float *d_b, float *d_c, int N){
    int i=blockIdx.x*blockDim.x+threadIdx.x;
    int j=blockIdx.y*blockDim.y+threadIdx.y;
    for(int p=0;p<N;p++){
        d_c[i*N + j] = d_c[i*N + j] + d_a[i*N + p]*d_b[p*N + j];
    }
}

int main(int argc, char * argv[]) {
    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);
    cudaEventRecord(start);

    int n=atoi(argv[1]);

    const float s= sizeof(float)*n*n;

    float *h_a=new float[n*n];
    float *h_b=new float[n*n];
    float *h_c=new float[n*n];

    srand((unsigned)time(NULL));

    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            h_a[i*n+j]=rand();
        }
    }
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            h_b[i*n+j]=rand();
        }
    }


    float *d_a;
    float *d_b;
    float *d_c;

    cudaMalloc((void**)&d_a,s);
    cudaMalloc((void**)&d_b,s);
    cudaMalloc((void**)&d_c,s);

    cudaMemcpy(d_a,h_a,s,cudaMemcpyHostToDevice);
    cudaMemcpy(d_b,h_b,s,cudaMemcpyHostToDevice);

    int m=n/1024;
    int r=n%1024;
    for(int i=0;i<m;i++){
        for(int j=0;j<1024;j++)
            mul<<<dim3(1,1,1),dim3(32,32,1)>>>(d_a,d_b,d_c,n);
    }
    for(int i=0;i<r;i++){
        mul<<<dim3(1,1,1),dim3(32,32,1)>>>(d_a,d_b,d_c,n);
    }

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