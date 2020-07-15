ax = 0
ay = 0
bx = 0
by = 0
first = true

speed = 0.5

function update()
    ax, ay = Entity.position()
    dirx = bx - ax
    diry = by - ay
    if (dirx == 0 and not (diry == 0)) or (not (dirx == 0) and diry == 0) or not (dirx == 0 and diry == 0)then
        dirx, diry = Vec.normalize(dirx, diry)
        dirx = dirx * speed
        diry = diry * speed
        Entity.move_by(dirx, diry)
    end
end

function on_mouse_down(mb, x, y)
    if mb == MouseButton.LMB then
        bx, by = x, y
    end
end

function on_mouse_up(mb, x, y)
end
