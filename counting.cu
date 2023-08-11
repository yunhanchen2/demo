#include <iostream>
#include <fstream>

using namespace std;

struct AMGraph{
    int vexnum;
    int edgnum;
};


//initialize array
void init(AMGraph &G,int n1,int n2){
    G.vexnum=n1;
    G.edgnum=n2;
}

void get_matrix(AMGraph &G,int v1,int v2,int * matx){
    int i=v1-1;
    int j=v2-1;
    matx[i*G.vexnum+j]=1;
    matx[j*G.vexnum+i]=1;
}

__global__ void count(int *d_mark,int *matx,int *vexnum){
    int i=blockIdx.x*blockDim.x+threadIdx.x;
    int ind=0;
    int *arr_tmp=new int[vexnum[0]];
    //get neighbor
    for(int j=0;j<vexnum[0];j++){
        if(matx[i*vexnum[0]+j]==1){
            arr_tmp[ind]=j;
            ind++;
        }
    }
    //count other
    d_mark[i]=0;
    for(int k=i;k<vexnum[0];k++){
        for(int p=k;p<vexnum[0];p++){
            for(int q=p;q<vexnum[0];q++){
                if(matx[k*vexnum[0]+p]+matx[k*vexnum[0]+q]+matx[p*vexnum[0]+q]==3){
                    d_mark[i]+=1;
                }
            }
        }
    }
}

int main() {
    AMGraph G;
    ifstream fin;
    fin.open("karate_edges.txt");
    int m1,m2,n1,n2;
    fin>>n1>>n2;
    init(G,n1,n2);

    //get the matrix first
    int *matx=new int[G.vexnum*G.vexnum];
    for(int i=0;i<G.vexnum*G.vexnum;i++){
        matx[i]=0;
    }
    while(fin>>m1>>m2){
        get_matrix(G,m1,m2,matx);
    }

    //counting
    int *mark=new int[G.vexnum];
    int v=G.vexnum;

    int *mark_d;
    int *vexnum;
    int *d_matx;

    cudaMalloc((void**)&mark,4*G.vexnum);
    cudaMalloc((void**)&vexnum,4);
    cudaMalloc((void**)&d_matx,4*G.vexnum*G.vexnum);

    cudaMemcpy(mark_d,mark,4*G.vexnum,cudaMemcpyHostToDevice);
    cudaMemcpy(vexnum,&v,4,cudaMemcpyHostToDevice);
    cudaMemcpy(d_matx,matx,4*G.vexnum*G.vexnum,cudaMemcpyHostToDevice);

    count<<<3,1024>>>(mark_d,matx,vexnum);

    cudaMemcpy(mark,mark_d,4*G.vexnum,cudaMemcpyDeviceToHost);

    int sum=0;
    for(int i=0;i<G.vexnum;i++){
        sum+=mark[i];
    }

    cout<<sum<<endl;

    return 0;
}
