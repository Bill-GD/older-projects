import pygame as pg, random
pg.init()

# Setup
pg.display.set_caption('Test Game v1.1')
size = width, height = 1600, 900
screen = pg.display.set_mode(size)
screenRect = screen.get_rect()
screenColor = 0, 0, 0

point = 0

class Player:
    def __init__(self, texture, posList):
        self.image = pg.image.load(texture)
        self.originalPos = posList
        self.rect = self.image.get_rect()
        self.rect.update(self.originalPos)
        
    def move(self):
        self.rect.move_ip(0, 1)
        
        keys = pg.key.get_pressed()
        
        if keys[pg.K_a]:
            if keys[pg.K_w] and keys[pg.K_s]:
                self.rect.move_ip(-2, 0)
            else: self.rect.move_ip(-1, 0)
        if keys[pg.K_d]:
            if keys[pg.K_w] and keys[pg.K_s]:
                self.rect.move_ip(2, 0)
            else: self.rect.move_ip(1, 0)
        if keys[pg.K_w]:
            if keys[pg.K_s]:
                self.rect.move_ip(0, -1)
            else: self.rect.move_ip(0, -2)
            
    def resetPos(self):
        self.rect.update(self.originalPos)
        
class Obstacle:
    def setInitSpeed(self):
        x, y = 0, 0
        while x == 0 or y == 0:
            x = random.randint(-1, 1)
            y = random.randint(-1, 1)
        return [x, y]
    
    def setInitPos(self):
        return [width / random.randint(1, 8), height / random.randint(2, 8), 90, 90]
    
    def __init__(self, texture):
        self.image = pg.image.load(texture)
        self.rect = self.image.get_rect()
        self.rect.update(self.setInitPos())
        self.moveCycle = 0
        self.speed = self.setInitSpeed()
        
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
        self.rect = pg.draw.rect(screen, (0, 0, 0), (coords, (30, 30)))
        
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

char1 = Player('square.png', [width / 2, height, 50, 50])
obList = []
ob1 = Obstacle('spikeball.png')
col1 = Collectible('collectibleSmall.png')
col1.setPos()
    
def resetGame():
    char1.resetPos()
    ob1.reset()
    col1.setPos()

# main loop
while True:
    # exit game
    for e in pg.event.get():
        if e.type == pg.QUIT:
            pg.quit()
           
    # score
    textP = Text('Point: ' + str(point), 'Consolas.ttf', 40, (0, 255, 0), True)
    textP.setCenter((100, 50))

    screen.fill(screenColor)
    
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
    
    screen.blit(char1.image, char1.rect)
    screen.blit(ob1.image, ob1.rect)
    screen.blit(col1.image, col1.rect)
    screen.blit(textP.text, textP.rect)
    
    pg.display.flip()
    
    del textP