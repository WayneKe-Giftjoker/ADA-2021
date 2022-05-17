#include <iostream>
#include <vector>
#include <algorithm>
//非常感謝沈家睿同學的協助!

using namespace std;

vector<pair<long long, long long>> arr({});
pair<long long, long long> record[2000000];

int N, a, b, c;
long long ans;

void getData(){
    scanf("%d %d %d %d", &N, &a, &b, &c);
    long long p, z;
    for(int i = 0; i < N; i++){
        scanf("%lld %lld", &p, &z);
        arr.push_back({p,z});
    }
}

bool cmp(pair<long long, long long> x, pair<long long, long long> y){
    if(x.second != y.second)
        return x.second < y.second;
    return x.first < y.first;
}

int findMid(long long tar, int start, int mid, int end){
    int result = mid;
    while(result < end && arr[result].second == tar){
        result++;
    }
    if(result == end && arr[result].second == tar){
        result = mid;
        while(start < result && arr[result].second == tar){
            result--;
        }
        return result;
    }
    else
        return result - 1;
    // printf("RESULT: %d\n", result - 1);
}

void mergeSort(int start, int end){
    // printf("ANS: %lld\n", ans);
    if(end > start){
        if(arr[start].second == arr[end].second){
            // printf("IF\n");
            int mid = (end + start) / 2;
            int idx1 = start, idx2 = mid + 1, idx = start;

            // for(auto it: arr){
            //     cout << it.first << ' ' << it.second << '\n';
            // }

            while(idx1 <= mid && idx2 <= end){
                if(arr[idx2].first <= arr[idx1].first){
                    record[idx++] = arr[idx2++];
                }
                else{
                    record[idx++] = arr[idx1++];
                }
            }
            while(idx1 <= mid){
                record[idx++] = arr[idx1++];
            }
            while(idx2 <= end){
                record[idx++] = arr[idx2++];
            }
            for(int i = start; i <= end; i++)
                arr[i] = record[i];

            // for(auto it: arr){
            //     cout << it.first << ' ' << it.second << '\n';
            // }
        }
        else{
            // printf("ELSE\n");
            int mid = findMid(arr[(end + start) / 2].second, start, (end + start) / 2, end);
            // printf("MID: %d\n", mid);
            mergeSort(start, mid);
            mergeSort(mid + 1, end);

            int idx1 = start, idx2 = mid + 1, idx = start;
            // printf("MID: %d || IDX1: %d\n", mid, idx1);
            // for(auto it: arr){
            //     cout << it.first << ' ' << it.second << '\n';
            // }
            while(idx1 <= mid && idx2 <= end){
                if(a*arr[idx2].first <= b*arr[idx1].first + c)
                    idx2++;
                else{
                    // printf("IDX2: %d\n", idx2);
                    ans += (end - idx2 + 1);
                    idx1++;
                }
            }
            while(idx1 <= mid){
                // printf("IDX2: %d\n", idx2);
                ans += (end - idx2 + 1);
                idx1++;
            }

            idx1 = start; idx2 = mid + 1; idx = start;
            while(idx1 <= mid && idx2 <= end){
                if(arr[idx2].first <= arr[idx1].first){
                    record[idx++] = arr[idx2++];
                }
                else{
                    record[idx++] = arr[idx1++];
                }
            }
            while(idx1 <= mid){
                record[idx++] = arr[idx1++];
            }
            while(idx2 <= end){
                record[idx++] = arr[idx2++];
            }
            for(int i = start; i <= end; i++)
                arr[i] = record[i];
        }
    }
    
}

int main(int argc, char *argv[]){
    // printf("NAS: %lld\n", ans);
    getData();
    sort(arr.begin(), arr.end(), cmp);
    mergeSort(0, N-1);
    // printf("DONE\n");
    printf("%lld\n", ans);
    
    return 0;
}
