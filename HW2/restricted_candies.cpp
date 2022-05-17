#include <iostream>
#include <algorithm>
#include <queue>

using namespace std;

int N, N1, N2, flag;
long long candies[1<<17];
long long Candies1[1<<17];
long long Candies2[1<<17];
long long ans[1<<17];

typedef struct node{
    long long value;
    pair <int, int> idx;
    int dirty;
    struct node *prev, *next;
} Node;

Node *genNode(int idx1, int idx2){
    Node *n = (Node *)malloc(sizeof(Node));
    n->idx = {idx1, idx2};
    if(idx1 != idx2)
        n->value = candies[idx1] + candies[idx2];
    else
        n->value = candies[idx1];
    n->dirty = 0;
    n->prev = n->next = NULL;
    return n;
}

class myDeque{
    public:
        int size;
        Node *head;
        myDeque(): size(0), head(0){}
        void buildDeque(long long *arr, int n);
        bool isempty();
        Node *deleteNode(Node *tar);
        void clear();
};

void myDeque::buildDeque(long long *arr, int n){
    if(n == 0){
        return;
    }
    else if(n == 1){
        this->head = genNode(arr[0], arr[0]);
        this->size = n;
        this->head->next = this->head;
        this->head->prev = this->head;
    }
    else{
        this->head = genNode(arr[0], arr[1]);
        Node *ptr = this->head;
        for(int i = 1; i < n; i++){
            ptr->next = genNode(arr[i], arr[(i + 1) % n]);
            ptr->next->prev = ptr;
            ptr = ptr->next;
        }
        this->size = n;
        this->head->prev = ptr;
        ptr->next = this->head;
    }
}

bool myDeque::isempty(){
    if(this->size)
        return false;
    return true;
}

Node *myDeque::deleteNode(Node *tar){
    if(!tar)
        return NULL;
    Node *pNode, *nNode;
    pNode = tar->prev; nNode = tar->next;
    tar->dirty = 1;
    Node *newNode = genNode(pNode->idx.first, nNode->idx.second);
    if(pNode != NULL){
        pNode->dirty = 1;
        newNode->prev = pNode->prev;
        pNode->prev->next = newNode;
        this->size--;
    }
    if(nNode != NULL){
        nNode->dirty = 1;
        newNode->next = nNode->next;
        nNode->next->prev = newNode;
        this->size--;
    }
    // printf("----delete--prev: (%d, %d) || next: (%d, %d)----\n", pNode->idx.first, pNode->idx.second, nNode->idx.first, nNode->idx.second);
    if(tar == this->head)
        this->head = newNode;
    return newNode;
}

void myDeque::clear(){
    this->size = 0;
    this->head = NULL;
}

struct cmp{
    bool operator()(Node* a, Node* b){
        return a->value > b->value;
    }
};

myDeque deq1, deq2;
priority_queue<Node*, vector<Node*>, cmp> pq1, pq2;

void Clear(){
    for(int i = 1; i <= N; i++)
        ans[i] = 0;
    deq1.clear(); deq2.clear();
    N1 = N2 = 0;
    while(!pq1.empty())
        pq1.pop();
    while(!pq2.empty())
        pq2.pop();
}

void getData(){
    scanf("%d", &N);
    for(int i = 0; i < N; i++)
        scanf("%lld", &candies[i]);
    // deq.buildDeque(candies, N);
}

void preprocess(){
    int state = 0;//POS: 0; NEG: 1
    int tmp = 0;
    long long tmpans = 0;
    for(int i = 1; i < N; i++){
        if(candies[i-1]*candies[i] < 0){
            Candies1[N1++] = tmp;
            tmpans += candies[tmp];
            tmp = i;
        }
        else{
            if(candies[i] > candies[tmp])
                tmp = i; 
        }
    }
    Candies1[N1++] = tmp;
    tmpans += candies[tmp];
    ans[N1] = tmpans;
    if(candies[Candies1[0]] > candies[Candies1[N1-1]]){
        for(int i = 0; i < N1 - 1; i++)
            Candies2[i] = Candies1[i];
        tmpans -= candies[Candies1[N1-1]];
    }
    else{
        for(int i = 1; i < N1; i++)
            Candies2[i - 1] = Candies1[i];
        tmpans -= candies[Candies1[0]];
    }
    if(N1 > 1){
        N2 = N1 - 1;
        ans[N2] = tmpans;
    }
    deq1.buildDeque(Candies1, N1);
    deq2.buildDeque(Candies2, N2);
}

void buildMinHeap(){
    Node *ptr = deq1.head;
    for(int i = 0; i < N1; i++){
        pq1.push(ptr);
        ptr = ptr->next;
    }
    ptr = deq2.head;
    for(int i = 0; i < N2; i++){
        pq2.push(ptr);
        ptr = ptr->next;
    }
}

Node *getMin(priority_queue<Node*, vector<Node*>, cmp> &pq){
    while(pq.top()->dirty && !pq.empty()){
        pq.pop();
    }
    return pq.top();
}

void printDeq(){
    Node *ptr = deq1.head;
    int done = 0;
    printf("deq1 %d items\n", N1);
    while(ptr != NULL && done <= N1){
        printf("===deq1===val: %lld idx: (%d, %d) dirty: %d===\n", ptr->value, ptr->idx.first, ptr->idx.second, ptr->dirty);
        done++;
        ptr = ptr->next;
    }
    ptr = deq2.head;
    done = 0;
    printf("deq2 %d items\n", N2);
    while(ptr != NULL && done <= N2){
        printf("===deq2===val: %lld idx: (%d, %d) dirty: %d===\n", ptr->value, ptr->idx.first, ptr->idx.second, ptr->dirty);
        done++;
        ptr = ptr->next;
    }
}

int main(int argc, char *argv[]){
    int T;
    scanf("%d %d", &T, &flag);
    while(T--){
        getData();
        preprocess();
        // printDeq();
        buildMinHeap();
        Node *ptr = NULL;
        N1 -= 2;
        while(N1 > 0){
            // printf("N1: %d\n", N1);
            ptr = getMin(pq1);
            ans[N1] = ans[N1+2] - ptr->value;
            ptr = deq1.deleteNode(ptr);
            // printDeq();
            pq1.push(ptr);
            N1 -= 2;
        }
        N2 -= 2;
        while(N2 > 0){
            ptr = getMin(pq2);
            ans[N2] = ans[N2+2] - ptr->value;
            ptr = deq2.deleteNode(ptr);
            pq2.push(ptr);
            N2 -= 2;
        }
        for(int i = 1; i <= N; i++)
            printf("%lld ", ans[i]);
        printf("\n");
        Clear();
    }
    
    return 0;
}
