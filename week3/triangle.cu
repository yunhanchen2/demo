#include <iostream>
#include <fstream>
using namespace std;

#define MVnum 1100

int arcs[MVnum][MVnum];

//kernal
__global__ void find_the_same(int *c,int *arr,int *n){
    int i=blockIdx.x*blockDim.x+threadIdx.x;
    int j=blockIdx.y*blockDim.y+threadIdx.y;
    if(i<j&&j<(*n)){
        if(arr[i*(*n)+j]==1){
            for(int k=j+1;k<(*n);k++){
                if(arr[k*(*n)+j]==1&&arr[i*(*n)+k]==1){
                    (*c)++;
                }
            }
        }
    }
}

struct AMGraph{
    int vexnum;
    int edgnum;
};

void init(AMGraph &G,int n1,int n2){
    G.vexnum=n1;
    G.edgnum=n2;
    //init matrix
    for(int i=0;i<G.vexnum;i++){
        for(int j=0;j<G.vexnum;j++){
            arcs[i][j]=0;
        }
    }
}

void Create(AMGraph &G,int v1,int v2){
    int i=v1-1;
    int j=v2-1;
    arcs[i][j]=1;
    arcs[j][i]=1;
}

int main() {
    //set the size to allocate
    const int arr_size=MVnum*MVnum*sizeof(int);
    const int c_size= sizeof(int);

    //init the graph and values
    AMGraph G;
    int v1,v2,n1,n2;
    ifstream fin;
    fin.open("karate_edges.txt");
    fin>>n1>>n2;
    init(G,n1,n2);
    while(fin>>v1>>v2){
        Create(G,v1,v2);
    }

    int co=0;
    int *count=&co;
    
    int n00=G.vexnum;
    int *n0=&n00;

    //declear GPU memory pointers
    int *c;
    int *arr;
    int *n;

    //allocate GPU memory
    cudaMalloc((void**)&arr,arr_size);
    cudaMalloc((void**)&c,c_size);
    cudaMalloc((void**)&n,c_size);

    //transfer the data from CPU to GPU
    cudaMemcpy(c,count,c_size,cudaMemcpyHostToDevice);
    cudaMemcpy(arr,arcs,arr_size,cudaMemcpyHostToDevice);
    cudaMemcpy(n,n0,c_size,cudaMemcpyHostToDevice);

    //launch the kernal
    int m=G.vexnum/32+1;
    find_the_same<<<dim3(m,m,1),dim3(32,32,1)>>>(c,arr,n);

    //copy back the result from GPU to CPU
    cudaMemcpy(count,c,c_size,cudaMemcpyDeviceToHost);

    //print out the result
    cout<<"the number of triangle is "<<*count<<endl;

    //free the memory
    cudaFree(c);
    cudaFree(arr);
    cudaFree(n);

    return 0;
}
