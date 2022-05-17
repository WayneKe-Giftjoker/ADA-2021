#include <iostream>
#include <algorithm>
#include <queue>
#define ll long long

using namespace std;

//非常感謝 b08902149 徐晨祐 & b09902098 杜生一!!!

class Task{
    public:
        int idx;
        ll units, prof;
        ll s, e;
        ll done;
};

struct cmp{
    bool operator()(Task a, Task b){
        return a.e > b.e;
    }
};

bool cmp_(Task a, Task b){
    if(a.prof != b.prof)
        return a.prof > b.prof;
    return a.s < b.s;
}

bool cmp_in(Task a, Task b){
    if(a.s != b.s)
        return a.s < b.s;
    return a.e < b.e;
}

int main(int argc, char *argv[]){
    int N;
    vector<Task> tasks;
    priority_queue<Task, vector<Task>, cmp> minHeap;
    ll doneCnt[1<<12];

    scanf("%d", &N);
    Task t;
    for(int i = 0; i < N; i++){
        t.idx = i;
        scanf("%lld %lld %lld %lld", &t.s, &t.e, &t.units, &t.prof);
        tasks.push_back(t);
    }
    sort(tasks.begin(), tasks.end(), cmp_);
    // printf("Got data\n");
    ll prev = 0; ll curr; ll ans = 0; ll p;
    ll time; int curridx; ll lapse;
    bool psame = tasks[0].prof == tasks[N-1].prof;
    if(!psame){
        for(int num = 1; num <= N; num++){
            p = tasks[num-1].prof;
            time = 1; curridx = 0; curr = 0;
            sort(tasks.begin(), tasks.begin() + num, cmp_in);
            while(!minHeap.empty() || curridx < num){
                if(minHeap.empty()){
                    minHeap.push(tasks[curridx]);
                    time = minHeap.top().s;
                    curridx++;
                }
                while(curridx < num && tasks[curridx].s <= time){
                    minHeap.push(tasks[curridx]);
                    curridx++;
                }
                if(curridx < num)
                    lapse = min({minHeap.top().units - doneCnt[minHeap.top().idx], minHeap.top().e - time + 1, tasks[curridx].s - time});
                else
                    lapse = min(minHeap.top().units - doneCnt[minHeap.top().idx], minHeap.top().e - time + 1);
                curr += lapse;
                doneCnt[minHeap.top().idx] += lapse;
                time += lapse;
                while(!minHeap.empty() && (minHeap.top().e < time || doneCnt[minHeap.top().idx] == minHeap.top().units))
                    minHeap.pop();
            }
            for(int i = 0; i < num; i++)
                doneCnt[tasks[i].idx] = 0;
            ans += (curr - prev) * p;
            prev = curr;
        }
    }
    else{
        time = 1; curridx = 0; curr = 0;
        while(!minHeap.empty() || curridx < N){
            if(minHeap.empty()){
                minHeap.push(tasks[curridx]);
                time = minHeap.top().s;
                curridx++;
            }
            while(curridx < N && tasks[curridx].s <= time){
                minHeap.push(tasks[curridx]);
                curridx++;
            }
            if(curridx < N)
                lapse = min({minHeap.top().units - doneCnt[minHeap.top().idx], minHeap.top().e - time + 1, tasks[curridx].s - time});
            else
                lapse = min(minHeap.top().units - doneCnt[minHeap.top().idx], minHeap.top().e - time + 1);
            curr += lapse;
            doneCnt[minHeap.top().idx] += lapse;
            time += lapse;
            while(!minHeap.empty() && (minHeap.top().e < time || doneCnt[minHeap.top().idx] == minHeap.top().units))
                minHeap.pop();
        }
        ans = curr * tasks[0].prof;
    }
    
    printf("%lld\n", ans);
    
    return 0;
}
