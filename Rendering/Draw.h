#ifndef DRAW_H
#define DRAW_H

#include <SFML/Graphics.hpp>
#include <memory>
#include <stack>
#include <unordered_set>
#include <vector>

#include "Core/Object.h"
#include "Drawable.h"
#include "Physics/vec.h"
#include "Utils/Common.h"
#include "Utils/DebugTools.h"

class GameWindow;

class DrawSurface {
    GameWindow& m_window;

    std::vector<Ref<sf::Text>> m_texts;
    sf::Color m_clear_color { sf::Color::Black };
    sf::View m_gui_view;
    std::unordered_set<DrawablePointerWrapper> m_drawables;

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
