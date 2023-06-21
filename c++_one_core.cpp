#include <iostream>
#include <chrono>
using namespace std;
using namespace chrono;

#define N 3000

int m1[N][N]={0};
int m2[N][N]={0};
int m3[N][N]={0};

int main(){
    auto start=system_clock::now();
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            for(int p=0;p<N;p++){
                m3[i][j]=m3[i][j]+m1[i][p]*m2[p][j];
            }
        }
    }
    auto end=system_clock::now();
    auto duration=duration_cast<microseconds>(end-start);
    cout<<"Spend of one core"<<double(duration.count())*microseconds::period::num/microseconds::period::den<<endl;
    return 0;
}