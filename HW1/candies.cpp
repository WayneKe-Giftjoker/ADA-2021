#include <iostream>
#include <vector>
#define POS 0
#define NEG 1

using namespace std;
//thanks to b06303131 沈家睿 & b09902070 蔡其翰 for helping!

vector <long long> num({});
long long dp[1<<1][1<<17];
int N, flag;
int sol[1<<17];
int pend, nend;

void init(){
    scanf("%d", &N);
    long long d;
    num.clear();
    for(int i = 0; i < N; i++){
        dp[POS][i] = -1000000001;
        dp[NEG][i] = -1000000001;
        scanf("%lld", &d);
        num.push_back(d);
        sol[i] = -1;
    }
    pend = nend = -1;
    if(num[0] > 0){
        dp[POS][0] = num[0];
        sol[0] = 0;
        pend = 0;
    }
    else if(num[0] < 0){
        dp[NEG][0] = num[0];
        sol[0] = 0;
        nend = 0;
    }
    else{
        dp[POS][0] = dp[NEG][0] = num[0];
        sol[0] = 0;
        pend = nend = 0;
    }
}

void output(){
    long long M;
    int idx;
    // if(pend == -1){
    //     max = dp[NEG][nend];
    //     idx = nend;
    // }
    // else if(nend == -1){
    //     max = dp[POS][pend];
    //     idx = pend;
    // }
    // else{
    //     max = (dp[POS][pend] > dp[NEG][nend])? dp[POS][pend] : dp[NEG][nend];
    //     if(max == dp[POS][pend])
    //         idx = pend;
    //     else
    //         idx = nend;
    // }
    M = max(dp[POS][N-1], dp[NEG][N-1]);
    if(M == dp[POS][N-1])
        idx = pend;
    else
        idx = nend;
    printf("%lld\n", M);
    if(flag){
        int prev = -1;
        int remain = 0;
        int output[1<<17];
        do{
            output[remain++] = idx;
            prev = idx;
            idx = sol[idx];
        } while(sol[idx] != prev);
        printf("%d ", remain);
        for(int i = 0; i < remain; i++){
            if(i != remain - 1)
                printf("%d ", output[i]+1);
            else
                printf("%d\n", output[i]+1);
        }
    }
    // printf("===============\n");
    // for(int i = 0; i < N; i++)
    //     printf("%d %d\n", dp[POS][i], dp[NEG][i]);
}

void find(){
    for(int i = 1; i < N; i++){
        if(num[i] > 0){
            dp[POS][i] = max(max(dp[NEG][i-1] + num[i], num[i]), dp[POS][i-1]);
            dp[NEG][i] = dp[NEG][i-1];
            if(dp[POS][i] == dp[NEG][i-1] + num[i]){
                sol[i] = nend;
                pend = i;
            }
            else if(dp[POS][i] == num[i]){
                sol[i] = i;
                pend = i;
            }
        }
        else if(num[i] < 0){
            dp[NEG][i] = max(max(dp[POS][i-1] + num[i], num[i]), dp[NEG][i-1]);
            dp[POS][i] = dp[POS][i-1];
            if(dp[NEG][i] == dp[POS][i-1] + num[i]){
                sol[i] = pend;
                nend = i;
            }
            else if(dp[NEG][i] == num[i]){
                sol[i] = i;
                nend = i;
            }
        }
        else{
            dp[POS][i] = dp[NEG][i] = max(max(dp[POS][i-1], dp[NEG][i-1]), num[i]);
            if(dp[POS][i] == dp[POS][i-1])
                sol[i] = pend;
            else if(dp[NEG][i] == dp[NEG][i-1])
                sol[i] = nend;
            else
                sol[i] = i;
            pend = nend = i;
        }
        // printf("-----i: %d || pend: %d || nend: %d------\n", i, pend, nend);
    }
}

int main(int argc, char *argv[]){
    int T;
    scanf("%d %d", &T, &flag);
    while(T--){
        init();
        find();
        output();
    }
    
    return 0;
}
