import tinyxel
import random

dots = [tinyxel.vector2d(80, 80)]
dirc = tinyxel.vector2d(-8, 0)

food = tinyxel.vector2d(random.randrange(0, 240), random.randrange(0, 240, 8))
food.x = food.x - (food.x % 8)
food.y = food.y - (food.y % 8)

end = False
enable = False

def update(): #

    global dots, dirc, food, enable

    if tinyxel.btnp(tinyxel.S_BAR) : enable = True

    if tinyxel.btnp(tinyxel.W_KEY) : dirc = tinyxel.vector2d(0, -8)
    if tinyxel.btnp(tinyxel.A_KEY) : dirc = tinyxel.vector2d(-8, 0)
    if tinyxel.btnp(tinyxel.S_KEY) : dirc = tinyxel.vector2d(0, +8)
    if tinyxel.btnp(tinyxel.D_KEY) : dirc = tinyxel.vector2d(+8, 0)

    if enable : #

        dots.insert(0, dots[0] + dirc)
        dots.pop(len(dots) - 1)
    #

    if dots[0].x == food.x and dots[0].y == food.y : coll()

    for dot in range(2, len(dots)) : #
        
        if dots[0].x == dots[dot].x and dots[0].y == dots[dot].y :
            if not end : kill()
    #

    if dots[0].x > 240 or dots[0].x < 0 or dots[0].y > 240 or dots[0].y < 0 : kill()
#

def draw(): #

    global end

    if not end : #

        tinyxel.cls(12)

        global dots, enable

        if enable : #

            for dot in range(0, len(dots)) : #

                if dot == 0 : tinyxel.boxf(dots[dot], 8, 8, 0)
                else : tinyxel.boxf(dots[dot], 8, 8, 1)
            #
        #

        if enable : tinyxel.boxf(food, 8, 8, 15)

        if enable : #

            tinyxel.text(tinyxel.vector2d(30, 12), "Score: ", 0)
            tinyxel.text(tinyxel.vector2d(60, 12), str(len(dots) - 1), 0)
        #

        else : tinyxel.text(tinyxel.vector2d(120, 120), "Press Space", round(tinyxel.frame_cont()) % 16)
    #
#

def coll(): #

    global dots, food

    dots.insert(len(dots), tinyxel.vector2d(dots[len(dots) - 1].x, dots[len(dots) - 1].y))

    food = tinyxel.vector2d(random.randrange(0, 240, 8), random.randrange(0, 240, 4))
    food.x = food.x - (food.x % 8)
    food.y = food.y - (food.y % 8)
#

def kill(): #

    global end, dots
    end = True

    tinyxel.cls(8)
    tinyxel.text(tinyxel.vector2d(120, 110), "You lose!", 0)
    tinyxel.text(tinyxel.vector2d(120, 130), "score: " + str(len(dots) - 1), 0)

    tinyxel.sleep(5)
    tinyxel.kill()
#

tinyxel.init("Snake!", 240, 240, update, draw, background=12, fps=12)