import tinyxel
from tinyxel import vector2d

def awake(): #

    tinyxel.text(vector2d(150, 24), "Tinyxel Colors", 0)
    tinyxel.line(vector2d(12, 12), vector2d(288, 12), 0)
    tinyxel.line(vector2d(12, 35), vector2d(288, 35), 0)

    ## Line One ##
    tinyxel.boxf(vector2d(12, 48), 24, 24, 0)
    tinyxel.boxf(vector2d(48, 48), 24, 24, 1)

    tinyxel.boxf(vector2d(84, 48), 24, 24, 2)
    tinyxel.boxf(vector2d(120, 48), 24, 24, 3)

    tinyxel.boxf(vector2d(156, 48), 24, 24, 4)
    tinyxel.boxf(vector2d(192, 48), 24, 24, 5)

    tinyxel.boxf(vector2d(228, 48), 24, 24, 6)
    tinyxel.boxf(vector2d(264, 48), 24, 24, 7)

    ## Line Two ##
    tinyxel.boxf(vector2d(12, 84), 24, 24, 8)
    tinyxel.boxf(vector2d(48, 84), 24, 24, 9)

    tinyxel.boxf(vector2d(84, 84), 24, 24, 10)
    tinyxel.boxf(vector2d(120, 84), 24, 24, 11)

    tinyxel.boxf(vector2d(156, 84), 24, 24, 12)
    tinyxel.boxf(vector2d(192, 84), 24, 24, 13)

    tinyxel.boxf(vector2d(228, 84), 24, 24, 14)
    tinyxel.boxf(vector2d(264, 84), 24, 24, 15)

    ## Numbers ##
    tinyxel.text(vector2d(24, 60), "00", 3)
    tinyxel.text(vector2d(60, 60), "01", 3)

    tinyxel.text(vector2d(96, 60), "02", 3)
    tinyxel.text(vector2d(132, 60), "03", 0)

    tinyxel.text(vector2d(168, 60), "04", 3)
    tinyxel.text(vector2d(204, 60), "05", 3)

    tinyxel.text(vector2d(240, 60), "06", 3)
    tinyxel.text(vector2d(276, 60), "07", 3)

    tinyxel.text(vector2d(24, 96), "08", 3)
    tinyxel.text(vector2d(60, 96), "09", 3)

    tinyxel.text(vector2d(96, 96), "10", 3)
    tinyxel.text(vector2d(132, 96), "11", 3)

    tinyxel.text(vector2d(168, 96), "12", 3)
    tinyxel.text(vector2d(204, 96), "13", 3)

    tinyxel.text(vector2d(240, 96), "14", 3)
    tinyxel.text(vector2d(276, 96), "15", 3)

    tinyxel.sleep(9) # run for 10 seconds
    tinyxel.kill()
#

tinyxel.init("Tinyxel colors", 300, 122, awake=awake, background=3)