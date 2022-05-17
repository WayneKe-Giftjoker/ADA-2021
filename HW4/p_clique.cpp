#include <iostream>
#include <vector>
#include <bitset>
#include "ypglpk.hpp" //TA provided hpp
#define MAXN 1<<7
#define MAXC 1<<10

//感謝b08902149 徐晨祐 & b06303131 沈家睿的協助!!!

using namespace std;

vector<bitset<MAXN>> clique;
vector<vector<int>> G(MAXN, vector<int>({})); vector<bitset<MAXN>> neighbors(MAXN, 0);
bitset<MAXN> vertexRecord;
bitset<MAXN> one = 1;

void BronKerbosch(bitset<MAXN> P, bitset<MAXN> R, bitset<MAXN> X){
    bitset<MAXN> PUX = P|X;
    if(PUX.none()){
        clique.push_back(R);
        return;
    }
    int maxDeg = 0, pivot = 0;
    while(PUX.any()){
        int u = PUX._Find_first();
        if(maxDeg < G[u].size()){
            maxDeg = G[u].size();
            pivot = u;
        }
        PUX[u] = 0;
    }
    bitset<MAXN> P_ = P^(P&neighbors[pivot]);
    while(P_.any()){
        int v = P_._Find_first();
        BronKerbosch(P&neighbors[v], R|(one<<v), X&neighbors[v]);
        P_[v] = 0;
        P[v]  = 0;
        X[v]  = 1;
    }
}

void reset(int N, bitset<MAXN> &P, bitset<MAXN> &R, bitset<MAXN> &X){
    for(int i = 0; i < N; ++i){
        G[i].clear();
        neighbors[i].reset();
    }
    clique.clear();
    vertexRecord.reset();
    P.reset(); R.reset(); X.reset();
}

int main(int argc, char *argv[]){
    int T, N, M;
    scanf("%d", &T);
    int u, v;
    bitset<MAXN> P, R, X;
    while(T--){
        scanf("%d %d", &N, &M);
        if(!M){
            printf("%d\n", N);
            for(int i = 0; i < N; ++i)
                printf("1 %d\n", i);
            continue;
        }
        for(int i = 0 ; i < M; ++i){
            scanf("%d %d", &u, &v);
            G[u].push_back(v); G[v].push_back(u);
            neighbors[u][v] = 1; neighbors[v][u] = 1;
        }
        
        for(int i = 0; i < N; ++i)
            P[i] = 1;
        BronKerbosch(P, R, X);
        
        int cliqnum = clique.size();
        vector<vector<double>> A((cliqnum<<1)+N, vector<double>(cliqnum, 0));
        vector<double> b((cliqnum<<1)+N, 0), c(cliqnum, -1);
        vector<bool> isint(cliqnum, true);
        for(int i = 0; i < cliqnum; ++i){
            A[i][i] = 1;
            A[i+cliqnum][i] = -1;
            b[i] = 1;
        }
        for(int i = 0; i < N; ++i){
            for(int j = 0; j < cliqnum; ++j){
                A[i+(cliqnum<<1)][j] = -clique[j][i];
            }
            b[i+(cliqnum<<1)] = -1;
        }
        pair<double, vector<double>> x = ypglpk::mixed_integer_linear_programming(A, b, c, isint);
        
        printf("%d\n", (int)(-x.first));
        for(int i = 0; i < cliqnum; ++i){
            if((int)x.second[i]){
                clique[i] = clique[i]^(clique[i]&vertexRecord);
                printf("%d ", clique[i].count());
                while(clique[i].any()){
                    v = clique[i]._Find_first();
                    if(!vertexRecord[v]){
                        printf("%d ", v);
                        vertexRecord[v] = 1;
                    }
                    clique[i][v] = 0;
                }
                printf("\n");
            }
        }
        reset(N, P, R, X);
    }
    
    return 0;
}
