function update()
    Entity.move_by(1, 2)
    local x, y = Entity.position()
    local r = Entity.rotation()
    Engine.log_info(x .. ", " .. y .. " @ " .. r)
end

function on_mouse_down(mb, x, y)
    local name = "unknown"
    if mb == MouseButton.LMB then
        name = "lmb"
    elseif mb == MouseButton.RMB then
        name = "rmb"
    elseif mb == MouseButton.MMB then
        name = "mmb"
    end
    Engine.log_info(name .. " was pressed!")
end
