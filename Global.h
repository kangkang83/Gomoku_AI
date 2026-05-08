#ifndef GLOBAL_H
#define GLOBAL_H

#include <vector>
#include <iostream>

// Kích thước bàn cờ
const int BOARD_SIZE = 15;
const int CELL_SIZE = 40; // Kích thước mỗi ô (pixel)

// Trạng thái ô cờ
const int EMPTY = 0;
const int PLAYER = 1; // Người chơi (Đen)
const int BOT = 2;    // Máy (Trắng)

// Điểm số cho AI
const int WIN_SCORE = 100000000;
const int LOSE_SCORE = -100000000;

#endif // GLOBAL_H
