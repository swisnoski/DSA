// Sam Wisnoski
// 09/09/2026
// DSA Translation Project: Chess

/*
Okay so I was a bit too ambition with this one thinking I could do thing in a new language 
so quickly, sooooo we ended up with sort of chess? unfortunately I spent a whole bunch of time
learning a library that I'll never probably use for this class again, but the good news is that I got 
a ton of practice with c++, so yay! 

Anyway, here is "SortaChess". Essentially, we've generated a chess board and pieces, 
and just like a regular physical chess board, you can pick up a piece and move it to a new location ANYWHERE 
on the board. And the other piece stays there as well, because why would it magically teleport away? 

So it's not really super playable, but whatever. It's written in C++, soooooooo hopefully counts. 
*/


/* so since we use pygame in the original project, we've got to switch
things up a bit here. Apparently, SDL3 is the C++ equivalent of pygame, 
so we'll use that instead. */

#include <SDL3/SDL.h> // for basically everything 
#include <iostream> // for debugging output 
#include <array> // For std::array
#include <algorithm> // For std::find



// declare some variables. in pygame, we set the window size, window
// the font, etc, and we can pretty much do the same thing here. 

int width = {720};
int height = {720};
SDL_Window* screen = SDL_CreateWindow("Funny Chess but now it's C++", width, height, 0); 
SDL_Renderer *screen_renderer = SDL_CreateRenderer(screen, nullptr);

int fps = {60};  


////////////////////////////////////////////////////// 

// next, we need to create the game variables and images. we use 
// a list to store the pieces and their starting positions for each color 

// okay so apparently to do this we want an array of strings, which we can 
// make my declaring the type, then the name and max number of elements in the array, 
//then we can initialize it with the values we want.
std::string white_pieces[16] = {"rook", "knight", "bishop","king","queen", "bishop","knight","rook",
                "pawn","pawn","pawn","pawn","pawn","pawn","pawn","pawn"};
int num_white_pieces = {16};
// we have to use std::string because we are using the string class from the std library

// for tuples, we can do the same thing, just need to declare what type is in the tuple
std::array<std::array<int, 2>, 16> white_positions = {{
    {{0, 0}}, {{1, 0}}, {{2, 0}}, {{3, 0}},
    {{4, 0}}, {{5, 0}}, {{6, 0}}, {{7, 0}},
    {{0, 1}}, {{1, 1}}, {{2, 1}}, {{3, 1}},
    {{4, 1}}, {{5, 1}}, {{6, 1}}, {{7, 1}}
}};


// and then we can do the same thing for black pieces and positions
std::string black_pieces[16] = {"rook", "knight", "bishop","king","queen", "bishop","knight","rook",
                "pawn","pawn","pawn","pawn","pawn","pawn","pawn","pawn"};
int num_black_pieces = {16};
std::array<std::array<int, 2>, 16> black_positions = {{
    {{0, 7}}, {{1, 7}}, {{2, 7}}, {{3, 7}},
    {{4, 7}}, {{5, 7}}, {{6, 7}}, {{7, 7}},
    {{0, 6}}, {{1, 6}}, {{2, 6}}, {{3, 6}},
    {{4, 6}}, {{5, 6}}, {{6, 6}}, {{7, 6}}
}};

// then we have a few random other things to declare that allow us to have a functioning chess game 
int turn_step = {0};
std::array<std::array<int, 2>, 2> selection = {{0, 0}};
std::array<std::array<int, 2>, 64> valid_moves = {}; //set to 64 since thats the max number of possible moves even though it technically could be less than that

// and then we have to load in the game pieces
// we can do this by loading the image first onto a surface, and then converting to a texture
// we need to have the images in bmp format in order to use this function, else we would have 
// to use a different library, which I did attempt at first but pretty much immedietly gave up. 
SDL_Surface* black_rook = SDL_LoadBMP("C:/Users/swisnoski/OneDrive - Olin College of Engineering/2026_02 Fall/Data Structures + Algorithms/Assignment 1/ChessCpp/images/kR.bmp");
SDL_Surface* black_queen = SDL_LoadBMP("C:/Users/swisnoski/OneDrive - Olin College of Engineering/2026_02 Fall/Data Structures + Algorithms/Assignment 1/ChessCpp/images/kQ.bmp");
SDL_Surface* black_king = SDL_LoadBMP("C:/Users/swisnoski/OneDrive - Olin College of Engineering/2026_02 Fall/Data Structures + Algorithms/Assignment 1/ChessCpp/images/kK.bmp");
SDL_Surface* black_bishop = SDL_LoadBMP("C:/Users/swisnoski/OneDrive - Olin College of Engineering/2026_02 Fall/Data Structures + Algorithms/Assignment 1/ChessCpp/images/kB.bmp");
SDL_Surface* black_knight = SDL_LoadBMP("C:/Users/swisnoski/OneDrive - Olin College of Engineering/2026_02 Fall/Data Structures + Algorithms/Assignment 1/ChessCpp/images/kN.bmp");
SDL_Surface* black_pawn = SDL_LoadBMP("C:/Users/swisnoski/OneDrive - Olin College of Engineering/2026_02 Fall/Data Structures + Algorithms/Assignment 1/ChessCpp/images/kP.bmp");
SDL_Surface* white_rook = SDL_LoadBMP("C:/Users/swisnoski/OneDrive - Olin College of Engineering/2026_02 Fall/Data Structures + Algorithms/Assignment 1/ChessCpp/images/wR.bmp");
SDL_Surface* white_queen = SDL_LoadBMP("C:/Users/swisnoski/OneDrive - Olin College of Engineering/2026_02 Fall/Data Structures + Algorithms/Assignment 1/ChessCpp/images/wQ.bmp");
SDL_Surface* white_king = SDL_LoadBMP("C:/Users/swisnoski/OneDrive - Olin College of Engineering/2026_02 Fall/Data Structures + Algorithms/Assignment 1/ChessCpp/images/wK.bmp");
SDL_Surface* white_bishop = SDL_LoadBMP("C:/Users/swisnoski/OneDrive - Olin College of Engineering/2026_02 Fall/Data Structures + Algorithms/Assignment 1/ChessCpp/images/wB.bmp");
SDL_Surface* white_knight = SDL_LoadBMP("C:/Users/swisnoski/OneDrive - Olin College of Engineering/2026_02 Fall/Data Structures + Algorithms/Assignment 1/ChessCpp/images/wN.bmp");
SDL_Surface* white_pawn = SDL_LoadBMP("C:/Users/swisnoski/OneDrive - Olin College of Engineering/2026_02 Fall/Data Structures + Algorithms/Assignment 1/ChessCpp/images/wP.bmp");
SDL_Texture* black_rook_texture = SDL_CreateTextureFromSurface(screen_renderer, black_rook); 
SDL_Texture* black_queen_texture = SDL_CreateTextureFromSurface(screen_renderer, black_queen);
SDL_Texture* black_king_texture = SDL_CreateTextureFromSurface(screen_renderer, black_king);
SDL_Texture* black_bishop_texture = SDL_CreateTextureFromSurface(screen_renderer, black_bishop);
SDL_Texture* black_knight_texture = SDL_CreateTextureFromSurface(screen_renderer, black_knight);
SDL_Texture* black_pawn_texture = SDL_CreateTextureFromSurface(screen_renderer, black_pawn);
SDL_Texture* white_rook_texture = SDL_CreateTextureFromSurface(screen_renderer, white_rook);
SDL_Texture* white_queen_texture = SDL_CreateTextureFromSurface(screen_renderer, white_queen);
SDL_Texture* white_king_texture = SDL_CreateTextureFromSurface(screen_renderer, white_king);
SDL_Texture* white_bishop_texture = SDL_CreateTextureFromSurface(screen_renderer, white_bishop);
SDL_Texture* white_knight_texture = SDL_CreateTextureFromSurface(screen_renderer, white_knight);
SDL_Texture* white_pawn_texture = SDL_CreateTextureFromSurface(screen_renderer, white_pawn);

// then, following the example of our original code, we group those into arrays, which we can use to index in the future. 
SDL_Texture* black_piece_textures[6] = {black_rook_texture, black_knight_texture, black_bishop_texture, black_queen_texture, black_king_texture, black_pawn_texture};
SDL_Texture* white_piece_textures[6] = {white_rook_texture, white_knight_texture, white_bishop_texture, white_queen_texture, white_king_texture, white_pawn_texture};
std::string piece_list[6] = {"rook", "knight", "bishop", "queen", "king", "pawn"};


// we are not sizing that shit up because we don't need to. no fancy here 
// I don’t want fancy things.
// I don’t want fancy schmacy things.
// I don’t want fancy schmacy whancy things.
// I don’t want fancy schmacy whancy take me on a trip to Paris Francy things.
// What I do what
// Is to waste your and my time.
// - Norm Macdonald

// okay lets try to make a quick simple board
int column = {0};
int row = {0};
SDL_FRect rect = {0, 0, 90, 90};

// that we have our images set to go, we can draw a board using some simple rectangles! 
// we need 32 dark squares, so we can iterate 32 times and draw one square per iteration, 
// jumping accross rows and columns as we go 

// math in C++ is mostly the same minus some syntax and variable declaration differences, but 
// it's mostly familiar and pretty doable 
void drawboard(){
    for (int i = 0; i <= 31; i++) {
        column = i % 4;
        row = i / 4; // just does integer division since both are ints i guess
        if (row % 2 == 0){
            rect.x = column * 180;
            rect.y = row * 90;
            SDL_SetRenderDrawColor(screen_renderer, 211, 50, 211, 255);
            SDL_RenderFillRect(screen_renderer, &rect);
        }
        else{
            rect.x = column * 180 + 90;
            rect.y = row * 90;
            SDL_SetRenderDrawColor(screen_renderer, 211, 50, 211, 255);
            SDL_RenderFillRect(screen_renderer, &rect);
        }
    }
    return;
}

// i was trying to use the "find" function for a while 
// but gave up and eventually just wrote my own function to index a list 
int get_index(std::string target) {
    for (int j = 0; j < 6; j++) {
        if (piece_list[j] == target) return j;
    }
}

// so again we just copy over the exact same logic for drawing pieces as before, minus the scaling changes 
// we just declare each texture as a rectangle, and then draw that accross the board based on 
// our lists of starting locations. it's the same process for each color 
void draw_pieces() {
    for (int i = 0; i < num_white_pieces; i++) {
        // Find which texture matches this piece name
        int index = get_index(white_pieces[i]);

        SDL_FRect dstrect = {
            white_positions[i][0] * 90.0f,
            white_positions[i][1] * 90.0f,
            90.0f,
            90.0f
        };

        // Render texture
        SDL_RenderTexture(screen_renderer, white_piece_textures[index], nullptr, &dstrect);
    }
    for (int i = 0; i < num_black_pieces; i++) {
        // Find which texture matches this piece name
        int index = get_index (black_pieces[i]);

        SDL_FRect dstrect = {
            black_positions[i][0] * 90.0f,
            black_positions[i][1] * 90.0f,
            90.0f,
            90.0f
        };

        // Render texture
        SDL_RenderTexture(screen_renderer, black_piece_textures[index], nullptr, &dstrect);
    }
}


//////////////////////////
/*

UNIT TEST TIME 
(i hate unit tests)
Also I wasn't really sure what unit tests to write because this isn't a very 
straightforward, easily testible program. I kinda procrastinated on this part and 
now it's very late and night and so I'm just going to steal the unit test format I 
had for the meeting scheduler and run them with main 

*/ 
////////////////////////

void runTest(const std::string& test_name, bool actual, bool expected) {
    std::cout << test_name << ":\n";
    std::cout << "  Result: " << actual << "\n";
    std::cout << "  Expected: " << expected << "\n";
    std::cout << "---------\n";
}

// and we can just add all the tests in one function we call in main that 
void runAllTests() {
    std::cout << "=== Running SortaChess Unit Tests ===\n\n";

    // i wanted to test some of the "trickier" parts in C for this project 
    // since 99% of the logic is just borrowed from the original python project
    // getting index to work was a pain and I had to write my own function 
    // so I figured that was a good place to start 
    bool t1_result = (get_index("rook") == 0);
    runTest("index test rook", t1_result, true);

    bool t2_result = (get_index("pawn") == 5);
    runTest("index test pawn", t2_result, true);

    // and then after that i kinda ran out of good idea's for unit tests 
    // so let's just check some random parts of my code! 
    // this tests that mouse clicks accurately convert to tiles 
    int click_x = 45 / 90;
    int click_y = 45 / 90;
    bool t3_result = (click_x == 0 && click_y == 0);
    runTest("pixel to tile test", t3_result, true);

    // and we can also index a piece by it's position and test that too 
    int found_index = -1;
    for (int i = 0; i < num_white_pieces; i++) {
        if (white_positions[i][0] == 0 && white_positions[i][1] == 0) {
            found_index = i;
            break;
        }
    }
    bool t4_result = (found_index == 0);
    runTest("find rook at top left tile", t4_result, true);

    // and then let's try RUNNING THOSE TESTS!! 
}

////// END UNIT TEST SECTION 


// lastly, our main function 
int main() { //we make the main function int for some stupid reason (because we return 0 on success)
    std::cout << "ChessCpp started successfully.\n";
    // the first thing we need to do is initialize the game 
    SDL_Init(SDL_INIT_VIDEO);

    runAllTests();
    /////////////////////////////////////////////////////
    // now for the main event loop. this is pretty different since 
    // we skipped a lot of the main mean of chess, but don't you 
    // worry, this one is a hell of a lot simpler 

    // 
    int selection = -1; // define if a piece is "selected"
    bool w = nullptr; // color of the "selected piece"
    bool run = {true}; // run until we get a quick event 
    SDL_Event event; // initialize an event 
    while (run) {

        // our loop consists of drawing the board 
        // set color to green and clear the board 
        // draw the board tiles, draw pieces on top of tiles 
        // render the screen to the user 
        // add a short delay 
        SDL_SetRenderDrawColor(screen_renderer, 0, 100, 0, 255);
        SDL_RenderClear(screen_renderer);
        drawboard();
        draw_pieces();
        SDL_RenderPresent(screen_renderer);
        SDL_Delay(100);
        
    // if we have an event, we handle it 
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_EVENT_QUIT: // if quit event than we quit 
                run = false;
                break;

            // if its a button event we check if the user clicked on a piece 
            // we do this by just tracking the mouse on the board, then dividing by 90 
            // and matching it to our existing location lists 
            case SDL_EVENT_MOUSE_BUTTON_DOWN:  
                if (event.button.button == SDL_BUTTON_LEFT) {
                    int click_x = event.button.x / 90;
                    int click_y = event.button.y / 90;

                    // check if it's a match, if so we find the index and color of the piece 
                    // if selection is -1, no piece is actively selected
                    if (selection == -1) {
                        for (int i = 0; i < num_white_pieces; i++) {
                            if (white_positions[i][0] == click_x && white_positions[i][1] == click_y) {
                                selection = i;
                                w = true;
                                break;
                            }
                        }
                        for (int i = 0; i < num_black_pieces; i++) {
                            if (black_positions[i][0] == click_x && black_positions[i][1] == click_y) {
                                selection = i;
                                w = false;
                                break;
                            }
                        }
                    } 
                    // if selection is not -1, than a piece WAS actively selected, 
                    // and we use that selection index to update our positions list, 
                    // which will be immedietly re-rendered 
                    else {
                        if (w) {
                            white_positions[selection][0] = click_x;
                            white_positions[selection][1] = click_y;
                        } else {
                            black_positions[selection][0] = click_x;
                            black_positions[selection][1] = click_y;
                        }
                        selection = -1; // reset selection back to nothing
                    }
                }
                break;
            }
        }
    }

    // then we do a bit of teardown and that's the game! huzzah!

    SDL_DestroyRenderer(screen_renderer);
    SDL_DestroyWindow(screen);
    SDL_Quit();
    return 0;
}