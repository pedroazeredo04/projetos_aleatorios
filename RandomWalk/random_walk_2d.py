import random
from time import sleep
import pygame
import math

# Vector class
class Vector2D:
    def __init__(self, x, y):
        self.x = x
        self.y = y

    # operators overloading
    def __add__(self, other):
        return Vector2D(self.x + other.x, self.y + other.y)

    def __str__(self):
        return f"({self.x}, {self.y})"

    def __rmul__(self, num):
        return Vector2D(num*self.x, num*self.y)

    # turn Vector2D into a tuple
    def tuple(self):
        return (self.x, self.y)


# Set colors variables
background_color = (255, 255, 255)
line_color = (0, 0, 0)

# Set display/screen variables
display_speed = 0.1
screen_size = 1.5 * Vector2D(1024, 512)

walker_position = Vector2D(screen_size.x/2, screen_size.y/2)
sides = int(input("Number of sides: "))

# Initialize pygame variables
screen = pygame.display.set_mode(screen_size.tuple())
pygame.display.set_caption("Random Walk")
screen.fill(background_color)

# Create window
running = True
pause = False
while running:
    pygame.display.flip()
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

        if event.type == pygame.KEYDOWN:
            pause = True if not pause else False

    if not pause:
        # Choose direction
        line_size = 15.36  # size of the lines
        choosen_angle = random.randint(0, sides)
        angle = 2*math.pi/sides * choosen_angle
        direction = line_size * Vector2D(math.cos(angle), math.sin(angle))

        sleep(display_speed)

        # Draw line
        future_position = walker_position + direction
        pygame.draw.line(screen, line_color, walker_position.tuple(), future_position.tuple(), 4)
        walker_position = future_position
        
        if walker_position.x > screen_size.x:
            walker_position = Vector2D(0, walker_position.y)

        if walker_position.x < 0:
            walker_position = Vector2D(screen_size.x, walker_position.y)

        if walker_position.y > screen_size.y:
            walker_position = Vector2D(walker_position.x, 0)

        if walker_position.y < 0:
            walker_position = Vector2D(walker_position.x, screen_size.y)
