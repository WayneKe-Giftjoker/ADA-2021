#include "connect4.h" #TA provided
#include <iostream>
#include <vector>
#include <climits>
#include <cstdlib>
#include <ctime>
#define ll long long
#define ME 1
#define EMPTY 0
#define YP 2
#define NONE -1
#define ROWN 6
#define COLN 7
#define WINDOWLEN 4
#define INF LLONG_MAX
#define NINF LLONG_MIN
#define DEP 3
//感謝b06303131 沈家睿 & b08902149 徐晨祐 & b08502041 李芸芳的協助!!!
//reference: https://github.com/KeithGalli/Connect4-Python/blob/master/connect4_with_ai.py?fbclid=IwAR1lZbBoWsbVU-GhsWCqjW7YlSukyNk8tsXHT8_CXacWBeyPSHMtWxgkNFw

using namespace std;

vector<vector<int>> board(1<<3, vector<int>(1<<3));

bool isValidLocation(vector<vector<int>> &B, int col){
    return B[ROWN-1][col] == EMPTY;
}

vector<int> getValidLocations(vector<vector<int>> &B){
    vector<int> ret;
    for(int col = 0; col < COLN; col++){
        if(isValidLocation(B, col))
            ret.push_back(col);
    }
    return ret;
}

bool winningMove(vector<vector<int>> &B, int piece){
    for(int c = 0; c < COLN-3; c++)//hori
        for(int r = 0; r < ROWN; r++)
            if(B[r][c] == piece && B[r][c+1] == piece && B[r][c+2] == piece && B[r][c+3] == piece)
                return true;
    
    for(int c = 0; c < COLN; c++)//ver
        for(int r = 0; r < ROWN-3; r++)
            if(B[r][c] == piece && B[r+1][c] == piece && B[r+2][c] == piece && B[r+3][c] == piece)
                return true;
    
    for(int c = 0; c < COLN-3; c++)//pos dia
        for(int r = 0; r < ROWN-3; r++)
            if(B[r][c] == piece && B[r+1][c+1] == piece && B[r+2][c+2] == piece && B[r+3][c+3] == piece)
                return true;
    
    for(int c = 0; c < COLN-3; c++)//neg dia
        for(int r = 3; r < ROWN; r++)
            if(B[r][c] == piece && B[r-1][c+1] == piece && B[r-2][c+2] == piece && B[r-3][c+3] == piece)
                return true;

    return false;
}

bool isTerminalNode(vector<vector<int>> &B){
    return (winningMove(B, ME) || winningMove(B, YP) || int(getValidLocations(B).size()) == 0);
}

ll vectorCnt(vector<int> &vec, int obj){
    ll cnt = 0;
    for(auto it: vec)
        if(it == obj)
            cnt++;
    return cnt;
}

ll evaluateWindow(vector<int> &window, int piece){
    ll score = 0;
    int opp = (piece == ME)? YP : ME;
    
    ll cnt = vectorCnt(window, piece);
    ll emptyCnt = vectorCnt(window, EMPTY);
    ll oppCnt = vectorCnt(window, opp);

    if(cnt == 4)
        score += 225;//215
    else if(cnt == 3 && emptyCnt == 1)
        score += 78;//78
    else if(cnt == 2 && emptyCnt == 2)
        score += 8;//8
    
    if(oppCnt == 3 && emptyCnt == 1)
        score -= 28;//28
    
    return score;
}

ll scorePos(vector<vector<int>> &B, int piece){
    ll score = 0;
    
    vector<int> centerAry;
    for(int i = 0; i < ROWN; i++) centerAry.push_back(B[i][3]);//3 = COLN / 2
    ll centerCnt = vectorCnt(centerAry, piece);
    score += centerCnt * 3;//center
    
    for(int r = 0; r < ROWN; r++){//hori
        vector<int> rowAry;
        for(int i = 0; i < COLN; i++) rowAry.push_back(B[r][i]);
        for(int c = 0; c < COLN-3; c++){
            vector<int> window;
            window.assign(rowAry.cbegin()+c, rowAry.cbegin()+c+WINDOWLEN);
            score += evaluateWindow(window, piece);
        }
    }
    
    for(int c = 0; c < COLN; c++){//verti
        vector<int> colAry;
        for(int i = 0; i < ROWN; i++) colAry.push_back(B[i][c]);
        for(int r = 0; r < ROWN-3; r++){
            vector<int> window;
            window.assign(colAry.cbegin()+r, colAry.cbegin()+r+WINDOWLEN);
            score += evaluateWindow(window, piece);
        }
    }
    
    for(int r = 0; r < ROWN-3; r++){
        for(int c = 0; c < COLN-3; c++){
            vector<int> window;
            for(int i = 0; i < WINDOWLEN; i++) window.push_back(B[r+i][c+i]);
            score += evaluateWindow(window, piece);
        }
    }
    
    for(int r = 0; r < ROWN-3; r++){
        for(int c = 0; c < COLN-3; c++){
            vector<int> window;
            for(int i = 0; i < WINDOWLEN; i++) window.push_back(B[r+3-i][c+i]);
            score += evaluateWindow(window, piece);
        }
    }

    return score;
}

int randChoice(vector<int> &vec){
    int n = vec.size();
    int idx = rand()%n;
    return vec[idx];
}

int getNextOpenRow(vector<vector<int>> &B, int col){
    for(int r = 0; r < ROWN; r++)
        if(B[r][col] == 0)  
            return r;
}

void dropPiece(vector<vector<int>> &B, int row, int col, int piece){
    B[row][col] = piece;
}

vector<vector<int>> vecCpy(vector<vector<int>> &vec){
    vector<vector<int>> ret(1<<3, vector<int>(1<<3));
    for(int r = 0; r < 8; r++){
        for(int c = 0; c < 8; c++){
            ret[r][c] = vec[r][c];
        }
    }
    return ret;
}

pair<int, ll> minimax(vector<vector<int>> &B, int depth, ll alpha, ll beta, bool maxPlayer){
    vector<int> validLoc = getValidLocations(B);
    bool isTerminal = isTerminalNode(B);
    if(depth == 0 || isTerminal){
        if(isTerminal){
            if(winningMove(B, ME))
                return {NONE, INF};
            else if(winningMove(B, YP))
                return {NONE, NINF};
            else
                return {NONE, 0};
        }
        else
            return {NONE, scorePos(B, ME)};
    }
    
    if(maxPlayer){
        ll value = NINF;
        int column = randChoice(validLoc);
        for(auto col: validLoc){
            int row = getNextOpenRow(B, col);
            vector<vector<int>> Bcpy = vecCpy(B);
            dropPiece(Bcpy, row, col, ME);
            ll newScore = minimax(Bcpy, depth-1, alpha, beta, false).second;
            if(newScore > value){
                value = newScore;
                column = col;
            }
            alpha = max(alpha, value);
            if(alpha >= beta)
                break;
        }
        return {column, value};
    }
    else{
        ll value = INF;
        int column = randChoice(validLoc);
        for(auto col: validLoc){
            int row = getNextOpenRow(B, col);
            vector<vector<int>> Bcpy = vecCpy(B);
            dropPiece(Bcpy, row, col, YP);
            ll newScore = minimax(Bcpy, depth-1, alpha, beta, true).second;
            if(newScore < value){
                value = newScore;
                column = col;
            }
            beta = min(beta, value);
            if(alpha >= beta)
                break;
        }
        return {column, value};
    }
}

int decide(int yp_move){
    if(yp_move == -1){
        //new game
        srand(1219);
        for(int r = 0; r < 8; r++)
            for(int c = 0; c < 8; c++)
                board[r][c] = EMPTY;
        int col = 3;
        int row = getNextOpenRow(board, col);
        dropPiece(board, row, col, ME);
        return col;
    }
    else{
        //yp placed at col yp_move
        int yprow = getNextOpenRow(board, yp_move);
        dropPiece(board, yprow, yp_move, YP);
        int col = minimax(board, DEP, NINF, INF, true).first;
        if(col != NONE && isValidLocation(board, col)){
            int row = getNextOpenRow(board, col);
            dropPiece(board, row, col, ME);
        }
        return col;
    }
}
