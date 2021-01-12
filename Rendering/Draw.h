#ifndef DRAW_H
#define DRAW_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <stack>

#include "Core/Object.h"
#include "Utils/DebugTools.h"
#include "Utils/Common.h"
#include "Physics/vec.h"
#include "Drawable.h"

class GameWindow;

class DrawSurface
{
    GameWindow& m_window;

    std::vector<Ref<sf::Text>> m_texts;
    sf::Color m_clear_color { sf::Color::Black };
    sf::View m_gui_view;
    std::stack<const Drawable*> m_drawables;

public:
    DrawSurface(GameWindow& window);

    void set_clear_color(sf::Color color) { m_clear_color = color; }
    const sf::Color& clear_color() const { return m_clear_color; }

    void draw(const Drawable& drawable);

    GameWindow& window() { return m_window; }
    const GameWindow& window() const { return m_window; }

    void finalize();
};

#endif // DRAW_H
