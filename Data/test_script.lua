i = 1

function update()
end

function on_mouse_down(mb, x, y)
    if mb == 0 then
        Engine.log_info(x * y)
    else
        Engine.log_info("wrong button >:(")
    end
end
