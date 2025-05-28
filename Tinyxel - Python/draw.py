import tinyxel

def draw(): #

    tinyxel.cls(0)

    for x in range(0, 320): #
        
        for y in range(0, 320): #
            
            if (x + y) % 25 == 0 :
                tinyxel.boxf(tinyxel.vector2d(x, y), 1, 1, round((x + y) * tinyxel.getTime()) % 16)
        #
    #
#

tinyxel.init('Draw lines', 160, 160, draw=draw, fps=60)
