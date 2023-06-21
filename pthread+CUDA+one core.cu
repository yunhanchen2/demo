#include <iostream>

using namespace std;

#define n 100


__global__ void mul(float *d_a,float *d_b,float *d_c,int n){
    int i=blockIdx.x*blockDim.x+threadIdx.x;
    int j=blockIdx.y*blockDim.y+threadIdx.y;

    for(int p=0;p<n;p++){
        d_c[i][j]=d_c[i][j]+d_a[i][p]*d_b[p][j];
    }
}

int main() {
    const int s= sizeof(float)*n*n;

    int h_a[n][n]={0};
    int h_b[n][n]={0};
    int h_c[n][n]={0};

    float (*d_a)[n];
    float (*d_b)[n];
    float (*d_c)[n];

    cudaMalloc((void**)&d_a,s);
    cudaMalloc((void**)&d_b,s);
    cudaMalloc((void**)&d_c,s);

    cudaMemcpy(d_a,h_a,s,cudaMemcopyHostToDevice);
    cudaMemcpy(d_b,h_b,s,cudaMemcopyHostToDevice);

    mul<<<dim3(1,1,1),dim3(n,n,1)>>>(d_a,d_b,d_c,n);

    cudaMemcpy(h_c,d_c,s,cudaMemcopyDeviceToHost);

    cudaFree(d_a);
    cudaFree(d_b);
    cudaFree(d_c);



    return 0;
}