#ifndef GRIDCOMPONENT_H
#define GRIDCOMPONENT_H

#include "Component.h"

class TileMapComponent : public Component
{
    OBJNAME(TileMapComponent)
private:
    SharedPtr<TextureAtlas> m_atlas;
    TileMap m_grid;

public:
    TileMapComponent(Entity& e, vec<size_t> grid_size, double tile_size, const std::string& texture_atlas_name, size_t subtexture_size);

    virtual void on_update(float dt) override;
    virtual void on_draw(DrawSurface& surface) override;

    TileMap& tilemap() { return m_grid; }
    const TileMap& tilemap() const { return m_grid; }
};

#endif // GRIDCOMPONENT_H
