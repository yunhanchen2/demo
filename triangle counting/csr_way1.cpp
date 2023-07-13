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

void init_arr(AMGraph &G,int *index_ptr,int *indices,int *ind_row_num){
    for(int i=0;i<=G.vexnum;i++){
        index_ptr[i]=0;
    }
    for(int i=0;i<G.edgnum;i++){
        indices[i]=0;
    }
    for(int i=0;i<G.vexnum;i++){
        ind_row_num[i]=0;
    }
}

void get_matrix(AMGraph &G,int v1,int v2,int * matx){
    int i=v1-1;
    int j=v2-1;
    matx[i*G.vexnum+j]=1;
    matx[j*G.vexnum+i]=1;
}

void change(AMGraph &G,int *matx,int *ind_row_num,int *indices){
    int k=0;
    for(int i=0;i<G.vexnum;i++){
        for(int j=0;j<G.vexnum;j++){
            if(matx[i*G.vexnum+j]==1){
                ind_row_num[i]++;
                indices[k]=j;
                k++;
            }
        }
    }
}

int Get_array(AMGraph &G,int *arr_tmp,int n,int *index_ptr,int *indices){
    int ind=0;
    //copy the old numbers
    for(int i=index_ptr[n];i<index_ptr[n+1];i++){
        arr_tmp[ind]=indices[i];
        ind++;
    }
    return ind;
}

void find_the_same(int *arri,int *arrj,int arr_numi,int arr_numj,int &count,int i2){
    for(int i=0;i<arr_numi;i++){
        for(int j=0;j<arr_numj;j++){
            if(arri[i]==arrj[j]&&arri[i]>i2){
                count++;
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

    int *index_ptr=new int[G.vexnum+1];
    int *indices=new int[G.edgnum];
    int *ind_row_num=new int[G.vexnum];

    //init the array
    init_arr(G,index_ptr,indices,ind_row_num);

    //get the matrix first
    int *matx=new int[G.vexnum*G.vexnum];
    for(int i=0;i<G.vexnum*G.vexnum;i++){
        matx[i]=0;
    }
    while(fin>>m1>>m2){
        get_matrix(G,m1,m2,matx);
    }

    //change the matrix to csr
    change(G,matx,ind_row_num,indices);
    
    delete [] matx;

    //create index_ptr
    int num=0;
    for(int i=0;i<G.vexnum;i++){
        index_ptr[i]=num;
        num+=ind_row_num[i];
    }
    index_ptr[G.vexnum]=G.edgnum;

    clock_t s,fi;
    double d;
    s=clock();

    //find the neighbor index
    int count=0;
    for(int i=0;i<G.vexnum;i++){
        int *arri=new int[G.vexnum];
        int arri_num=Get_array(G,arri,i,index_ptr,indices);

        for(int j=0;j<arri_num;j++){
            if(i<arri[j]){
                int *arrj=new int[G.vexnum];
                int arrj_num=Get_array(G,arrj,arri[j],index_ptr,indices);
                //find how many the same
                find_the_same(arri,arrj,arri_num,arrj_num,count,arri[j]);
            }
        }
    }

    delete [] index_ptr;
    delete [] indices;
    delete [] ind_row_num;

    cout<<count<<endl;

    fi=clock();

    d=(double)(fi-s)/CLOCKS_PER_SEC;
    cout<<"Spend of csr"<<d<<endl;

    return 0;
}
