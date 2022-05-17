#include "ada-hw4-p3.h" //TA provided

// feel free to use global variables
#define MAXN 1<<12
//感謝b08902149 徐晨祐 & b06303131 沈家睿的協助!!!
std::vector<int>  contest;
std::vector<int>  posToId;//posToId[pos] = Id
std::vector<int>  pairRecord(MAXN);//(id, id)
std::vector<bool> concealed(MAXN, false);//concealed[id]

std::vector<int> init(int N) {
    posToId.resize(N);
    for (int i = 0; i < N; ++i){
        // concealed[i] = false;
        posToId[i] = i;
        pairRecord[i] = -1;
    }
    int L = 0, R = 1;
    while(R < N){
        if(compare(L, R)){L = R; R++;}
        else{
            contest.push_back(L); contest.push_back(R);
            pairRecord[L] = R; pairRecord[R] = L;
            concealed[L] = concealed[R] = true;
            R++;
            while(L >= 0 && concealed[L]){
                L--;
            }
            if(L < 0){R++; L = R-1;}
        }
    }
    return contest;
}

std::vector<int> insert(int p, int id) {
    posToId.insert(posToId.begin() + p, id);
    int N = posToId.size();
    int L = p-1, R = p+1;
    
    while(L >= 0 && concealed[posToId[L]]){
        L--;
    }
    if(L >= 0 && !compare(posToId[L], id)){
        contest.push_back(posToId[L]); contest.push_back(id);
        pairRecord[posToId[L]] = id; pairRecord[id] = posToId[L];
        concealed[posToId[L]] = concealed[id] = true;
        return contest;
    }
    
    while(R < N && concealed[posToId[R]]){
        R++;
    }
    if(R < N && !compare(id, posToId[R])){
        contest.push_back(id); contest.push_back(posToId[R]);
        pairRecord[id] = posToId[R]; pairRecord[posToId[R]] = id;
        concealed[id] = concealed[posToId[R]] = true;
        return contest;
    }

    return contest;
}

std::vector<int> remove(int p) {
    if(concealed[posToId[p]]){
        int otherhalf = pairRecord[posToId[p]];
        int p_i = 0, n = contest.size();
        for(; p_i < n; p_i++)
            if(contest[p_i] == posToId[p])
                break;
        contest.erase(contest.begin() + p_i);
        concealed[posToId[p]] = false;
        pairRecord[posToId[p]] = -1;
        posToId.erase(posToId.begin() + p);
        
        int N = posToId.size();
        int oh_pos = 0;//otherhalf position
        for(; oh_pos < N; oh_pos++){
            if(posToId[oh_pos] == otherhalf)
                break;
        }
        if(oh_pos >= N)
            return contest;
        
        n = contest.size();
        int cont_oh_pos = 0;//otherhalf contest position
        for(; cont_oh_pos < n; cont_oh_pos++){
            if(contest[cont_oh_pos] == otherhalf)
                break;
        }
        contest.erase(contest.begin() + cont_oh_pos);
        concealed[otherhalf] = false; pairRecord[otherhalf] = -1;
        
        
        int L = oh_pos-1, R = oh_pos+1;
        while(L >= 0 && concealed[posToId[L]]){
            L--;
        }
        if(L >= 0 && !compare(posToId[L], otherhalf)){
            contest.push_back(posToId[L]); contest.push_back(otherhalf);
            pairRecord[posToId[L]] = otherhalf; pairRecord[otherhalf] = posToId[L];
            concealed[posToId[L]] = concealed[otherhalf] = true;
            return contest;
        }
        while(R < N && concealed[posToId[R]]){
            R++;
        }
        if(R < N && !compare(otherhalf, posToId[R])){
            contest.push_back(otherhalf); contest.push_back(posToId[R]);
            pairRecord[otherhalf] = posToId[R]; pairRecord[posToId[R]] = otherhalf;
            concealed[otherhalf] = concealed[posToId[R]] = true;
            return contest;
        }
    }
    else{
        posToId.erase(posToId.begin() + p);
    }
    return contest;
}
