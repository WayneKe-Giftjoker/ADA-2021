#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;
//thanks to b06303131 沈家睿 for helping!!!

int N;
vector <pair <int, int>> p({});
vector <pair <int, int>> lStack({});
vector <pair <int, int>> rStack({});
long long ans;

void getData(){
    scanf("%d", &N);
    int x, y;
    for(int i = 0; i < N; i++){
        scanf("%d %d", &x, &y);
        p.push_back({x, y});
    }
}

bool cmpX(pair <int, int> a, pair <int, int> b){
    return a.first < b.first;
}

bool cmpY(pair <int, int> a, pair <int, int> b){
    return a.second < b.second;
}

bool cmp(pair <int, int> a, int v){
    return a.second <= v;
}

void DnC(int l, int r){
    if(r > l){
        int mid = (l + r) / 2;
        int midLine = (p[mid].first + p[mid+1].first) / 2;
        DnC(l, mid);
        DnC(mid + 1, r);
        sort(p.begin()+l, p.begin()+r+1, cmpY);
        rStack.clear(); lStack.clear();
        for(int i = l; i <= r; i++){
            // printf("-----------i: %d\n", i);
            if(p[i].first > midLine){//right case
                while(rStack.size() > 0 && rStack.back().first > p[i].first){
                    rStack.pop_back();
                }
                if(rStack.size() == 0)
                    ans += lStack.size();
                else{
                    ans += lStack.end() - lower_bound(lStack.begin(), lStack.end(), rStack.back().second, cmp);
                }
                rStack.push_back(p[i]);
            }
            else{
                while(lStack.size() > 0 && lStack.back().first < p[i].first){
                    lStack.pop_back();
                }
                if(lStack.size() == 0)
                    ans += rStack.size();
                else{
                    ans += rStack.end() - lower_bound(rStack.begin(), rStack.end(), lStack.back().second, cmp);
                }
                lStack.push_back(p[i]);
            }
            // printf("=====ans: %lld\n", ans);
        }
    }
}

int main(int argc, char *argv[]){
    getData();
    sort(p.begin(), p.end(), cmpX);
    DnC(0, N-1);
    printf("%lld\n", ans);
    
    return 0;
}
