function update()
    local x, y = Entity.position()
    local r = Entity.rotation()
end

function on_mouse_down(mb, x, y)
    if mb == MouseButton.LMB then
        Entity.set_position(x, y)
    end
end
