import tinyxel

pos = tinyxel.vector2d(120, 120)
col = 1

def update(): #

    global pos
    global col

    if tinyxel.btn(tinyxel.A_KEY) : pos -= tinyxel.vector2d(2, 0)
    if tinyxel.btn(tinyxel.D_KEY) : pos += tinyxel.vector2d(2, 0)
    if tinyxel.btn(tinyxel.W_KEY) : pos -= tinyxel.vector2d(0, 2)
    if tinyxel.btn(tinyxel.S_KEY) : pos += tinyxel.vector2d(0, 2)

    #col = tinyxel.frame_cont() % 16

    if tinyxel.btnp(tinyxel.Q_KEY) : #

        col -= 1
        if col < 0 : col = 15

        print(col, '<')
    #

    if tinyxel.btnp(tinyxel.E_KEY) : #

        col += 1
        if col > 15 : col = 0

        print(col, '>')
    #
#

def draw(): #

    tinyxel.cls(0)

    global pos
    global col
    tinyxel.boxf(pos, 8, 8, col)
#

tinyxel.init('Move box', 240, 240, update, draw, fps = 30)