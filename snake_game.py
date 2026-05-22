import pygame
import random
import sys

# ── Constants ────────────────────────────────────────────────────────────────
CELL       = 20          # px per grid cell
COLS       = 30          # grid width
ROWS       = 24          # grid height
WIDTH      = COLS * CELL
HEIGHT     = ROWS * CELL + 60   # extra 60 px for the HUD
FPS        = 10

# Palette
BG         = (15,  17,  26)
GRID_LINE  = (25,  28,  40)
SNAKE_HEAD = (80,  220, 140)
SNAKE_BODY = (40,  160,  90)
FOOD_COLOR = (240,  70,  80)
TEXT_COLOR = (220, 220, 230)
HUD_BG     = (22,  25,  38)
GAMEOVER   = (240,  70,  80)
SCORE_CLR  = (80,  220, 140)

UP    = ( 0, -1)
DOWN  = ( 0,  1)
LEFT  = (-1,  0)
RIGHT = ( 1,  0)
OPPOSITES = {UP: DOWN, DOWN: UP, LEFT: RIGHT, RIGHT: LEFT}

# ── Helpers ──────────────────────────────────────────────────────────────────
def cell_rect(x, y):
    return pygame.Rect(x * CELL, y * CELL, CELL, CELL)

def random_food(snake):
    occupied = set(snake)
    while True:
        pos = (random.randint(0, COLS - 1), random.randint(0, ROWS - 1))
        if pos not in occupied:
            return pos

def draw_grid(surf):
    for x in range(0, WIDTH, CELL):
        pygame.draw.line(surf, GRID_LINE, (x, 0), (x, ROWS * CELL))
    for y in range(0, ROWS * CELL, CELL):
        pygame.draw.line(surf, GRID_LINE, (0, y), (WIDTH, y))

def draw_snake(surf, snake):
    for i, (x, y) in enumerate(snake):
        color = SNAKE_HEAD if i == 0 else SNAKE_BODY
        r = cell_rect(x, y).inflate(-2, -2)
        pygame.draw.rect(surf, color, r, border_radius=5)

def draw_food(surf, food, tick):
    x, y = food
    # subtle pulse via size
    pulse = abs((tick % 20) - 10) / 10   # 0 → 1 → 0
    inflate = int(pulse * 4)
    r = cell_rect(x, y).inflate(-4 + inflate, -4 + inflate)
    pygame.draw.ellipse(surf, FOOD_COLOR, r)

def draw_hud(surf, font, score, high, paused):
    hud_y = ROWS * CELL
    pygame.draw.rect(surf, HUD_BG, (0, hud_y, WIDTH, 60))
    pygame.draw.line(surf, GRID_LINE, (0, hud_y), (WIDTH, hud_y), 2)

    score_txt  = font.render(f"SCORE  {score:04d}", True, SCORE_CLR)
    high_txt   = font.render(f"BEST  {high:04d}",  True, TEXT_COLOR)
    hint_txt   = font.render("P = pause   R = restart   ESC = quit",
                             True, (100, 100, 120))
    if paused:
        hint_txt = font.render("⏸  PAUSED — press P to continue",
                               True, (200, 180, 80))

    surf.blit(score_txt, (16, hud_y + 8))
    surf.blit(high_txt,  (16, hud_y + 32))
    hw = high_txt.get_width()
    surf.blit(hint_txt,  (WIDTH - hint_txt.get_width() - 16, hud_y + 20))

def draw_gameover(surf, big_font, font, score):
    overlay = pygame.Surface((WIDTH, ROWS * CELL), pygame.SRCALPHA)
    overlay.fill((0, 0, 0, 160))
    surf.blit(overlay, (0, 0))

    go   = big_font.render("GAME OVER", True, GAMEOVER)
    sc   = font.render(f"Score: {score}  —  press R to restart", True, TEXT_COLOR)
    surf.blit(go, go.get_rect(center=(WIDTH // 2, ROWS * CELL // 2 - 24)))
    surf.blit(sc, sc.get_rect(center=(WIDTH // 2, ROWS * CELL // 2 + 24)))

# ── Game state ────────────────────────────────────────────────────────────────
def new_game():
    cx, cy = COLS // 2, ROWS // 2
    snake = [(cx, cy), (cx - 1, cy), (cx - 2, cy)]
    direction = RIGHT
    food = random_food(snake)
    return snake, direction, food, 0, False   # snake, dir, food, score, dead

# ── Main ──────────────────────────────────────────────────────────────────────
def main():
    pygame.init()
    screen = pygame.display.set_mode((WIDTH, HEIGHT))
    pygame.display.set_caption("🐍  Snake")
    clock  = pygame.time.Clock()

    try:
        font     = pygame.font.SysFont("Consolas", 16, bold=True)
        big_font = pygame.font.SysFont("Consolas", 48, bold=True)
    except Exception:
        font     = pygame.font.Font(None, 22)
        big_font = pygame.font.Font(None, 64)

    snake, direction, food, score, dead = new_game()
    high_score = 0
    paused     = False
    pending_dir = direction
    tick        = 0

    while True:
        # ── Events ────────────────────────────────────────────────────────────
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit(); sys.exit()

            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_ESCAPE:
                    pygame.quit(); sys.exit()

                if event.key == pygame.K_r:
                    snake, direction, food, score, dead = new_game()
                    pending_dir = direction
                    paused = False

                if event.key == pygame.K_p and not dead:
                    paused = not paused

                if not dead and not paused:
                    if event.key in (pygame.K_UP,    pygame.K_w):
                        pending_dir = UP
                    elif event.key in (pygame.K_DOWN,  pygame.K_s):
                        pending_dir = DOWN
                    elif event.key in (pygame.K_LEFT,  pygame.K_a):
                        pending_dir = LEFT
                    elif event.key in (pygame.K_RIGHT, pygame.K_d):
                        pending_dir = RIGHT

        # ── Update ────────────────────────────────────────────────────────────
        if not dead and not paused:
            # Prevent 180° reversal
            if pending_dir != OPPOSITES.get(direction):
                direction = pending_dir

            head_x, head_y = snake[0]
            dx, dy = direction
            new_head = (head_x + dx, head_y + dy)

            # Wall collision
            if not (0 <= new_head[0] < COLS and 0 <= new_head[1] < ROWS):
                dead = True
            elif new_head in snake[1:]:   # Self collision
                dead = True
            else:
                snake.insert(0, new_head)
                if new_head == food:
                    score += 10
                    high_score = max(high_score, score)
                    food = random_food(snake)
                    # Speed up slightly every 50 pts (cap at FPS*2)
                    global FPS
                else:
                    snake.pop()

            tick += 1

        # ── Draw ──────────────────────────────────────────────────────────────
        screen.fill(BG)
        draw_grid(screen)
        draw_snake(screen, snake)
        draw_food(screen, food, tick)
        draw_hud(screen, font, score, high_score, paused)
        if dead:
            draw_gameover(screen, big_font, font, score)

        pygame.display.flip()

        # Dynamic speed: faster as score climbs, clamp 10–20
        speed = min(10 + score // 50, 20)
        clock.tick(speed)


if __name__ == "__main__":
    main()
