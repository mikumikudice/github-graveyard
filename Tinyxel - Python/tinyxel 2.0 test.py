import tinyxel

def awake(): #

    for y in range(16) : #

        for x in range(16) : tinyxel.boxf(tinyxel.vector2d(x * 20, y * 20), 20, 20, (x + y) % 16)
    #

    tinyxel.sleep(10)
    tinyxel.kill()
#

tinyxel.init('Test', 320, 320, awake = awake, fps = 60)