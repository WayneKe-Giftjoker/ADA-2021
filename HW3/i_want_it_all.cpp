#include <iostream>
#include <vector>
#include <tuple>
#include <queue>
#include <climits>
#include <algorithm>
#define ll long long
#define MAXN 1<<17
#define INF LLONG_MAX

//感謝 b06303131 沈家睿 的協助!!
//Reference: https://www.geeksforgeeks.org/prims-algorithm-using-priority_queue-stl/

using namespace std;

typedef pair<ll, ll> iPair;

ll N, M;

ll primMST(ll num, vector<vector<tuple<ll, ll, ll>>> &adjL){
    priority_queue<iPair, vector<iPair>, greater<iPair>> pq;
    ll src = 1;
    vector<ll> key(num+1, INF); // keys as infinite (INF)
    vector<bool> inMST(num+1, false); // To keep track of vertices included in MST
 
    pq.push(make_pair(0, src));
    key[src] = 0;
 
    ll cost = 0; ll u, v, weight, id; iPair min;
    while (!pq.empty()){
        // The first vertex in pair is the minimum key
        // vertex, extract it from priority queue.
        // vertex label is stored in second of pair (it
        // has to be done this way to keep the vertices
        // sorted key (key must be first item
        // in pair)
        min = pq.top();
        u = min.second;
        pq.pop();

        if(inMST[u] == true){
            continue;
        }
       
        inMST[u] = true;
        cost += min.first;
        for(auto x : adjL[u]){
            tie(v, weight, id) = x;

            // If v is not in MST and weight of (u,v) is smaller
            // than current key of v
            if(inMST[v] == false && key[v] > weight){
                // Updating key of v
                key[v] = weight;
                pq.push(make_pair(key[v], v));
            }
        }
    }
    return cost;
}

void primSPT(ll num, vector<vector<tuple<ll, ll, ll>>> &adjL, vector<iPair> &sptAns){
    priority_queue<iPair, vector<iPair>, greater<iPair>> pq;
    ll src = 1;
    vector<ll> delta(num+1, INF); //source到某點時，最好的cost是多少
 
    pq.push(make_pair(0, src));
    delta[src] = 0;
 
    ll cost = 0; ll u, v, weight, id; iPair min;
    while (!pq.empty()){
        min = pq.top();
        u = min.second;
        pq.pop();
        for(auto x : adjL[u]){
            tie(v, weight, id) = x;
            if(delta[v] > delta[u] + weight){
                delta[v] = delta[u] + weight;
                pq.push(make_pair(delta[v], v));
                sptAns[v] = {delta[v], id};
            }
            else if(delta[v] == delta[u] + weight && sptAns[v].first > weight){
                sptAns[v] = {weight, id};
            }
        }
    }
}

int main(int argc, char *argv[]){
    scanf("%lld %lld", &N, &M);
    ll a, b, c;
    vector<vector<tuple<ll, ll, ll>>> adjL(N+1, vector<tuple<ll, ll, ll>>({}));
    vector<ll> edgeCost(M+1);
    vector<iPair> sptAns(N+1); //pair(走到此處的cost, edge_id)
    for(int i = 1; i <= M; i++){
        scanf("%lld %lld %lld", &a, &b, &c);
        adjL[a].push_back({b, c, i});
        adjL[b].push_back({a, c, i});
        edgeCost[i] = c;
    }
    ll MSTCost = primMST(N, adjL);
    primSPT(N, adjL, sptAns);
    ll SPTCost = 0;
    for(int i = 2; i <= N; i++)
        SPTCost += edgeCost[sptAns[i].second];
    if(SPTCost == MSTCost){
        printf("Yes\n");
        for(int i = 2; i <= N; i++)
            printf("%lld ", sptAns[i].second);
    }
    else
        printf("No\n");
    
    return 0;
}
