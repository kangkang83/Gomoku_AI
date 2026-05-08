#ifndef GAME_H
#define GAME_H

#include "Board.h"
#include "AI.h"
#include "View.h"
#include <SFML/Graphics.hpp>

class Game {
private:
    Board board;
    AI ai;
    View* view;
    sf::RenderWindow* window;
    
    int currentTurn; // PLAYER hoặc BOT
    bool gameOver;
    int winner; // 0: chưa, 1: Player, 2: Bot, 3: Hòa
    std::pair<int, int> lastMove; // Lưu nước đi cuối cùng để highlight

public:
    Game();
    ~Game();
    void run();
    void processEvents();
    void update();
    void render();
    void resetGame();
};

#endif // GAME_H
