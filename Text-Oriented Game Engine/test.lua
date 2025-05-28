require("toge")

local x = 0
local y = 20

function toge.update()

    x = math.floor((math.sin(os.time(sec)) * 16) + 16)
end

function toge.draw(dr)

    dr.set('x', x, y)
end

toge.init(32, 32)