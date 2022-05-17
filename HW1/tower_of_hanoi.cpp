#include <iostream>
#include <cstdlib>
#define MOD 998244353
#define INIT 0
#define MID 1
#define DES 2
//感謝 林胤辰 & 沈家睿 討論!!

using namespace std;

int checkList[1<<17];
int where[1<<17];
int n;

void buildCheck(int n){
    checkList[0] = 0;
    checkList[1] = 1;
    for(int i = 2; i <= n; i++){
        checkList[i] = (2*checkList[i-1] + 1) % MOD;
    }
}

void getData(){
    int num, disc;
    for(int i = 0; i < 3; i++){
        scanf("%d", &num);
        for(int j = 0; j < num; j++){
            scanf("%d", &disc);
            where[disc] = i;
        }
    }
}

int ToH(int tar, int from, int aux, int des, int ans){
    if(tar == 0){
        return ans;
    }
    if(where[tar] == from){ //maybe use an array to keep what disc is on what peg?
        return ToH(tar-1, from, des, aux, (ans + checkList[tar-1] + 1) % MOD);
    }
    else if(where[tar] == des){
        return ToH(tar-1, aux, from, des, ans);
    }
    else{
        return ToH(0, from, aux, des, -1);
    }
}

int main(int argc, char *argv[]){
    scanf("%d", &n);
    buildCheck(n);
    getData();
    printf("%d\n", ToH(n, INIT, MID, DES, 0));
    
    return 0;
}
