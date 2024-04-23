import pygame as pg, random, sys
from pygame.math import Vector2

pg.init()

# Setup
pg.display.set_caption('Test Game v1.2')
size = width, height = 1600, 900
screen = pg.display.set_mode(size)
screenBG = pg.image.load('wall.jpg')
screenBlit = screen.blit
screenRect = screen.get_rect()
screenColor = (0, 0, 0)
useFont = pg.font.get_default_font()

objList = []

point = 0

class Player:
    def __init__(self, pos):
        self.fly = pg.image.load('charFly.png')
        self.run = pg.image.load('charRun.png')
        self.stand = pg.image.load('charStand.png')
        self.image = self.run
        self.originalPos = pos
        self.rect = self.image.get_rect()
        self.rect.update(self.originalPos)
        self.side = 'l'
        self.state = 's'
        
    def move(self):
        keys = pg.key.get_pressed()
        
        if self.rect.bottom < height: self.state = 'f'
        else:
            if keys[pg.K_a] or keys[pg.K_d]: self.state = 'r'
            else: self.state = 's'
        
        if self.state == 'r':
            self.image = self.run
        elif self.state == 'f':
            self.image = self.fly
        elif self.state == 's':
            self.image = self.stand
        
        self.rect.move_ip(0, 1)
        
        if keys[pg.K_a]:
            if self.side == 'r':
                self.run = pg.transform.flip(self.run, True, False)
                self.fly = pg.transform.flip(self.fly, True, False)
                self.stand = pg.transform.flip(self.stand, True, False)
                self.side = 'l'
            if keys[pg.K_w] and keys[pg.K_s]:
                self.rect.move_ip(-3, 0)
            elif self.state != 'r': self.rect.move_ip(-1, 0)
            else: self.rect.move_ip(-2, 0)
        if keys[pg.K_d]:
            if self.side == 'l': 
                self.run = pg.transform.flip(self.run, True, False)
                self.fly = pg.transform.flip(self.fly, True, False)
                self.stand = pg.transform.flip(self.stand, True, False)
                self.side = 'r'
            if keys[pg.K_w] and keys[pg.K_s]:
                self.rect.move_ip(3, 0)
            elif self.state != 'r': self.rect.move_ip(1, 0)
            else: self.rect.move_ip(2, 0)
        if keys[pg.K_w]:
            if keys[pg.K_s]:
                self.rect.move_ip(0, -1)
            else: self.rect.move_ip(0, -2)
            
    def resetPos(self):
        self.rect.update(self.originalPos)
        
class Obstacle:
    def __init__(self, texture):
        self.image = pg.image.load(texture)
        self.rect = self.image.get_rect()
        # self.rect.inflate_ip(-20, -20)
        self.speed = [0, 0]
        self.moveCycle = 0
        
    def rescaleRect(self, inflateCoords: tuple):
        x, y = inflateCoords
        self.rect.inflate_ip(inflateCoords)
        self.rect.topleft += Vector2(-x, -y)
        
    def setInitSpeed(self):
        x, y = 0, 0
        while x == 0 or y == 0:
            x = random.randint(-1, 1)
            y = random.randint(-1, 1)
        return [x, y]
    
    def setInitPos(self):
        self.size = self.image.get_size()
        return [width / random.randint(1, 8), height / random.randint(2, 8), self.rect.right - self.rect.left, self.rect.bottom - self.rect.top]
    '''
    def update(self):
        self.rect.update(self.setInitPos())
        self.speed = self.setInitSpeed()
    '''  
    def move(self):
        if self.moveCycle % 900 == 0:
            while True:
                self.speed[0] += random.randint(-1, 1)
                self.speed[1] += random.randint(-1, 1)
                if self.speed[0] > 6 or self.speed[0] < -6: self.speed[0] /= 2
                if self.speed[1] > 6 or self.speed[1] < -6: self.speed[1] /= 2
                
                if self.speed[0] != 0 and self.speed[1] != 0 and self.speed[0] != self.speed[1]:
                    break
            self.moveCycle = 1
            
        if self.rect.left <= 0 or self.rect.right >= width:
            self.speed[0] = -self.speed[0]
        if self.rect.top <= 0 or self.rect.bottom >= height:
            self.speed[1] = -self.speed[1]
            
        self.rect.move_ip(self.speed)
        self.moveCycle += 1
        
    def reset(self):
        self.rect.update(self.setInitPos())
        self.speed = self.setInitSpeed()
        self.moveCycle = 0

class Collectible:
    def __init__(self, texture):
        self.image = pg.image.load(texture)
        self.rect = self.image.get_rect()
        
    def setPos(self):
        coords = x, y = random.randint(50, width - 50), random.randint(50, height - 50)
        self.rect = pg.draw.rect(screen, screenColor, (coords, (30, 30)))
        
    def collected(self):
        global point
        point += 1
        self.setPos()
        
class Text:
    def __init__(self, text: str, fontFile: str, size: int, color, bold: bool = False, italian: bool = False):
        self.font = pg.font.SysFont(fontFile, size, bold, italian)
        self.text = self.font.render(text, True, color)
        self.rect = self.text.get_rect()
        
    def setCenter(self, pos):
        self.rect.center = pos
    
    def setPos(self, left: int, top: int):
        self.rect.left = left
        self.rect.top = top

# Initiate main Objects
char1 = Player([width / 2, height, 50, 50])
obList = []
ob1 = Obstacle('obstacle.png')
# ob1.rescaleRect((16, -9))
col1 = Collectible('collectibleSmall.png')
col1.setPos()

textP = Text('Point: ' + str(point), useFont, 40, (0, 255, 0), True)
textP.setCenter((100, 50))

objList.append(char1)
objList.append(ob1)
objList.append(col1)

# Refresh canvas
def refresh():
    screen.fill(screenColor)
    screenBlit(char1.image, char1.rect)
    screenBlit(ob1.image, ob1.rect)
    screenBlit(col1.image, col1.rect)
    pg.display.flip()

# Hitbox
hitbox = False
def showHitbox(show: bool):
    global hitbox
    if show:
        for s in objList:
            pg.draw.rect(screen, (255, 255, 0), s.rect, 2)
    else:
        refresh()

# Pause
pause = False

# Menu
showInstruct = True
menuText = []
menuText.append(Text('A, W, D: move', useFont, 40, (255, 255, 255)))
menuText.append(Text('W + S: hover -> faster horizontal movement', useFont, 40, (255, 255, 255)))
menuText.append(Text('F5: new game', useFont, 40, (255, 255, 255)))
menuText.append(Text('Press SPACE to continue', useFont, 25, (255, 255, 255)))
menuText.append(Text('ESCAPE: pause', useFont, 40, (255, 255, 255)))

menuText[0].setPos(width / 4, 200)
menuText[1].setPos(width / 4, 250)
menuText[2].setPos(width / 4, 300)
menuText[3].setCenter((width / 2, 700))
menuText[4].setPos(width / 4, 350)

pauseText = Text('PAUSED', useFont, 40, (255, 255, 255))
pauseText.setCenter((width / 2, 550))

def showInstructMenu():
    for t in menuText:
        screenBlit(t.text, t.rect)
    if pause:
        screenBlit(pauseText.text, pauseText.rect)
    
def resetGame():
    char1.resetPos()
    ob1.reset()
    col1.setPos()
    pg.time.wait(200)

# main loop
while True:
    # Screen bg
    # screen.fill(screenColor)
    screenBlit(screenBG, screenRect)
    
    # exit game
    for e in pg.event.get():
        if e.type == pg.QUIT:
            pg.quit()
            sys.exit()
            
    # Menu
    if showInstruct:
        showInstructMenu()
        if pg.key.get_pressed()[pg.K_SPACE]:
            showInstruct = False
            pause = False
    else:
        if pg.key.get_pressed()[pg.K_F3]: hitbox = not hitbox
        if hitbox:
            showHitbox(True)
        
        # Manual reset
        if pg.key.get_pressed()[pg.K_F5]:
            point = 0
            resetGame()
            
        # score
        textP = Text('Point: ' + str(point), useFont, 40, (0, 255, 0), True)
        textP.setCenter((100, 50))

        if pause == False:
            char1.move()
            ob1.move()
    
        if char1.rect.colliderect(ob1.rect):
            pg.time.wait(1000)
            point = 0
            resetGame()
    
        if char1.rect.colliderect(col1.rect):
            col1.collected()
    
        char1.rect.clamp_ip(screenRect)
        ob1.rect.clamp_ip(screenRect)
    
        screenBlit(char1.image, char1.rect)
        screenBlit(ob1.image, ob1.rect)
        screenBlit(col1.image, col1.rect)
        screenBlit(textP.text, textP.rect)
        
        # pause
        if pg.key.get_pressed()[pg.K_ESCAPE]:
            showInstruct = True
            pause = True
    
        del textP
        
    pg.display.flip()
    '''
    if pg.key.get_pressed()[pg.K_ESCAPE]:
        pause = True
    pg.event.pump()
    while pause:
        print(pause)
        pg.time.wait(500)
        print(pause)
        if pg.key.get_pressed()[pg.K_ESCAPE]: pause = False
        print(pause)
    '''