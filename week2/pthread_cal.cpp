#include <iostream>
#include <pthread.h>
#include <cstdlib>
#include <time.h>

using namespace std;

struct num{
    int a;
    int b;
    int c;
    int *m1;
    int *m2;
    int *m3;
};

void* cal(void* k){
    num* p=(struct num*)k;
    int n=p->b;
    int l=p->c;
    const int m=p->a;
    for(int w=0;w<l;w++) {
        int h = m * l + w;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                p->m3[h * n + i] = p->m3[h * n + i] + p->m1[h * n + j] * p->m2[j * n + i];
            }
        }
    }
    pthread_exit(NULL);
}

int main(int argc,char * argv[])
{
    clock_t s,fi;
    double d;
    s=clock();

    const int N=atoi(argv[1]);

    srand((unsigned)time(NULL));

    pthread_t thread_ids[8];

    int f=N/7;
    int r=N%7;
    num arr[7];
    for(int i=0;i<7;i++){
        arr[i].c=f;
        arr[i].m1=new int[N*N];
        arr[i].m2=new int[N*N];
        arr[i].m3=new int[N*N];
        for(int ii=0;ii<N;ii++){
            for(int j=0;j<N;j++){
                arr[i].m1[ii*N+j]=rand();
            }
        }
        for(int ii=0;ii<N;ii++){
            for(int j=0;j<N;j++){
                arr[i].m2[ii*N+j]=rand();
            }
        }
        arr[i].b=N;
        arr[i].a=i;
        int ret = pthread_create(&thread_ids[i], NULL, cal,(void*)&arr[i]);
        if (ret!= 0){ cout << "pthread CPU  create error code: " << ret<< endl;}
        int ret2=pthread_join(thread_ids[i],NULL);
        if (ret2!= 0){ cout << "pthread CPU join error code: " << ret2<< endl;}
        delete arr[i].m1;
        delete arr[i].m2;
        delete arr[i].m3;
        }
    num ar;
    ar.m1=new int[N*N];
    ar.m2=new int[N*N];
    ar.m3=new int[N*N];
    for(int ii=0;ii<N;ii++){
        for(int j=0;j<N;j++){
            ar.m1[ii*N+j]=rand();
        }
    }
    for(int ii=0;ii<N;ii++){
        for(int j=0;j<N;j++){
            ar.m2[ii*N+j]=rand();
        }
    }
    ar.b=N;
    ar.a=7;
    ar.c=r;
    int ret = pthread_create(&thread_ids[7], NULL, cal,(void*)&ar);
    if (ret!= 0){ cout << "pthread CPU  create error code: " << ret << endl;}
    int ret2=pthread_join(thread_ids[7],NULL);
    if (ret2!= 0){ cout << "pthread CPU join error code: " << ret2 << endl;}
    delete ar.m1;
    delete ar.m2;
    delete ar.m3;
    fi=clock();
    d=(double)(fi-s)/CLOCKS_PER_SEC;
    cout<<"Spend of pthread"<<d<<endl;
    return 0;
}
