#ifndef AI_H
#define AI_H

#include "Board.h"
#include "Global.h"

struct Move {
    int x, y;
    long long score;
};

class AI {
public:
    AI();
    Move findBestMove(Board& board);
    long long minimax(Board& board, int depth, long long alpha, long long beta, bool isMaximizing);
    long long evaluate(Board& board, int player);
    
    
    long long evaluateLine(int count, int blocks, int currentTurn);

   
    long long evaluateMove(Board& board, int x, int y, int player);
};

#endif 
