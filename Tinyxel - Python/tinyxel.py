import graphics as grp
import sys, time, aifc
from time import sleep

## Global window variable ##
win = None
version = "1.0.0"

Q_KEY = 'q'
W_KEY = 'w'
E_KEY = 'e'
A_KEY = 'a'
S_KEY = 's'
D_KEY = 'd'
Z_KEY = 'z'
X_KEY = 'x'
C_KEY = 'c'

NUM_I = '1'
NUM_II = '2'
NUM_III = '3'
NUM_IV = '4'
NUM_V = '5'
NUM_VI = '6'
NUM_VII = '7'
NUM_VIII = '8'
NUM_IX = '9'

U_ARW = 'Up'
L_ARW = 'Left'
D_ARW = 'Down'
R_ARW = 'Right'

S_BAR = 'space'
ESC_K = 'Escape'

## Tranform class ##
class vector2d: #

    def __init__(self, x, y): #

        self.x = x
        self.y = y
    #

    ## Vector2 + other vect2 ##
    def __add__(self, other): #
        
        return vector2d(self.x + other.x, self.y + other.y)
    #

    ## Vector2 - other vect2 ##
    def __sub__(self, other): #
        
        return vector2d(self.x - other.x, self.y - other.y)
    #

    ## Vector2 * other vect2 ##
    def __mul__(self, other): #
        
        return vector2d(self.x * other.x, self.y * other.y)
    #

    ## Vector2 / other vect2 ##
    def __truediv__(self, other): #
        
        return vector2d(self.x / other.x, self.y / other.y)
    #
#

## Global variables ##
class window: #

    def __init__(self, name, width, height, icon=None): #
        
        ## Window Gatget Container (Tkinter Stuff) ##
        self.this = grp.GraphWin(name, width, height)

        ## Window size ##
        self.wdth = width
        self.hght = height

        ## Running time values ##
        self.time = 0
        self.frame_cont = 0
        self.delta_time = 0

        ## Keyboard control ##
        self.down = False

        self.is_pressed = {

            Q_KEY : False,
            W_KEY : False,
            E_KEY : False,
            A_KEY : False,
            S_KEY : False,
            D_KEY : False,
            Z_KEY : False,
            X_KEY : False,
            C_KEY : False,

            NUM_I : False,
            NUM_II : False,
            NUM_III : False,
            NUM_IV : False,
            NUM_V : False,
            NUM_VI : False,
            NUM_VII : False,
            NUM_VIII : False,
            NUM_IX : False,

            U_ARW : False,
            L_ARW : False,
            D_ARW : False,
            R_ARW : False,

            S_BAR : False,
            ESC_K : False
        }

        self.on_pressed = {

            Q_KEY : [False, False],
            W_KEY : [False, False],
            E_KEY : [False, False],
            A_KEY : [False, False],
            S_KEY : [False, False],
            D_KEY : [False, False],
            Z_KEY : [False, False],
            X_KEY : [False, False],
            C_KEY : [False, False],

            NUM_I : [False, False],
            NUM_II : [False, False],
            NUM_III : [False, False],
            NUM_IV : [False, False],
            NUM_V : [False, False],
            NUM_VI : [False, False],
            NUM_VII : [False, False],
            NUM_VIII : [False, False],
            NUM_IX : [False, False],

            U_ARW : [False, False],
            L_ARW : [False, False],
            D_ARW : [False, False],
            R_ARW : [False, False],

            S_BAR : [False, False],
            ESC_K : [False, False]
        }

        ## Default window icon ##
        if icon != None : grp.set_icon(icon)

        ## Set on_Key_Down/Up events ##
        ## From Tkinter key resource ##
        grp.set_event(key_down, key_up)
    #
#

## Resource creator ##
class resource: #

    def __init__(self, _name): #
        
        self.name = _name

        self.sprites = ""
        self.tilemap = ""

        self.sounds = ""
        self.musics = ""
    #
#

## Colors ##
def color(num): #

    ## White and Black ##
    if num == 0 : return grp.color_rgb(255, 255, 255)
    elif num == 3 : return grp.color_rgb(000, 000, 000)

    ## Gray ##
    elif num == 1 : return grp.color_rgb(212, 212, 212)
    elif num == 2 : return grp.color_rgb( 42,  42,  42)

    ## Blue ##
    elif num == 4 : return grp.color_rgb(000, 149, 213)
    elif num == 5 : return grp.color_rgb(000,  89, 128)

    ## Lavander ##
    elif num == 6 : return grp.color_rgb(191, 177, 248)
    elif num == 7 : return grp.color_rgb(128,  99, 241)

    ## Pink ##
    elif num == 8 : return grp.color_rgb(213, 000,  69)
    elif num == 9 : return grp.color_rgb(128, 000,  41)

    ## Yellow ##
    elif num == 10 : return grp.color_rgb(255, 242, 000)
    elif num == 11 : return grp.color_rgb(255, 198, 000)

    ## Green ## 
    elif num == 12 : return grp.color_rgb( 34, 179,  78)
    elif num == 13 : return grp.color_rgb( 21, 106,  47)

    ## Brown ##
    elif num == 14 : return grp.color_rgb(112,  74,  48)
    elif num == 15 : return grp.color_rgb( 61,  37,  24)

    else : grp.GraphicsError(grp.TINYXEL_COLOR)
#

## Init system ##
def init(name="Tinyxel Project", width=64, height=64, update=None, draw=None, awake=None, background=0, fps=24): #

    ## Fix Values ##
    if (width > 512 or height > 512) or (width < 64 or height < 64) : grp.GraphicsError(grp.TINYXEL_ISIZE)

    ## Init Screen ##
    global win
    win = window(name, width, height)
    win.this.setBackground(color(0))
    win._fps = fps

    win.this.setBackground(color(background))
    
    ## alternative for init function ##
    try : #

        if awake != None : awake()

        if update == None and draw == None :

            while True : #
                
                if btnp(ESC_K) : kill()
                break
            #
        #
    #
    
    except grp.GraphicsError(grp.TINYXEL_COLOR) :
        grp.GraphicsError(grp.TINYXEL_COLOR)

    except grp.GraphicsError(grp.TINYXEL_COLOR) :
        grp.GraphicsError(grp.TINYXEL_ISIZE)

    finally:
        pass

    ## Main functions ##
    while True: #

        ## Update current time ##
        win.time = time.time()

        ## Initidal delta value ##
        first_time = time.time()

        ## Sleep at frame time ##
        sleep(1/fps)

        ## Call update ##
        if update != None : update()

        ## Draw ##
        if draw != None : draw()

        win.delta_time = time.time() - first_time
        win.frame_cont += 1

        ## Exit from program ##
        if btnp(ESC_K) : kill()
    #
#

## System Stuff ##

## Exit from program
def kill(): #

    global win
    win.this.close()
    sys.exit()
#

## Clear screen ##
def cls(col): #

    global win
    while len(win.this.items) > 0 : #

        for item in win.this.items : item.undraw()
    #

    win.this.setBackground(color(col))
#

def getTime(): #

    global win
    return win.time
#

def getDelta(): #

    global win
    return win.delta_time
#

def frame_cont(): #

    global win
    return win.frame_cont
#

## Primitive objects ##

## Line ##
def line(fvect, lvect, col): #

    line = grp.Line(grp.Point(fvect.x, fvect.y), grp.Point(lvect.x, lvect.y))
    line.setFill(color(col))

    global win
    line.draw(win.this)
#

## Box ##
def box(vect2, wdt, hgt, col): #

    ## Create a rectangle from graphics.py ##
    _box = grp.Rectangle(grp.Point(vect2.x, vect2.y), grp.Point(vect2.x + wdt, vect2.y + hgt))
    _box.setOutline(color(col))

    ## Draw it ##
    global win
    _box.draw(win.this)
#

## Full Box ##
def boxf(vect2, wdt, hgt, col): #

    ## Create a rectangle from graphics.py ##
    _box = grp.Rectangle(grp.Point(vect2.x, vect2.y), grp.Point(vect2.x + wdt, vect2.y + hgt))

    ## Fill with color inside ##
    _box.setOutline(color(col))
    _box.setFill(color(col))
    
    ## Draw it ##
    global win
    _box.draw(win.this)
#

## Circle ##
def cir(vect2, ray, col): #

    ## Create a circle from graphics.py ##
    _cir = grp.Circle(grp.Point(vect2.x, vect2.y), ray)
    _cir.setOutline(color(col))

    ## Draw it ##
    global win
    _cir.draw(win.this)
#

## Full Circle ##
def cirf(vect2, ray, col): #

    ## Create a circle from graphics.py ##
    _cir = grp.Circle(grp.Point(vect2.x, vect2.y), ray)

    ## Fill with color inside ##
    _cir.setOutline(color(col))
    _cir.setFill(color(col))

    ## Draw it ##
    global win
    _cir.draw(win.this)
#

## Resources ##

## Entry ##

## Called while key ##
## down but is True ##
## once.            ##
def key_down(key): #

    ckey = key.keysym

    global win
    win.is_pressed[ckey] = True

    ## First press ##
    try : #

        if not win.on_pressed[ckey][0] : #
            
            win.on_pressed[ckey][0] = True
            win.on_pressed[ckey][1] = True
        #
    #

    except :
        pass
#

## Reset the key_down ##
## return when key is ##
## released.          ##
def key_up(key): #

    lkey = key.keysym

    global win
    try : #
        
        win.is_pressed[lkey] = False
        win.on_pressed[lkey][0] = False
    #

    except :
        pass
#

## While button is pressed ##
def btn(key): #

    global win
    return win.is_pressed[key]
#

## Once when button pressed ##
def btnp(key): #

    global win
    if win.on_pressed[key][1] : #
        
        win.on_pressed[key][1] = False
        return True
    #

    else : return False
#

## Other Classes ##

## Print Text ##
def text(vect2, content, col): #

    global win

    #grp.Point(max(vect2.x + (((len(content) - 1) / 2) * 12) - 24, 0), vect2.y + 11)
    ## Create a text from graphics.py ##
    _text = grp.Text(grp.Point(vect2.x, vect2.y), content)

    ## Set the given color ##
    _text.setTextColor(color(col))
    _text.setFill(color(col))

    ## Draw it ##
    _text.draw(win.this)
#