#include <iostream>
#include <fstream>
using namespace std;
#define MVnum 1100


struct AMGraph{
    int arcs[MVnum][MVnum];
    int vexnum;
    int edgnum;
};

void init(AMGraph &G){
    //init matrix
    for(int i=0;i<G.vexnum;i++){
        for(int j=0;j<G.vexnum;j++){
            G.arcs[i][j]=0;
        }
    }
}


void Create(AMGraph &G,int v1,int v2){
        int i=v1-1;
        int j=v2-1;
        G.arcs[i][j]=1;
        G.arcs[j][i]=1;
}

int main() {
    AMGraph G;
    init(G);
    long long count=0;
    int v1,v2,n1,n2;
    ifstream fin;
    fin.open("graph1.txt");
    fin>>n1>>n2;
    G.vexnum=n1;
    G.edgnum=n2;
    while(fin>>v1>>v2){
        Create(G,v1,v2);
    }

    for(int i=0;i<G.vexnum;i++){
        for(int j=i+1;j<G.vexnum;j++){
            if(G.arcs[i][j]==1){
                for(int k=j+1;k<G.vexnum;k++){
                    if(G.arcs[k][j]==1&&G.arcs[i][k]==1){
                        count++;
                    }
                }
            }
        }
    }

    cout<<"The number of triangle is "<<count<<endl;
    fin.close();
    return 0;
}
