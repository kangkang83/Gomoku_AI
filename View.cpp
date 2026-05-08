#include "View.h"
#include <iostream>

using namespace std;

View::View(sf::RenderWindow* win) {
    this->window = win;
}

void View::Init() {
    
}

void View::renderGame(Board& board, int winner, pair<int, int> lastMove) {
    window->clear(sf::Color(235, 200, 150)); // Màu vàng gỗ #EBC896

    // 1. Vẽ lưới bàn cờ
    for (int i = 0; i < BOARD_SIZE; i++) {
        // Vẽ đường ngang
        sf::Vertex lineH[] = {
            sf::Vertex(sf::Vector2f(20, 20 + i * CELL_SIZE), sf::Color::Black),
            sf::Vertex(sf::Vector2f(20 + (BOARD_SIZE - 1) * CELL_SIZE, 20 + i * CELL_SIZE), sf::Color::Black)
        };
        window->draw(lineH, 2, sf::Lines);

        // Vẽ đường dọc
        sf::Vertex lineV[] = {
            sf::Vertex(sf::Vector2f(20 + i * CELL_SIZE, 20), sf::Color::Black),
            sf::Vertex(sf::Vector2f(20 + i * CELL_SIZE, 20 + (BOARD_SIZE - 1) * CELL_SIZE), sf::Color::Black)
        };
        window->draw(lineV, 2, sf::Lines);
    }

    // 2. Vẽ quân cờ
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board.grid[i][j] != EMPTY) {
                sf::CircleShape piece(CELL_SIZE / 2 - 2);
                piece.setPosition(20 + i * CELL_SIZE + 2, 20 + j * CELL_SIZE + 2);
                
                if (board.grid[i][j] == PLAYER) {
                    piece.setFillColor(sf::Color::Black);
                } else {
                    piece.setFillColor(sf::Color::White);
                    piece.setOutlineThickness(1);
                    piece.setOutlineColor(sf::Color::Black);
                }
                window->draw(piece);
            }
        }
    }
    
    // 3. Highlight nước đi cuối cùng
    if (lastMove.first != -1) {
        sf::CircleShape highlight(4); // Chấm đỏ nhỏ
        highlight.setFillColor(sf::Color::Red);
        // Canh giữa ô cờ
        highlight.setOrigin(4, 4);
        highlight.setPosition(20 + lastMove.first * CELL_SIZE + CELL_SIZE/2, 20 + lastMove.second * CELL_SIZE + CELL_SIZE/2);
        window->draw(highlight);
    }

    // 3. Hiển thị thông báo thắng thua (Tiêu đề cửa sổ)
    if (winner == PLAYER) {
        window->setTitle("Gomoku - BAN THANG!");
    } else if (winner == BOT) {
        window->setTitle("Gomoku - MAY THANG!");
    } else {
        window->setTitle("Gomoku AI - TranKyAnh-DH52200339");
    }

    window->display();
}

sf::Vector2i View::getGridPos(int mouseX, int mouseY) {
    // Trừ offset (20) và chia cho kích thước ô
    int x = (mouseX - 20) / CELL_SIZE; 
    int y = (mouseY - 20) / CELL_SIZE;
    
    // Điều chỉnh để click gần giao điểm nào thì chọn giao điểm đó (nếu vẽ theo kiểu lưới cờ tướng)
    // Nhưng ở đây ta vẽ ô vuông, nên click vào trong ô là được.
    // Tuy nhiên, logic trên đang tính x là cột (col), y là hàng (row) nếu vẽ theo trục tọa độ. 
    // Trong Board.grid[x][y], ta cần thống nhất x, y. 
    // Ở đoạn vẽ: x = 20 + i * CELL, y = 20 + j * CELL => x là i, y là j.
    // getGridPos cần trả về i, j.
    
    // Với cách tính: int x = (mouseX - 20) / CELL_SIZE; 
    // Nếu mouseX = 25 (vừa qua lề 20), x = 0. Đúng.
    
    // Tuy nhiên, nếu muốn chính xác hơn (click vào gần tâm ô), có thể không cần chỉnh sửa vì ô đủ lớn (40px).
    
    return sf::Vector2i(x, y);
}
