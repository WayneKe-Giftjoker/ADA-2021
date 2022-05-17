#include <iostream>
#include <algorithm>
#include <vector>
#include <deque>
#include <string>
#include <climits>
#define NINF LLONG_MIN
#define ll long long

using namespace std;

//感謝 b06303131 沈家睿 & b08902149 徐晨祐 & b05504066 李旻翰 的幫忙!!

int N, M, K;

int main(int argc, char *argv[]){
    int T;
    
    scanf("%d", &T);
    
    while(T--){
        //input start
        string s, num;
        int ssize, m;
        scanf("%d %d %d", &N, &M, &K);
        ll map[N+1][M+1];
        deque<int> off;
        int k_ = K;
        while(K >= 3){
            off.push_front(K/3);
            K -= K/3;
        }
        while(K > 0){
            off.push_front(1);
            K--;
        }
        K = off.size();
        vector<vector<vector<ll>>> dp(K, vector<vector<ll>>(N+1, vector<ll>(M+1)));

        for(int i = 1; i <= N; i++){
            for(int j = 1; j <= M; j++){
                cin >> s;
                if(s[0] == 'X'){
                    map[i][j] = NINF;
                }
                else map[i][j] = stoll(s);
            }
        }
        //input end
        ll tmpMax = NINF;
        ll up, upf, left, leftf, on;
        for(int i = 1; i <= N; i++){
            for(int j = 1; j <= M; j++){
                if(i == 1 && j == 1)
                    continue;
                
                up = (i == 1)? NINF : dp[K-1][i-1][j];
                upf = (i-off[K-1] < 1)? NINF : dp[K-1][i-off[K-1]][j];
                left = (j == 1)? NINF : dp[K-1][i][j-1];
                leftf = (j-off[K-1] < 1)? NINF : dp[K-1][i][j-off[K-1]];
                tmpMax = max({up, upf, left, leftf});
                if(tmpMax != NINF && map[i][j] != NINF)
                    dp[0][i][j] = tmpMax + map[i][j];
                else
                    dp[0][i][j] = NINF;
                for(int k = 1; k < K; k++){
                    on = dp[k-1][i][j];
                    up = (i-off[k-1] < 1)? NINF : dp[k-1][i-off[k-1]][j];
                    left = (j-off[k-1] < 1)? NINF : dp[k-1][i][j-off[k-1]];
                    dp[k][i][j] = max({on, up, left});
                }
            }
        }
        ll ans = dp[0][N][M];
        deque<pair<int, int>> sol;
        if(ans != NINF){
            printf("Passable\n%lld\n", ans);
            int i = N; int j = M; int k = K - 1;
            while(i >= 1 && j >= 1){
                bool found = dp[k][i][j] == dp[0][i][j];
                if(found){
                    k = K - 1;
                    sol.push_front({i, j});
                }
                else
                    k--;
                up = (i == 1)? NINF : dp[k][i-1][j];
                upf = (i-off[k] < 1)? NINF : dp[k][i-off[k]][j];
                left = (j == 1)? NINF : dp[k][i][j-1];
                leftf = (j-off[k] < 1)? NINF : dp[k][i][j-off[k]];
                tmpMax = max({up, upf, left, leftf});
                if(tmpMax == up)
                    i--;
                else if(tmpMax == upf)
                    i -= off[k];
                else if(tmpMax == left)
                    j--;
                else
                    j -= off[k];
            }
            ll size = sol.size();
            printf("%lld\n", size);
            for(int a = 0; a < size; a++)
                printf("%d %d\n", sol[a].first, sol[a].second);
        }
        else
            printf("Impassable\n");
    }
    
    return 0;
}
