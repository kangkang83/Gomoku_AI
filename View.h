#ifndef VIEW_H
#define VIEW_H

#include <SFML/Graphics.hpp>
#include "Board.h"
#include "Global.h"

class View {
private:
    sf::RenderWindow* window;
    sf::Font font;
    sf::Text statusText;
    
public:
    View(sf::RenderWindow* win);
    void Init();
    void renderGame(Board& board, int winner, std::pair<int, int> lastMove);
    
    // Chuyển đổi tọa độ chuột sang tọa độ bàn cờ
    sf::Vector2i getGridPos(int mouseX, int mouseY);
};

#endif // VIEW_H
