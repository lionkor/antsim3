function update()
end

function on_mouse_down(mb, x, y)
    if mb == MouseButton.LMB then
        Engine.log_info("lmb pressed!")
        Engine.log_warning("g_parent is " .. g_parent)
    end
end
