#include <iostream>
#include <pthread.h>
#include <chrono>
using namespace std;
using namespace chrono;

#define N 100

int m1[N][N]={0};
int m2[N][N]={0};
int m3[N][N]={0};


struct num{
    int a;
    int b;
};

void* cal(void* k){
    num* pp;
    pp=(struct num*)k;
    for(int p=0;p<N;p++){
        m3[pp->a][pp->b]=m3[pp->a][pp->b]+m1[pp->a][p]*m2[p][pp->b];
    }
    delete pp;
    pthread_exit(NULL);
}

int main()
{
    auto start=system_clock::now();
    pthread_t thread_ids[N];
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            num *arr=new num;
            arr->a=i;
            arr->b=j;
            int ret = pthread_create(&thread_ids[i], NULL, cal, arr);
            if (ret!= 0){ cout << "pthread CPU  create error code: " << ret<< endl;}
        }
    }
    for(int i=0;i<N;i++){
        int m=pthread_join(thread_ids[i],NULL);
        if (m != 0){ cout << "pthread CPU join error code: " << m<< endl;}
    }

    auto end=system_clock::now();
    auto duration=duration_cast<microseconds>(end-start);
    cout<<"Spend of pthread"<<double(duration.count())*microseconds::period::num/microseconds::period::den<<endl;
    return 0;
}


