#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#define ll long long
#define MAXN 1<<18

using namespace std;

//非常感謝 b06303131 沈家睿 && b05504066 李旻翰 && b08902149 徐晨祐 的幫忙!

ll N;
vector<vector<ll>> adjL(MAXN, vector<ll>({}));
vector<int> visited(MAXN, 0);
vector<ll> visRecord;
vector<ll> parentRecord(MAXN, -1);
vector<ll> diameter;
vector<ll> branch(MAXN, 0);
vector<ll> inversebranch(MAXN, 0);
queue<ll> q;
ll farthestCity, maxDist;

ll BFS(ll start, int flag){ //flag: 是否為第一次找直徑
    visited[start] = 1;
    q.push(start);
    ll len, idx, num;
    ll dist = 0;
    visRecord.push_back(start);
    while(!q.empty()){
        num = q.size();
        for(int i = 0; i < num; i++){
            idx = q.front(); q.pop();
            len = adjL[idx].size();
            for(int j = 0; j < len; j++){
                if(!visited[adjL[idx][j]]){
                    visRecord.push_back(adjL[idx][j]);//caution
                    visited[adjL[idx][j]] = 1;
                    q.push(adjL[idx][j]);
                }
            }
        }
        dist++;
    }
    
    if(flag){
        farthestCity = idx;
        len = visRecord.size();
        for(int i = 0; i < len; i++)
            visited[visRecord[i]] = 0;
        visRecord.clear(); dist = 0;
        visited[farthestCity] = 1;
        q.push(farthestCity);
        visRecord.push_back(farthestCity);
        while(!q.empty()){
            num = q.size();
            for(int i = 0; i < num; i++){
                idx = q.front(); q.pop();
                len = adjL[idx].size();
                for(int j = 0; j < len; j++){
                    if(!visited[adjL[idx][j]]){
                        visRecord.push_back(adjL[idx][j]);
                        visited[adjL[idx][j]] = 1;
                        q.push(adjL[idx][j]);
                        parentRecord[adjL[idx][j]] = idx;
                        // printf("===idx: %lld\n", idx);
                    }
                }
            }
            dist++;
        }
        if(dist >= maxDist){
            maxDist = dist;
            farthestCity = idx;
        }
        len = visRecord.size();
        for(int i = 0; i < len; i++)
            visited[visRecord[i]] = 0;
    }
    return dist;
}

int main(int argc, char *argv[]){
    scanf("%lld", &N);
    ll a, b;
    for(int i = 0; i < N-1; i++){
        scanf("%lld %lld", &a, &b);
        adjL[a].push_back(b);
        adjL[b].push_back(a);
    }
    maxDist = -1; farthestCity = -1;
    ll dist = BFS(1, 1), cur = farthestCity;
    // printf("==%lld\n", dist);
    for(int i = 0; i < dist; i++){
        // printf("cur: %lld\n", cur);
        if(cur != -1){
            diameter.push_back(cur);
            visited[cur] = 1;
            cur = parentRecord[cur];
        }
    }
    // for(int i = 0; i < dist; i++)
    //     printf("===diameter[%d] = %lld\n", i, diameter[i]);
    ll len, tmpdist; dist = 0;
    for(int i = 1; i < maxDist; i++){
        len = adjL[diameter[i]].size();
        branch[diameter[i]] = max(branch[diameter[i-1]], (ll)(i));
        if(len > 2)
            branch[diameter[i]] = max(branch[diameter[i]], BFS(diameter[i], 0)-1+i);
    }
    visited.assign(MAXN, 0);
    for(int i = 0; i < diameter.size(); i++)
        visited[diameter[i]] = 1;
	// for(int i = 0;i < MAXN;i++)
	// 	if(visited[i])
	// 		cout << "node id : " << i << '\n';
    dist = 0;
    for(int i = maxDist-2; i > 0; i--){
        len = adjL[diameter[i]].size();
        inversebranch[diameter[i]] = max(inversebranch[diameter[i+1]], (ll)(maxDist-1-i));
		ll branch_len = 0;
        if(len > 2){
			branch_len = BFS(diameter[i], 0);
			inversebranch[diameter[i]] = max(inversebranch[diameter[i]], branch_len-2+maxDist-i);
		}
        //printf("===now diameter = %lld || inversebranch[%d] = %lld || branch = %lld\n", diameter[i], i, inversebranch[diameter[i]], branch_len);
    }
    /* for(int i = 1; i <= maxDist-1; i++){
        printf("===Branch[%d] = %lld || InverseBranch[%d] = %lld\n", i, branch[i], i, inversebranch[diameter[i]]);
    } */
    ll ans = MAXN, tmpans;
    for(int i = 0; i < maxDist-1; i++){
        tmpans = max({branch[diameter[i]], inversebranch[diameter[i+1]], (branch[diameter[i]]+1)/2 + (inversebranch[diameter[i+1]]+1)/2 + 1});
		//printf("tmpans = %lld \n", tmpans);
        ans = min(ans, tmpans);
    }
    printf("%lld\n", ans);
    
    return 0;
}
