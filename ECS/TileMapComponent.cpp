#include "TileMapComponent.h"

#include "Rendering/TextureAtlas.h"
#include "Utils/ResourceManager.h"
#include "Entity.h"

TileMapComponent::TileMapComponent(Entity& e, vec<size_t> grid_size, double tile_size, const std::string& texture_atlas_name, size_t subtexture_size)
    : Component(e)
    , m_atlas(make_shared<TextureAtlas>(resource_manager().load_texture(texture_atlas_name), subtexture_size))
    , m_grid(grid_size, tile_size, m_atlas) {
    m_grid.set_tile_texture({ 2, 2 }, { 1, 0 });
}

void TileMapComponent::on_update(float) {
    m_grid.set_position(parent().transform().position());
}

void TileMapComponent::on_draw(DrawSurface& surface) {
    surface.draw(m_grid);
}
