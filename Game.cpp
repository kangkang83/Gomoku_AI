#include "Game.h"
#include <iostream>

using namespace std;

Game::Game() {
    // Tạo cửa sổ với Anti-aliasing
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    
    int windowSize = BOARD_SIZE * CELL_SIZE + 40; // +40 cho lề
    window = new sf::RenderWindow(sf::VideoMode(windowSize, windowSize), "Gomoku AI - C++_trankyanh_dh52200339", sf::Style::Close, settings);
    window->setFramerateLimit(60);
    
    view = new View(window);
    view->Init();
    
    board.reset();
    currentTurn = PLAYER; // Người đi trước
    gameOver = false;
    winner = 0;
    lastMove = {-1, -1}; // Khởi tạo chưa có nước đi nào
}

Game::~Game() {
    delete view;
    delete window;
}

void Game::run() {
    while (window->isOpen()) {
        processEvents();
        update();
        render();
    }
}

void Game::processEvents() {
    sf::Event event;
    while (window->pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window->close();
            
        // Reset Game
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R) {
            resetGame();
        }
            
        // Xử lý click chuột
        if (!gameOver && currentTurn == PLAYER && event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i pos = view->getGridPos(event.mouseButton.x, event.mouseButton.y);
                
                // Kiểm tra nước đi hợp lệ
                if (board.isValid(pos.x, pos.y)) {
                    board.move(pos.x, pos.y, PLAYER);
                    lastMove = {pos.x, pos.y};
                    
                    // Kiểm tra thắng
                    if (board.checkWin(pos.x, pos.y, PLAYER)) {
                        gameOver = true;
                        winner = PLAYER;
                        cout << "Nguoi choi thang!" << endl;
                    } else {
                        currentTurn = BOT; // Đến lượt máy
                    }
                }
            }
        }
    }
}

void Game::update() {
    if (!gameOver && currentTurn == BOT) {
        // Máy suy nghĩ
        // Có thể render một lần trước khi máy nghĩ để hiển thị nước người vừa đi
        render(); 
        
        Move bestMove = ai.findBestMove(board);
        
        if (bestMove.x != -1) {
            board.move(bestMove.x, bestMove.y, BOT);
            lastMove = {bestMove.x, bestMove.y};
            
            if (board.checkWin(bestMove.x, bestMove.y, BOT)) {
                gameOver = true;
                winner = BOT;
                cout << "May thang!" << endl;
            } else {
                currentTurn = PLAYER;
            }
        } else {
            // Hòa (het nuoc o)
            gameOver = true;
            winner = 3;
        }
    }
}

void Game::render() {
    view->renderGame(board, winner, lastMove);
}

void Game::resetGame() {
    board.reset();
    currentTurn = PLAYER;
    gameOver = false;
    winner = 0;
    lastMove = {-1, -1};
    cout << "--- GAME RESTARTED ---" << endl;
}
