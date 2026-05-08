#ifndef BOARD_H
#define BOARD_H

#include "Global.h"

class Board {
public:
    int grid[BOARD_SIZE][BOARD_SIZE];
    int count; // Số nước đã đánh

    Board();
    void reset();
    bool isValid(int x, int y);
    void move(int x, int y, int player);
    void undo(int x, int y);
    
    bool checkWin(int x, int y, int player);
    
    
    long long evaluateBoard(int player);
    
   
    std::vector<std::pair<int, int>> getEmptyNeighbors();
};

#endif // BOARD_H
