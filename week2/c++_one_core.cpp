#include <iostream>
#include <time.h>
#include <cstdlib>

using namespace std;

int main(int argc, char *argv[]){
    clock_t s,f;
    double d;
    s=clock();

    int N=atoi(argv[1]);

    int *m1=new int[N*N];
    int *m2=new int[N*N];
    int *m3=new int[N*N];

    srand((unsigned)time(NULL));

    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            m1[i*N+j]=rand();
        }
    }
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            m2[i*N+j]=rand();
        }
    }


    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            for(int p=0;p<N;p++){
                m3[i*N+j]=m3[i*N+j]+m1[i*N+p]*m2[p*N+j];
            }
        }
    }

    delete []m1;
    delete []m2;
    delete []m3;

    f=clock();
    d=(double)(f-s)/CLOCKS_PER_SEC;
    cout<<"Spend of one core"<<d<<endl;

    return 0;
}