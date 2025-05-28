require("SystemLib")

toge = {}
dr = {}

local grid = {}
local time = 0

local swt, sht

function vector2(x, y)

    local vect = {}
    
    vect.x = x
    vect.y = y

    return vect
end

function toge.init(wdt, hgt)

    for y = 0, hgt do
    
        grid[y] = string.rep('.', wdt)
    end

    swt = wdt
    sht = hgt

    while true do
        
        if os.time(sec) > time + 0.0416 then
        
            if toge.update ~= nil then toge.update() end
            
            time = os.time(sec)
        end

        if toge.update ~= nil then

            toge.draw(dr)
            
            for y in pairs(grid) do

                if y == 0 then
                    
                    io.write(('\b'):rep((swt * sht) + 3))
                    io.write(('='):rep(swt) .. '\n')
                    io.write(grid[y]:sub(1, swt) .. '\n')
                    io.flush()

                elseif y == sht then

                    io.write(('='):rep(swt) .. '\n')
                    io.write(grid[y]:sub(1, swt) .. '\n')

                else io.write(grid[y]:sub(1, swt) .. '\n') end

                grid[y] = string.rep('.', swt)
            end
        end
    end
end

function dr.set(char, x, y)

    grid[y] = grid[y]:sub(1, x - 2) .. char .. grid[y]:sub(x)
end

