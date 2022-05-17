#include <iostream>
#include <algorithm>
//感謝 b06303131 沈家睿 & b10902021 張佑亘 的幫忙!!

using namespace std;

int N;
long long num[1<<9];
long long GCD[1<<9][1<<9];
long long dp[1<<9][1<<9];

void getData(){
    scanf("%d", &N);
    for(int i = 0; i < N; i++)
        scanf("%lld", &num[i]);
}

long long gcd(long long a, long long b){
    long long temp;
    while(a % b){
        temp = a % b;
        a = b;
        b = temp;
    }
    return b;
}

void buildGCD(){
    for(int i = 0; i < N; i++){
        for(int j = i + 1; j < N; j++){
            if(i != j)
                GCD[i][j] = gcd(num[i], num[j]);
        }
    }
}

long long buildDP(){
    int i, j;
    for(int len = 2; len <= 3; len++){ //base case
        long long res1, res2, res3;
        for(i = 0, j = i + len - 1; j < N; i++, j++){
            if(len == 2){
                res1 = GCD[i][j];
                if(res1 > 1)
                    dp[i][j] = res1;
            }
            else{
                res1 = GCD[i][j-1]; res2 = GCD[i][j]; res3 = GCD[j-1][j];
                if(res1 > 1 && res2 > 1 && res3 > 1){
                    dp[i][j] = res1 + res3;
                }
            }
        }
    }

    long long case1, case2, case3;
    for(int len = 4; len <= N; len++){
        for(i = 0, j = i + len - 1; j < N; i++, j++){
            case1 = case2 = case3 = 0;
            if(GCD[i][j] > 1 && dp[i+1][j-1] > 1)
                case1 = dp[i+1][j-1] + GCD[i][j];
            
            case2 = case3 = -1;
            if(GCD[i][j] > 1){
                for(int k = i + 2; k <= j - 2; k++){
                    if(dp[i+1][k-1] > 1 && dp[k+1][j-1] > 1 && GCD[i][k] > 1 && GCD[k][j] > 1)
                        case2 = max(case2, dp[i+1][k-1] + dp[k+1][j-1] + GCD[i][k] + GCD[k][j]);
                }
                if(dp[i+2][j-1] > 1 && GCD[i][i+1] > 1 && GCD[i+1][j] > 1)
                    case2 = max(case2, dp[i+2][j-1] + GCD[i][i+1] + GCD[i+1][j]);
                if(dp[i+1][j-2] > 1 && GCD[i][j-1] > 1 && GCD[j-1][j] > 1)
                    case2 = max(case2, dp[i+1][j-2] + GCD[i][j-1] + GCD[j-1][j]);
            }    
            for(int k = i + 1; k <= j - 2; k++){
                if(dp[i][k] > 1 && dp[k+1][j] > 1)
                    case3 = max(case3, dp[i][k] + dp[k+1][j]);
            }
            
            dp[i][j] = max({case1, case2, case3});
        }
    }
    return dp[0][N-1];
}

int main(int argc, char *argv[]){
    getData();
    buildGCD();
    long long ans = buildDP();
    printf("%lld\n", (ans > 1)? ans : -1);
    
    return 0;
}
