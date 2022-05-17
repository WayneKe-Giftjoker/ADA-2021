#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#define ll long long
#define FOOD 1
#define MOVE 2

using namespace std;

//感謝 b06303131 沈家睿 的協助!!!

ll N;

typedef struct node{
    ll size, pos;
    struct node *par;
}Node;

Node *genNode(ll pos, ll size){
    Node *n = (Node *)malloc(sizeof(Node));
    n->par = n;
    n->pos = pos;
    n->size = size;
    return n;
}

Node* findRoot(Node* x){
    if(x != x->par)
        x->par= findRoot(x->par);
    return x->par;
}

int main(int argc, char *argv[]){
    ll Q;
    scanf("%lld %lld", &N, &Q);
    vector<Node *> cats(N+1);
    map<ll, Node *> mp; //map<input_val, output_val>
    ll crowdness = 0;
    vector<ll> combination(N+1);
    for(ll i = 0; i <= N; i++){
        combination[i] = i*(i-1)/2;
    }
    ll pos; Node *tmp;
    for(int i = 1; i <= N; i++){
        scanf("%lld", &pos);
        if(mp.count(pos)){
            tmp = mp[pos];
            tmp->size++;
            cats[i] = tmp;
            crowdness += (combination[tmp->size] - combination[tmp->size-1]);
        }
        else{
            tmp = genNode(pos, 1);
            mp[pos] = tmp;
            cats[i] = tmp;
        }
    }
    int cmd; ll rad, des, t; Node *tar;
    while(Q--){
        scanf("%d %lld %lld", &cmd, &pos, &t);
        if(cmd == FOOD){
            rad = t;
            if(!mp.count(pos)){
                tmp = genNode(pos, 0);
                mp[pos] = tmp;
            }
            tar = mp[pos];
            // vector<ll> rec;
            for(auto left = mp.lower_bound(pos-rad); left != mp.end();){
                if(left->first > rad + pos)
                    break;
                if(left->first == pos){
                    left++;
                    continue;
                }
                tmp = mp[left->first];
                tmp->par = tar;
                tar->size += tmp->size;
                crowdness += (combination[tar->size] - combination[tar->size - tmp->size] - combination[tmp->size]);
                left = mp.erase(left);
            }
        }
        else{
            des = t;
            tmp = findRoot(cats[pos]);
            tmp->size--;
            ll dec = combination[tmp->size+1] - combination[tmp->size];
            if(!mp.count(des)){
                tar = genNode(des, 0);
                mp[des] = tar;
            }
            tar = mp[des];
            cats[pos] = tar;
            tar->size++;
            ll inc = combination[tar->size] - combination[tar->size - 1];
            crowdness += inc - dec;
            if(tmp->size == 0)
                mp.erase(tmp->pos);
        }
        printf("%lld\n", crowdness);
    }
    
    return 0;
}
