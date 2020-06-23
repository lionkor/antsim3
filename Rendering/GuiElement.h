// Copyright (c) Lion Kortlepel 2020
// This software is free software and licensed under GPL-3.0.
// You should have received a copy of the GNU General Public License along 
// with this program. If not, see <https://www.gnu.org/licenses/>.

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
    sf::Text           m_text_obj;
    class Application& m_app;
    sf::Font           m_font;

public:
    GuiElement(Application& app, const vecu& screen_pos, const vecd& scale, const std::string& text = "", sf::Color color = sf::Color::White);

    virtual void update() final;
    virtual void draw(DrawSurface&) final;
    
    void set_text(const std::string& text) { m_text_obj.setString(text); }

    std::function<void()>             on_update { nullptr };
    std::function<void(DrawSurface&)> on_draw { nullptr };
};

#endif // GUIELEMENT_H
