#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
#define ll long long
#define MAXN 1<<10
//Reference: https://github.com/PetarV-/Algorithms/blob/master/Graph%20Algorithms/Kosaraju's%20Algorithm.cpp

using namespace std;

typedef struct comp{
    vector<ll> SCC; //cities that are strongly connected
    vector<ll> WCC; //adjList for SCCs
    ll in, out;
}Comp;

ll T, flag, N, M;
vector<vector<ll>> adjL(MAXN, vector<ll>({})); //adjList
vector<vector<ll>> adjLT(MAXN, vector<ll>({})); //Transpose adjList
vector<int> visited(MAXN, 0);
vector<Comp> SCCGraph(MAXN);
vector<ll> idxToSCCidx(MAXN, -1);
ll numSCC;
vector<ll> source({}); vector<ll> sink({});
vector<pair<ll, ll>> Chain({}); vector<ll> srcLeftover({}); vector<ll> snkLeftover({}); vector<ll> Iso({});
// vector<pair<ll, ll>> ans({});
stack<ll> S;

void kosaraju_dfs_1(ll x){
    visited[x] = 1;
    ll num = adjL[x].size();
    for (int i = 0;i < num; i++){
        if (!visited[adjL[x][i]]) kosaraju_dfs_1(adjL[x][i]);
    }
    S.push(x);
}

void kosaraju_dfs_2(ll x){
    idxToSCCidx[x] = numSCC;
    SCCGraph[numSCC].SCC.push_back(x);
    visited[x] = 1;
    ll num = adjLT[x].size();
    for (int i = 0; i < num; i++){
        if (!visited[adjLT[x][i]]) kosaraju_dfs_2(adjLT[x][i]);
    }
}

void Kosaraju(){
    for (int i = 1; i <= N; i++){
        if (!visited[i]) kosaraju_dfs_1(i);
    }
    
    for (int i = 1; i <= N; i++)
        visited[i] = 0;
    
    while (!S.empty()){
        int v = S.top(); S.pop();
        if (!visited[v]){
            kosaraju_dfs_2(v);
            numSCC++;
        }
    }
}

void connectSCCs(){
    ll len;
    for(int i = 1; i <= N; i++){
        len = adjL[i].size();
        for(int j = 0; j < len; j++){
            if(idxToSCCidx[i] != idxToSCCidx[adjL[i][j]]){
                SCCGraph[idxToSCCidx[i]].WCC.push_back(idxToSCCidx[adjL[i][j]]);
                SCCGraph[idxToSCCidx[i]].out++;
                SCCGraph[idxToSCCidx[adjL[i][j]]].in++;
            }
        }
    }
}

ll getChain(ll srcidx){
    visited[srcidx] = 1;
    if(SCCGraph[srcidx].out == 0)
        return srcidx;
    ll len = SCCGraph[srcidx].WCC.size(); ll tmp;
    for(int i = 0; i < len; i++){
        if(!visited[SCCGraph[srcidx].WCC[i]]){
            tmp = getChain(SCCGraph[srcidx].WCC[i]);
            if(SCCGraph[tmp].out == 0)
                return tmp;
        }
    }
    return -1;
}

void findGroup(ll srcnum, ll snknum){
    visited.assign(numSCC, 0);
    ll tmp;
    for(int i = 0; i < srcnum; i++){
        tmp = getChain(source[i]);
        if(tmp == -1)
            srcLeftover.push_back(source[i]);
        else if(source[i] != tmp && SCCGraph[tmp].out == 0)
            Chain.push_back({source[i], tmp});
        else if(SCCGraph[source[i]].in == 0 && SCCGraph[source[i]].out == 0)
            Iso.push_back(source[i]);
    }
    for(int i = 0; i < snknum; i++){
        if(!visited[sink[i]])
            snkLeftover.push_back(sink[i]);
    }
}

void findAns(){
    ll chainNum = Chain.size(), srcleftoverNum = srcLeftover.size(), snkleftoverNum = snkLeftover.size(), isoNum = Iso.size();
    //type I
    for(int i = 0; i < chainNum - 1; i++){
        printf("%lld %lld\n", SCCGraph[Chain[i].second].SCC[0], SCCGraph[Chain[i+1].first].SCC[0]);
    }
    //type II
    ll tmp, dummy;
    if(chainNum > 0){
        tmp = Chain[chainNum-1].second;
        for(int i = 0; i < isoNum; i++){
            printf("%lld %lld\n", SCCGraph[tmp].SCC[0], SCCGraph[Iso[i]].SCC[0]);
            tmp = Iso[i];
        }
        printf("%lld %lld\n", SCCGraph[tmp].SCC[0], SCCGraph[Chain[0].first].SCC[0]);
        dummy = Chain[0].first;
    }
    else{
        for(int i = 0; i < isoNum-1; i++){
            printf("%lld %lld\n", SCCGraph[Iso[i]].SCC[0], SCCGraph[Iso[i+1]].SCC[0]);
        }
        printf("%lld %lld\n", SCCGraph[Iso[isoNum-1]].SCC[0], SCCGraph[Iso[0]].SCC[0]);
        return;
    }
    
    //type III
    ll srcidx = 0; ll snkidx = 0;
    while(srcidx < srcleftoverNum || snkidx < snkleftoverNum){
        if(srcidx < srcleftoverNum && snkidx < snkleftoverNum){
            printf("%lld %lld\n", SCCGraph[snkLeftover[snkidx]].SCC[0], SCCGraph[srcLeftover[srcidx]].SCC[0]);
            snkidx++; srcidx++;
        }
        else if(snkidx < snkleftoverNum){
            printf("%lld %lld\n", SCCGraph[snkLeftover[snkidx]].SCC[0], SCCGraph[dummy].SCC[0]);
            snkidx++;
        }
        else if(srcidx < srcleftoverNum){
            printf("%lld %lld\n", SCCGraph[dummy].SCC[0], SCCGraph[srcLeftover[srcidx]].SCC[0]);
            srcidx++;
        }
    }
    
}

void reset(){
    for(int i = 0; i < numSCC; i++){
        SCCGraph[i].in = SCCGraph[i].out = 0;
        SCCGraph[i].SCC.clear();
        SCCGraph[i].WCC.clear();
    }
    for(int i = 0; i <= N; i++){
        adjL[i].clear();
        adjLT[i].clear();
    }
    visited.assign(MAXN, 0); idxToSCCidx.assign(MAXN, -1);
    numSCC = 0; source.clear(); sink.clear(); Chain.clear(); Iso.clear(); srcLeftover.clear(); snkLeftover.clear();
}

void printInfo(){
    for(int i = 0; i < Chain.size(); i++)
        printf("===Chain[%d] = {%lld, %lld}\n", i, Chain[i].first, Chain[i].second);
    for(int i = 0; i < Iso.size(); i++)
        printf("===Iso[%d] = %lld\n", i, Iso[i]);
    for(int i = 0; i < srcLeftover.size(); i++)
        printf("===srcLeftOver[%d] = %lld\n", i, srcLeftover[i]);
    for(int i = 0; i < snkLeftover.size(); i++)
        printf("===snkLeftOver[%d] = %lld\n", i, snkLeftover[i]);
}

int main(int argc, char *argv[]){
    scanf("%lld %lld", &T, &flag);
    while(T--){
        scanf("%lld %lld", &N, &M);
        ll u, v;
        for(int i = 0; i < M; i++){
            scanf("%lld %lld", &u, &v);
            adjL[u].push_back(v);
            adjLT[v].push_back(u);
        }
        Kosaraju();
        if(numSCC == 1){
            printf("0\n");
            reset();
            continue;
        }
        connectSCCs();
        ll numsrc = 0, numsnk = 0;
        for(int i = 0; i < numSCC; i++){
            if(SCCGraph[i].in == 0){
                source.push_back(i);
                numsrc++;
            }
            if(SCCGraph[i].out == 0){
                sink.push_back(i);
                numsnk++;
            }
        }
        printf("%lld\n", (numsrc > numsnk)? numsrc : numsnk);
        if(flag){
            findGroup(numsrc, numsnk);
            findAns();
        }
        reset();
    }
    
    return 0;
}
