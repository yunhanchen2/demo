#include <iostream>
#include <fstream>
using namespace std;

#define MVnum 30000

struct AMGraph{
    int index_ptr[MVnum];
    int indices[MVnum];
    int ind_row_num[MVnum];
    int vexnum;
    int edgnum;
};

//initialize array
void init(AMGraph &G,int n1,int n2){
    G.vexnum=n1;
    G.edgnum=n2;
    for(int i=0;i<MVnum;i++){
        G.ind_row_num[i]=0;
    }
    for(int i=0;i<MVnum*2;i++){
        G.indices[i]=0;
    }
}

//csr store
void get_graph(AMGraph &G,int v2,int v1,int i){
    G.ind_row_num[v1-1]++;
    G.indices[i]=v2-1;
}

int Get_array(AMGraph &G,int arr_tmp[MVnum],int n){
    int ind=0;

    //copy the old numbers
    for(int i=G.index_ptr[n];i<G.index_ptr[n+1];i++){
        arr_tmp[ind]=G.indices[i];
        ind++;
    }
    //find new numbers
    for(int i=0;i<G.edgnum;i++){
        if(G.indices[i]==n){
            for(int j=0;j<G.vexnum;j++){
                if(i>=G.index_ptr[j]&&i<G.index_ptr[j+1]){
                    arr_tmp[ind]=j;
                    ind++;
                }
            }
        }
    }

    //delete the same numbers in array
    int arr_num=ind;
    int ptr=0;
    for(int i=0;i<ind;i++){
        for(int j=i+1;j<ind;j++){
            if(arr_tmp[i]==arr_tmp[j]){
                for(int k=j+1;k<ind;k++){
                    arr_tmp[k-1]=arr_tmp[k];
                }
                ind--;
                j--;
            }
        }
    }
    return ind;
}

void find_the_same(int arri[MVnum],int arrj[MVnum],int arr_numi,int arr_numj,int &count,int i2){
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
    fin.open("graph1.txt");
    int m1,m2,n1,n2;
    int poz=0;
    fin>>n1>>n2;
    init(G,n1,n2);
    while(fin>>m1>>m2){
        if(m1==m2){
            G.edgnum--;
        }else{
            get_graph(G,m1,m2,poz);
            poz+=1;
        }
    }

    //create index_ptr
    int num=0;
    for(int i=0;i<G.vexnum;i++){
        G.index_ptr[i]=num;
        num+=G.ind_row_num[i];
    }
    G.index_ptr[G.vexnum]=G.edgnum;
    int count=0;
    //find the neighbor index
    for(int i=0;i<G.vexnum;i++){
        int arri[MVnum];
        int arri_num=Get_array(G,arri,i);

        for(int j=0;j<arri_num;j++){
            if(i<arri[j]){
                int arrj[MVnum];
                int arrj_num=Get_array(G,arrj,arri[j]);
                //find how many the same
                find_the_same(arri,arrj,arri_num,arrj_num,count,arri[j]);
            }
        }
    }
    cout<<count<<endl;
    return 0;
}
