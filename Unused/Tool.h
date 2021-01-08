#ifndef TOOL_H
#define TOOL_H

#include <SFML/Graphics.hpp>

class GameWindow;

class Tool
{
protected:
    GameWindow* m_window;

public:
    Tool(GameWindow* window);
    virtual ~Tool() = default;
    Tool(Tool&&) = default;
    Tool& operator=(Tool&&) = default;

    Tool(const Tool&) = delete;
    Tool& operator=(const Tool&) = delete;

    virtual void draw() { }

    virtual void handle_mouse_button_press(sf::Mouse::Button, sf::Vector2i pos) = 0;
    virtual void handle_mouse_button_release(sf::Mouse::Button, sf::Vector2i pos) = 0;
};

#endif // TOOL_H
