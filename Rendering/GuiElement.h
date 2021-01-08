#ifndef GUIELEMENT_H
#define GUIELEMENT_H

#include "Core/Object.h"
#include "Draw.h"
#include "Physics/vec.h"

class GuiElement
    : public Object
{
    OBJNAME(GuiElement)
private:
    sf::Text m_text_obj;
    class Application& m_app;
    sf::Font m_font;

public:
    GuiElement(Application& app, const vecu& screen_pos, const vecd& scale, const std::string& text = "", sf::Color color = sf::Color::White);
    GuiElement(const GuiElement&) = delete;

    virtual void update(float) final;
    virtual void draw(DrawSurface&) final;

    void set_text(const std::string& text) { m_text_obj.setString(text); }

    std::function<void(float)> on_update { nullptr };
    std::function<void(DrawSurface&)> on_draw { nullptr };
};

#endif // GUIELEMENT_H
