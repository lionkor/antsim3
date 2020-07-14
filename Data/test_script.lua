ax, ay = 0, 0
bx, by = 0, 0
is_pressed = false
_do = false

function update()
    x, y = Entity.position()
    if math.abs(x - bx) > 1 and math.abs(y - by) > 1 then
        Entity.move_by((bx - ax) * 0.1, (by - ay) * 0.1) -- normalize and multiply
    end
end

function on_mouse_down(mb, x, y)
    if mb == MouseButton.LMB then
        is_pressed = true
        ax, ay = x, y
    end
end

function on_mouse_up(mb, x, y)
    is_pressed = false
end

function on_mouse_move(x, y) 
    if is_pressed then
        bx, by = x, y
    end
end





