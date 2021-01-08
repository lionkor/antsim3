#include "GuiElement.h"
#include "Core/Application.h"

GuiElement::GuiElement(Application& app, const vecu& screen_pos, const vecd& scale, const std::string& text, sf::Color color)
    : m_app(app) {
    m_text_obj.setPosition(screen_pos.x, screen_pos.y);
    m_text_obj.setScale(scale.x, scale.y);
    m_text_obj.setFillColor(color);
    m_text_obj.setString(text);
    auto result = m_app.resource_manager().get_resource_by_name("mono.ttf");
    if (result.error()) {
        report_error(result.message());
        return;
    }
    auto& file = result.value().get();
    if (!file.is_valid()) {
        report_error(file.validation_error_message());
        return;
    }
    auto* data_ptr = file.load();
    auto loaded = m_font.loadFromMemory(data_ptr->data(), data_ptr->size());
    if (!loaded) {
        report_error("failed loading mono.ttf as font!");
        return;
    }
    report("loaded font mono.ttf");
    m_text_obj.setFont(m_font);
}

void GuiElement::update() {

    if (on_update)
        on_update();
}

void GuiElement::draw(DrawSurface& surface) {
    surface.draw_text(m_text_obj);

    if (on_draw)
        on_draw(surface);
}
