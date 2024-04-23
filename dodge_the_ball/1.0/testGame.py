import pygame, random
pygame.init()

# Setup
pygame.display.set_caption('Test Game')

size = width, height = 1600, 900
screenColor = 0, 0, 0

oriPosC = [width / 2, height, 50, 50]
oriPosO = [width / 2, height / 2, 90, 90]

screen = pygame.display.set_mode(size)

# Config 'objects'
char = pygame.image.load('./Images/square.png')
charrect = char.get_rect()
charrect.update(oriPosC)

obs = pygame.image.load('./Images/spikeball.png')
obsRect = obs.get_rect()
obsRect.update(oriPosO)

col = pygame.image.load('./Images/collectibleSmall.png')
def genCol():
    coords = x, y = random.randint(50, width - 50), random.randint(50, height - 50)
    colRect = pygame.draw.rect(screen, (0, 0, 0), (coords, (30, 30)))
    return colRect

colRect = genCol()
point = 0

def colGet():
    global colRect, point
    colRect = genCol()
    point += 1

speed = [1, 2]
speedModCycle = 1

def reset():
    global speedModCycle, speed
    charrect.update(oriPosC)
    obsRect.update(oriPosO)
    speedModCycle = 1
    speed = [1, 2]

def charMove():
    charrect.move_ip(0, 1)
    
    keys = pygame.key.get_pressed()
    
    if keys[pygame.K_SPACE]: charrect.update(char.get_rect())
    
    if keys[pygame.K_a]: charrect.move_ip(-1, 0)
    if keys[pygame.K_d]: charrect.move_ip(1, 0)
    if keys[pygame.K_w]:
        if keys[pygame.K_s]:
            charrect.move_ip(0, -1)
        else: charrect.move_ip(0, -2)
          
def obsMove():
    global speedModCycle
    if speedModCycle % 750 == 0:
        while True:
            speed[0] += random.randint(-2, 2)
            speed[1] += random.randint(-2, 2)
            if speed[0] != 0 and speed[1] != 0 and speed[0] != speed[1]:
                break
        speedModCycle = 1
        
    if obsRect.left <= 0 or obsRect.right >= width:
        speed[0] = -speed[0]
    if obsRect.top <= 0 or obsRect.bottom >= height:
        speed[1] = -speed[1]
        
        
    obsRect.move_ip(speed)
    speedModCycle += 1
    
font = pygame.font.Font(pygame.font.get_default_font(), 40)
font.bold = True
    
# Main
while True:
    text = font.render('Point: ' + str(point), True, (255, 255, 255), screenColor)
    textRect = text.get_rect()
    textRect.center = (110, 40)
    
    screen.fill(screenColor)
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            pygame.quit()
    
    charMove()
    obsMove()
    
    if pygame.key.get_pressed()[pygame.K_SPACE]:
        reset()
        pygame.time.wait(300)
    
    if charrect.colliderect(obsRect):
        pygame.time.wait(1000)
        point = 0
        reset()
    
    if charrect.colliderect(colRect):
        colGet()
        
    charrect.clamp_ip(screen.get_rect())
    obsRect.clamp_ip(screen.get_rect())
    
    screen.blit(char, charrect)
    screen.blit(obs, obsRect)
    screen.blit(col, colRect)
    screen.blit(text, textRect)
    
    
    pygame.display.flip()