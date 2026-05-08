#include "Board.h"
#include <cstring>
#include <algorithm>

using namespace std;

Board::Board() {
    reset();
}

void Board::reset() {
    memset(grid, 0, sizeof(grid));
    count = 0;
}

bool Board::isValid(int x, int y) {
    return x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE && grid[x][y] == EMPTY;
}

void Board::move(int x, int y, int player) {
    if (isValid(x, y)) {
        grid[x][y] = player;
        count++;
    }
}

void Board::undo(int x, int y) {
    if (x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE) {
        grid[x][y] = EMPTY;
        count--;
    }
}

// Kiểm tra 4 hướng: Ngang, Dọc, Chéo chính, Chéo phụ
bool Board::checkWin(int x, int y, int player) {
    int dirs[4][2] = {{1, 0}, {0, 1}, {1, 1}, {1, -1}};
    
    for (int d = 0; d < 4; d++) {
        int count = 1;
        int dx = dirs[d][0];
        int dy = dirs[d][1];
        
        // Kiểm tra chiều xuôi
        int i = x + dx;
        int j = y + dy;
        while (i >= 0 && i < BOARD_SIZE && j >= 0 && j < BOARD_SIZE && grid[i][j] == player) {
            count++;
            i += dx;
            j += dy;
        }
        
        // Kiểm tra chiều ngược
        i = x - dx;
        j = y - dy;
        while (i >= 0 && i < BOARD_SIZE && j >= 0 && j < BOARD_SIZE && grid[i][j] == player) {
            count++;
            i -= dx;
            j -= dy;
        }
        
        if (count >= 5) return true;
    }
    return false;
}

// Hàm đánh giá heuristic
long long Board::evaluateBoard(int player) {
    return 0; 
}

// Lấy các ô trống xung quanh các ô đã đánh (để giảm không gian tìm kiếm cho AI)
vector<pair<int, int>> Board::getEmptyNeighbors() {
    vector<pair<int, int>> moves;
    bool visited[BOARD_SIZE][BOARD_SIZE] = {false};
    
    // Duyệt qua tất cả ô trên bàn cờ, nếu ô đó đã đánh thì thêm các ô trống xung quanh nó
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (grid[i][j] != EMPTY) {
                for (int dx = -1; dx <= 1; dx++) {
                    for (int dy = -1; dy <= 1; dy++) {
                        if (dx == 0 && dy == 0) continue;
                        int ni = i + dx;
                        int nj = j + dy;
                        if (ni >= 0 && ni < BOARD_SIZE && nj >= 0 && nj < BOARD_SIZE && grid[ni][nj] == EMPTY && !visited[ni][nj]) {
                            moves.push_back({ni, nj});
                            visited[ni][nj] = true;
                        }
                    }
                }
            }
        }
    }
    
    // Nếu bàn cờ trống (lượt đầu tiên), đánh vào giữa
    if (moves.empty() && grid[BOARD_SIZE/2][BOARD_SIZE/2] == EMPTY) {
        moves.push_back({BOARD_SIZE/2, BOARD_SIZE/2});
    }
    
    return moves;
}

