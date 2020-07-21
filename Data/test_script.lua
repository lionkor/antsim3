ax = 0
ay = 0
bx = 0
by = 0
move = false

speed = 0.5

function update()
    if not move then
        return
    end
    ax, ay = Entity.position()
    local dirx = bx - ax
    local diry = by - ay
    if Vec.length(dirx, diry) ~= 0 then
        dirx, diry = Vec.normalize(dirx, diry)
        dirx = dirx * speed
        diry = diry * speed
        Entity.move_by(dirx, diry)
    end
end

function on_mouse_move(x, y)
    bx = x
    by = y
end

function on_mouse_down(mb, x, y)
    if mb == MouseButton.LMB then
        move = true
    end
end

function on_mouse_up(mb, x, y)
    if mb == MouseButton.LMB then
        move = false
    end
end
