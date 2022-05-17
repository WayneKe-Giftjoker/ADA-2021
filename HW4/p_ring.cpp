#include <iostream>
#include <algorithm>
#include <vector>
#include <cstdlib>
#include <climits>
#include <numeric>
#define ll long long
#define POKENUM 5
#define INF ((ll)1)<<61//LLONG_MAX
//感謝b06303131 沈家睿 & b08902149 徐晨祐的協助!!!

using namespace std;

ll N, M;
vector<ll> symbols;

ll risk_bit(ll end, vector<ll> &symbCpy){
    if(end <= 1)
        return 0;
    ll ret = symbCpy[end-1] * ((symbCpy[end-2] & symbCpy[end-1]) ^ (symbCpy[end-1] | symbCpy[end]) ^ (symbCpy[end-2] + symbCpy[end]));
    return ret;
}

ll gap(ll end, vector<ll> &symbCpy){
    return symbCpy[end] - llabs(symbCpy[end] - symbCpy[end-1]) + risk_bit(end, symbCpy);
}

void rotate(vector<ll> &symbCpy, ll offset){
    for(int i = offset; i < N; i++){
        symbCpy[i-offset] = symbols[i];
    }
    for(int i = 0; i < offset; i++){
        symbCpy[i+N-offset] = symbols[i];
    }
}

int main(int argc, char *argv[]){
    srand(20008912199121980002);//20008912199121980002
    scanf("%lld %lld", &N, &M);
    for(int i = 0; i < N; i++){
        ll tmp;
        scanf("%lld", &tmp);
        symbols.push_back(tmp);
    }
    vector<ll> symbCpy(N);ll k = 4*N/M; ll offset;//POKENUM
    ll ans = INF;
    if(N == M){
        ans = accumulate(symbols.begin(), symbols.end(), (ll)0);
        printf("%lld\n", ans);
        return 0;
    }
    while(k--){
        offset = rand() % N;
        rotate(symbCpy, offset);
        ll dp[N][M];
        dp[0][0] = symbCpy[0];
        for(int i = 1; i < M; i++) dp[0][i] = INF;
        for(int i = 1; i < N; i++){
            for(int j = 0; j < M; j++){
                if(!j) dp[i][j] = dp[i-1][j] + gap(i, symbCpy);
                else if(i < j) dp[i][j] = INF;
                else{
                    dp[i][j] = min(dp[i-1][j] + gap(i, symbCpy), dp[i-1][j-1] + symbCpy[i]);
                    if(i > 1) dp[i][j] = min(dp[i][j], dp[i-2][j-1] + symbCpy[i] + symbCpy[i-1] - llabs(symbCpy[i]-symbCpy[i-1]));
                }
            }
        }
        ans = min(ans, dp[N-1][M-1]);
    }
    printf("%lld\n", ans);
    
    return 0;
}
