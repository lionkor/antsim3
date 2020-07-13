function update()
    local x, y = Entity.position()
    Engine.log_info(x .. ", " .. y .. GGG)
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
