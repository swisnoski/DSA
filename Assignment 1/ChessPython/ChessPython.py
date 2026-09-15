# Sam Wisnoski
# 12/7/2023
# Passionate Pursuit Final Project: Chess

import pygame 


# initialize function and create game loop 
pygame.init()
width = 800
height = 720
screen = pygame.display.set_mode([width, height])
pygame.display.set_caption('Funny Chess')
font = pygame.font.Font('freesansbold.ttf',20)
big_font = pygame.font.Font('freesansbold.ttf', 40)
timer = pygame.time.Clock()
fps = 60

# create game variables and images 
white_pieces = ['rook', 'knight', 'bishop','king','queen', 'bishop','knight','rook',
                'pawn','pawn','pawn','pawn','pawn','pawn','pawn','pawn']
white_locations = [(0,0),(1,0),(2,0),(3,0),(4,0),(5,0),(6,0),(7,0),
                   (0,1),(1,1),(2,1),(3,1),(4,1),(5,1),(6,1),(7,1)]
black_pieces = ['rook', 'knight', 'bishop','king','queen', 'bishop','knight','rook',
                'pawn','pawn','pawn','pawn','pawn','pawn','pawn','pawn']
black_locations = [(0,7),(1,7),(2,7),(3,7),(4,7),(5,7),(6,7),(7,7),
                   (0,6),(1,6),(2,6),(3,6),(4,6),(5,6),(6,6),(7,6)]
captured_pieces_white = []
captured_pieces_black = []

# 0 - whites turn, no selection, 1 - whites turn piece selected: 2 - black turn no selection, 3 - black turn piece selecter
turn_step = 0
selection = 100
valid_moves = []

#load in game piece images
black_queen = pygame.image.load("C:/Users\swisnoski\OneDrive - Olin College of Engineering/2026_02 Fall\Data Structures + Algorithms\Assignment 1\ChessPython\images\kQ.png")
black_queen = pygame.transform.scale(black_queen, (64,64))
black_queen_small = pygame.transform.scale(black_queen, (36,36))
black_king = pygame.image.load("C:/Users\swisnoski\OneDrive - Olin College of Engineering/2026_02 Fall\Data Structures + Algorithms\Assignment 1\ChessPython\images\kK.png")
black_king = pygame.transform.scale(black_king, (64,64))
black_king_small = pygame.transform.scale(black_king, (36,36))
black_rook = pygame.image.load("C:/Users\swisnoski\OneDrive - Olin College of Engineering/2026_02 Fall\Data Structures + Algorithms\Assignment 1\ChessPython\images\kR.png")
black_rook = pygame.transform.scale(black_rook, (64,64))
black_rook_small = pygame.transform.scale(black_rook, (36,36))
black_bishop = pygame.image.load("C:/Users\swisnoski\OneDrive - Olin College of Engineering/2026_02 Fall\Data Structures + Algorithms\Assignment 1\ChessPython\images\kB.png")
black_bishop = pygame.transform.scale(black_bishop, (64,64))
black_bishop_small = pygame.transform.scale(black_bishop, (36,36))
black_knight = pygame.image.load("C:/Users\swisnoski\OneDrive - Olin College of Engineering/2026_02 Fall\Data Structures + Algorithms\Assignment 1\ChessPython\images\kN.png")
black_knight = pygame.transform.scale(black_knight, (64,64))
black_knight_small = pygame.transform.scale(black_knight, (36,36))
black_pawn = pygame.image.load("C:/Users\swisnoski\OneDrive - Olin College of Engineering/2026_02 Fall\Data Structures + Algorithms\Assignment 1\ChessPython\images\kp.png")
black_pawn = pygame.transform.scale(black_pawn, (52,52))
black_pawn_small = pygame.transform.scale(black_pawn, (36,36))

white_queen = pygame.image.load("C:/Users\swisnoski\OneDrive - Olin College of Engineering/2026_02 Fall\Data Structures + Algorithms\Assignment 1\ChessPython\images\wQ.png")
white_queen = pygame.transform.scale(white_queen, (64,64))
white_queen_small = pygame.transform.scale(white_queen, (36,36))
white_king = pygame.image.load("C:/Users\swisnoski\OneDrive - Olin College of Engineering/2026_02 Fall\Data Structures + Algorithms\Assignment 1\ChessPython\images\wK.png")
white_king = pygame.transform.scale(white_king, (64,64))
white_king_small = pygame.transform.scale(white_king, (36,36))
white_rook = pygame.image.load("C:/Users\swisnoski\OneDrive - Olin College of Engineering/2026_02 Fall\Data Structures + Algorithms\Assignment 1\ChessPython\images\wR.png")
white_rook = pygame.transform.scale(white_rook, (64,64))
white_rook_small = pygame.transform.scale(white_rook, (36,36))
white_bishop = pygame.image.load("C:/Users\swisnoski\OneDrive - Olin College of Engineering/2026_02 Fall\Data Structures + Algorithms\Assignment 1\ChessPython\images\wB.png")
white_bishop = pygame.transform.scale(white_bishop, (64,64))
white_bishop_small = pygame.transform.scale(white_bishop, (36,36))
white_knight = pygame.image.load("C:/Users\swisnoski\OneDrive - Olin College of Engineering/2026_02 Fall\Data Structures + Algorithms\Assignment 1\ChessPython\images\wN.png")
white_knight = pygame.transform.scale(white_knight, (64,64))
white_knight_small = pygame.transform.scale(white_knight, (36,36))
white_pawn = pygame.image.load("C:/Users\swisnoski\OneDrive - Olin College of Engineering/2026_02 Fall\Data Structures + Algorithms\Assignment 1\ChessPython\images\wp.png")
white_pawn = pygame.transform.scale(white_pawn, (52,52))
white_pawn_small = pygame.transform.scale(white_pawn, (36,36))

white_images = [white_pawn, white_queen, white_king, white_knight, white_rook, white_bishop]
white_images_small = [white_pawn_small, white_queen_small, white_king_small, white_knight_small, white_rook_small, white_bishop_small]
black_images = [black_pawn, black_queen, black_king, black_knight, black_rook, black_bishop]
black_images_small = [black_pawn_small, black_queen_small, black_king_small, black_knight_small, black_rook_small, black_bishop_small]
piece_list = ['pawn', 'queen', 'king', 'knight', 'rook', 'bishop']

# check variables/flashing counter
counter = 0 
winner = ''
game_over = False

# draw main game board
def draw_board():
    for i in range(32):
        column = i % 4
        row = i // 4 
        if row % 2 == 0:
            pygame.draw.rect(screen, 'light gray', [480 - (column*160), row * 80,80,80])
        else:
            pygame.draw.rect(screen, 'light gray', [560 - (column*160), row * 80,80,80])
    pygame.draw.rect(screen, 'gray', [0, 640, width, 80])
    pygame.draw.rect(screen, 'gold', [0, 640, width, 80], 5)
    pygame.draw.rect(screen, 'gold', [640, 0, 160, height], 5)
    status_text = ['White: Select a Piece to Move!', 'White: Select a Destination!', 
                    'Black: Select a Piece to Move!', 'Black: Select a Destination!']
    screen.blit(big_font.render(status_text[turn_step], True, 'black'), (20, 660))
    for i in range(9):
        pygame.draw.line(screen, 'black', (0, 80*i), (640, 80*i), 2)
        pygame.draw.line(screen, 'black', (80*i,0), (80*i,640), 2)

# draw pieces on board
def draw_pieces():
    for i in range(len(white_pieces)):
        index = piece_list.index(white_pieces[i])
        if white_pieces[i] == 'pawn':
            screen.blit(white_pawn, (white_locations[i][0]*80 + 13, white_locations[i][1]*80 + 14))
        else: 
            screen.blit(white_images[index], (white_locations[i][0]*80 + 8, white_locations[i][1]*80 + 8))
        if turn_step < 2:
            if selection == i:
                pygame.draw.rect(screen, 'red', [white_locations[i][0] * 80 + 1, white_locations[i][1] * 80 + 1, 80, 80], 2)

    for i in range(len(black_pieces)):
        index = piece_list.index(black_pieces[i])
        if black_pieces[i] == 'pawn':
            screen.blit(black_pawn, (black_locations[i][0]*80 + 13, black_locations[i][1]*80 + 14))
        else: 
            screen.blit(black_images[index], (black_locations[i][0]*80 + 8, black_locations[i][1]*80 + 8))
        if turn_step >= 2:
            if selection == i:
                pygame.draw.rect(screen, 'blue', [black_locations[i][0] * 80 + 1, black_locations[i][1] * 80 + 1, 80, 80], 2)

# check all pieces valid options on board
def check_options(pieces, locations, turn):
    moves_list = []
    all_moves_list = []
    for i in range (len(pieces)):
        location = locations[i]
        piece = pieces[i]
        if piece == 'pawn':
            moves_list = check_pawn(location, turn)
        elif piece == 'rook':
            moves_list = check_rook(location, turn)
        elif piece == 'knight':
            moves_list = check_knight(location, turn)
        elif piece == 'bishop':
            moves_list = check_bishop(location, turn)
        elif piece == 'queen':
            moves_list = check_queen(location, turn)
        elif piece == 'king':
            moves_list = check_king(location, turn)
        all_moves_list.append(moves_list)
    return all_moves_list

# check valid pawn moves
def check_pawn(position, color):
    moves_list = []
    if color == 'white':
        if (position[0], position [1] + 1) not in white_locations and (position[0], position [1] + 1) not in black_locations and position [1] < 7:
            moves_list.append((position[0], position[1] + 1))
        if (position[0], position [1] + 1) not in white_locations and (position[0], position [1] + 1) not in black_locations and position [1] == 1:
            moves_list.append((position[0], position[1] + 2))
        if (position[0] + 1, position [1] + 1) in black_locations:
            moves_list.append((position[0] + 1, position [1] + 1))
        if (position[0] + -1, position [1] + 1) in black_locations:
            moves_list.append((position[0] + -1, position [1] + 1))
    else:
        if (position[0], position [1] - 1) not in white_locations and (position[0], position [1] - 1) not in black_locations and position [1] > 0:
            moves_list.append((position[0], position[1] - 1))
        if (position[0], position [1] - 1) not in white_locations and (position[0], position [1] - 1) not in black_locations and position [1] == 6:
            moves_list.append((position[0], position[1] - 2))
        if (position[0] + 1, position [1] - 1) in white_locations:
            moves_list.append((position[0] + 1, position [1] - 1))
        if (position[0] - 1, position [1] - 1) in white_locations:
            moves_list.append((position[0] -1, position [1] - 1))
    return moves_list

# check valid rook moves
def check_rook(position, color):
    moves_list = []
    if color == 'white':
        enemies_list = black_locations
        friends_list = white_locations
    else:
        enemies_list = white_locations
        friends_list = black_locations
    for i in range(4):    # up, down, right, left
        path = True
        chain = 1
        if i == 0:
            x = 0
            y = 1
        elif i == 1:
            x = 0
            y = -1
        elif i == 2:
            x = 1
            y = 0
        elif i == 3:
            x = -1 
            y = 0
        while path:
            if (position[0] + (chain*x), position[1] + (chain*y)) not in friends_list and 0 <= position[0] + (chain*x) <= 7 and  0 <= position[1] + (chain*y) <= 7:
                moves_list.append((position[0] + (chain*x), position[1] + (chain*y)))
                if (position[0] + (chain*x), position[1] + (chain*y)) in enemies_list:
                    path = False
                chain += 1

            else: 
                path = False
    return moves_list

# check valid knight moves
def check_knight(position, color):
    moves_list = []
    if color == 'white':
        friends_list = white_locations
    else:
        friends_list = black_locations

    targets = [(1, 2), (1, -2), (2,1), (2,-1), (-1, 2), (-1, -2), (-2, 1), (-2,-1)]
    for i in range(8):
        target = (position[0] + targets[i][0], position[1] + targets[i][1])
        if target not in friends_list and 0 <= target[0] <= 7 and 0 <= target[1] <= 7:
            moves_list.append(target)

    return moves_list

# check valid bishop moves
def check_bishop(position, color):
    moves_list = []
    if color == 'white':
        enemies_list = black_locations
        friends_list = white_locations
    else:
        enemies_list = white_locations
        friends_list = black_locations
    for i in range(4):    # up-right, up-left, down-right, down-left
        path = True
        chain = 1
        if i == 0:
            x = 1
            y = -1
        elif i == 1:
            x = -1
            y = -1
        elif i == 2:
            x = 1
            y = 1
        elif i == 3:
            x = -1 
            y = 1
        while path:
            if (position[0] + (chain*x), position[1] + (chain*y)) not in friends_list and 0 <= position[0] + (chain*x) <= 7 and  0 <= position[1] + (chain*y) <= 7:
                moves_list.append((position[0] + (chain*x), position[1] + (chain*y)))
                if (position[0] + (chain*x), position[1] + (chain*y)) in enemies_list:
                    path = False
                chain += 1

            else: 
                path = False
    return moves_list

# check valid queen moves
def check_queen(position, color):
    moves_list = check_bishop(position, color)
    second_list = check_rook(position, color)
    for i in range(len(second_list)):
        moves_list.append(second_list[i])
    return moves_list

# check valid king moves
def check_king(position, color):
    moves_list = []
    if color == 'white':
        enemies_list = black_locations
        friends_list = white_locations
    else:
        enemies_list = white_locations
        friends_list = black_locations
    targets = [(1, 0), (-1, 0), (1,1), (1,-1), (0, 1), (0, -1), (-1,-1), (-1,1)]
    for i in range(8):
        target = (position[0] + targets[i][0], position[1] + targets[i][1])
        if target not in friends_list and 0 <= target[0] <= 7 and 0 <= target[1] <= 7:
            moves_list.append(target)
    return moves_list

# check valid moves for selected piece 
def check_valid_moves():
    if turn_step < 2:
        options_list = white_options
    else:
        options_list = black_options
    valid_options = options_list[selection]
    return valid_options

# draw captured pieces on screen
def draw_captured():
    for i in range(len(captured_pieces_white)):
        captured_piece = captured_pieces_white[i]
        index = piece_list.index(captured_piece)
        screen.blit(black_images_small[index], (660, 4 + 40*i))
    for i in range(len(captured_pieces_black)):
        captured_piece = captured_pieces_black[i]
        index = piece_list.index(captured_piece)
        screen.blit(white_images_small[index], (740, 4 + 40*i))

# draw valid moves on screen
def draw_valid(moves):
    if turn_step < 2:
        color = 'red'
    else: 
        color = 'blue'
    for i in range(len(moves)):
        pygame.draw.circle(screen, color, (moves[i][0] * 80 + 40, moves[i][1] * 80 + 40), 5)

# check if king is in check, draw check
def draw_check():
    if turn_step < 2:
        if 'king' in white_pieces:
            king_index = white_pieces.index('king')
            king_location = white_locations[king_index]
            for i in range(len(black_options)):
                if king_location in black_options[i]:
                    if counter < 15:
                        pygame.draw.rect(screen, 'dark red', [white_locations[king_index][0]*80 + 1, white_locations[king_index][1]*80 + 1, 80, 80], 5)
    else:
        if 'king' in black_pieces:
            king_index = black_pieces.index('king')
            king_location = black_locations[king_index]
            for i in range(len(white_options)):
                if king_location in white_options[i]:
                    if counter < 15:
                        pygame.draw.rect(screen, 'dark blue', [black_locations[king_index][0]*80 + 1, black_locations[king_index][1]*80 + 1, 80, 80], 5)

# draw game over condition
def draw_game_over():
    pygame.draw.rect(screen, 'black', [160,160,320,80])
    screen.blit(font.render(f'{winner} won the game!', True, 'white'), (168,168))
    screen.blit(font.render(f'Press ENTER to Restart', True, 'white'), (168,192))
    

# main game loop
black_options = check_options(black_pieces, black_locations, 'black')
white_options = check_options(white_pieces, white_locations, 'white')

run = True
while run:
    timer.tick(fps)
    if counter < 30:
        counter += 1 
    else:
        counter = 0
    screen.fill('dark gray')
    draw_board()
    draw_pieces()
    if selection != 100:
        valid_moves = check_valid_moves()
        draw_valid(valid_moves)
    draw_captured()
    draw_check()

    # event handling
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            run = False
        if event.type == pygame.MOUSEBUTTONDOWN and event.button == 1 and not game_over:
            x_coord = event.pos[0] // 80
            y_coord = event.pos[1] // 80
            click_coords = (x_coord, y_coord)
            if turn_step <= 1 : 
                if click_coords in white_locations: 
                    selection = white_locations.index(click_coords)
                    if turn_step == 0: 
                        turn_step = 1
                if click_coords in valid_moves and selection != 100: 
                    white_locations[selection] = click_coords
                    if click_coords in black_locations:
                        captured_black_piece = black_locations.index(click_coords)
                        captured_pieces_white.append(black_pieces[captured_black_piece])
                        if black_pieces[captured_black_piece] == 'king':
                            winner = 'white'
                        black_pieces.pop(captured_black_piece)
                        black_locations.pop(captured_black_piece)
                    black_options = check_options(black_pieces, black_locations, 'black')
                    white_options = check_options(white_pieces, white_locations, 'white')
                    turn_step = 2
                    selection = 100
                    valid_moves = []

            if turn_step > 1:
                if click_coords in black_locations: 
                    selection = black_locations.index(click_coords)
                    if turn_step == 2: 
                        turn_step = 3
                if click_coords in valid_moves and selection != 100: 
                    black_locations[selection] = click_coords
                    if click_coords in white_locations:
                        captured_white_piece = white_locations.index(click_coords)
                        captured_pieces_black.append(white_pieces[captured_white_piece])
                        if white_pieces[captured_white_piece] == 'king':
                            winner = 'black'
                        white_pieces.pop(captured_white_piece)
                        white_locations.pop(captured_white_piece)
                    black_options = check_options(black_pieces, black_locations, 'black')
                    white_options = check_options(white_pieces, white_locations, 'white')
                    turn_step = 0
                    selection = 100
                    valid_moves = []
        if event.type == pygame.KEYDOWN and game_over:
            if event.key == pygame.K_RETURN:
                game_over = False
                winner = ''
                white_pieces = ['rook', 'knight', 'bishop','king','queen', 'bishop','knight','rook',
                'pawn','pawn','pawn','pawn','pawn','pawn','pawn','pawn']
                white_locations = [(0,0),(1,0),(2,0),(3,0),(4,0),(5,0),(6,0),(7,0),
                   (0,1),(1,1),(2,1),(3,1),(4,1),(5,1),(6,1),(7,1)]
                black_pieces = ['rook', 'knight', 'bishop','king','queen', 'bishop','knight','rook',
                'pawn','pawn','pawn','pawn','pawn','pawn','pawn','pawn']
                black_locations = [(0,7),(1,7),(2,7),(3,7),(4,7),(5,7),(6,7),(7,7),
                   (0,6),(1,6),(2,6),(3,6),(4,6),(5,6),(6,6),(7,6)]
                captured_pieces_white = []
                captured_pieces_black = []
                turn_step = 0
                selection = 100
                valid_moves = []
                black_options = check_options(black_pieces, black_locations, 'black')
                white_options = check_options(white_pieces, white_locations, 'white')



    if winner != '':
        game_over = True
        draw_game_over()


    
    pygame.display.flip()
pygame.quit

