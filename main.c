#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <SDL_ttf.h>
#define SCREEN_WIDTH 640  // window height
#define SCREEN_HEIGHT 480 // window widt

// surfaces
static SDL_Surface *screen;
static SDL_Surface *title;
static SDL_Surface *numbermap;
static SDL_Surface *end;

int width, height; // used if fullscreen

SDL_Window *window = NULL; // The window we'll be rendering to
SDL_Renderer *renderer;    // The renderer SDL will use to draw to the screen

// textures
SDL_Texture *screen_texture;
int gameState[4][4] = {{4, 4, 2, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};

static void draw_gradient()
{
    SDL_Rect src;
    src.x = 0;
    src.y = 0;
    src.h = SCREEN_HEIGHT / 40;
    int iterations = 0;
    while (src.y <= SCREEN_HEIGHT)
    {
        Uint32 colorkey = SDL_MapRGB(screen->format, 140 - (iterations * 3), 200 - (iterations * 3), 220 - (iterations * 3));
        src.w = SCREEN_WIDTH;
        int r = SDL_FillRect(screen, &src, colorkey);

        if (r != 0)
        {

            printf("fill gradient faliled in func drawball()");
        }
        src.y += SCREEN_HEIGHT / 40;
        iterations++;
    }
}

static void draw_text(char string[], SDL_Rect rect)
{
    TTF_Init();

    // this opens a font style and sets a size
    TTF_Font *Sans = TTF_OpenFont("font.ttf", 14);
    if (Sans == NULL)
    {
        printf("TTF_Init: %s\n", TTF_GetError());
        return;
    }
    SDL_Color black = {125, 125, 125};
    SDL_Surface *surfaceMessage;
    if (string[0] == '0')
    {
        surfaceMessage = TTF_RenderText_Solid(Sans, "", black);
    }
    else
    {
        surfaceMessage = TTF_RenderText_Solid(Sans, string, black);
    }

    // now you can convert it into a texture
    SDL_Texture *Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

    SDL_UpdateTexture(screen_texture, NULL, screen->pixels, screen->w * sizeof(Uint32));

    SDL_RenderCopy(renderer, Message, NULL, &rect);
    // Don't forget to free your surface and texture
    SDL_FreeSurface(surfaceMessage);
    SDL_DestroyTexture(Message);
}

char *convert_int_to_string(int num)
{
    char *str = malloc(5 * sizeof(char));
    sprintf(str, "%d", num);
    return str;
}
static void draw_tiles()
{
    SDL_Rect tile;

    for (int i = 0; i < 16; i++)
    {
        int xBuffer = i % 4;
        int yBuffer = i / 4;
        tile.x = (SCREEN_WIDTH / 2 - 175) + (75 * xBuffer) + (10 * (xBuffer + 1));
        tile.y = (SCREEN_HEIGHT / 2 - 175) + (75 * yBuffer) + (10 * (yBuffer + 1));

        tile.h = 75;
        tile.w = 75;
        Uint32 tileColor = SDL_MapRGB(screen->format, 238, 228, 218);
        int z = SDL_FillRect(screen, &tile, tileColor);

        if (z != 0)
        {

            printf("fill gradient faliled in func drawball()");
        }
        SDL_UpdateTexture(screen_texture, NULL, screen->pixels, screen->w * sizeof(Uint32));
        SDL_RenderCopy(renderer, screen_texture, NULL, NULL);
    }
}
static void draw_board()
{
    SDL_Rect background;
    background.h = SCREEN_HEIGHT;
    background.w = SCREEN_WIDTH;
    background.x = 0;
    background.y = 0;
    int t = SDL_FillRect(screen, &background, 0xffffffff);
    if (t != 0)
    {

        printf("fill background faliled in func drawball()");
    }
    SDL_Rect board;
    board.x = SCREEN_WIDTH / 2 - 175;
    board.y = SCREEN_HEIGHT / 2 - 175;
    board.h = 350;
    board.w = 350;
    Uint32 colorkey = SDL_MapRGB(screen->format, 187, 173, 160);
    int r = SDL_FillRect(screen, &board, colorkey);

    if (r != 0)
    {

        printf("fill gradient faliled in func drawball()");
    }

    SDL_UpdateTexture(screen_texture, NULL, screen->pixels, screen->w * sizeof(Uint32));
    SDL_RenderCopy(renderer, screen_texture, NULL, NULL);
}

static void draw_numbers()
{

    for (int i = 0; i < 16; i++)
    {
        int xBuffer = i % 4;
        int yBuffer = i / 4;
        SDL_Rect Message_rect;                                                              // create a rect
        Message_rect.x = SCREEN_WIDTH / 2 - 175 + (75 * xBuffer) + (10 * (xBuffer + 1));    // controls the rect's x coordinate
        Message_rect.y = (SCREEN_HEIGHT / 2 - 175) + (75 * yBuffer) + (10 * (yBuffer + 1)); // controls the rect's y coordinte
        Message_rect.w = 75;                                                                // controls the width of the rect
        Message_rect.h = 75;                                                                // controls the height of the rect

        draw_text(convert_int_to_string(gameState[yBuffer][xBuffer]), Message_rect);
    }
}

int init(int width, int height, int argc, char *args[])
{

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {

        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());

        return 1;
    }

    int i;

    for (i = 0; i < argc; i++)
    {

        // Create window
        if (strcmp(args[i], "-f"))
        {

            SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN, &window, &renderer);
        }
        else
        {

            SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_FULLSCREEN_DESKTOP, &window, &renderer);
        }
    }

    if (window == NULL)
    {

        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());

        return 1;
    }

    // create the screen sruface where all the elemnts will be drawn onto (ball, paddles, net etc)
    screen = SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, SDL_PIXELFORMAT_RGBA32);

    if (screen == NULL)
    {

        printf("Could not create the screen surfce! SDL_Error: %s\n", SDL_GetError());

        return 1;
    }

    // create the screen texture to render the screen surface to the actual display
    screen_texture = SDL_CreateTextureFromSurface(renderer, screen);

    if (screen_texture == NULL)
    {

        printf("Could not create the screen_texture! SDL_Error: %s\n", SDL_GetError());

        return 1;
    }

    // draw_gradient();

    return 0;
}

static void moveLeft()
{
    for (int i = 0; i < 4; i++)
    {
        int comparisonPos = 0;
        int emptyPos = 0;
        for (int j = 1; j < 4; j++)
        {
            if (gameState[i][j] == 0)
            {
                continue;
            }
            else if (gameState[i][j] == gameState[i][comparisonPos])
            {
                gameState[i][comparisonPos] = gameState[i][j] * 2;
                gameState[i][j] = 0;
                comparisonPos += 1;
                emptyPos++;
            }
            else if (gameState[i][emptyPos] != 0) // if the number in the emptyPos doesn't equal the current number leave the current numberin it's current position
            {
                continue;
            }
            else
            {
                gameState[i][emptyPos] = gameState[i][j];
                gameState[i][j] = 0;
                emptyPos++;
            }
        }
    }
}

static void moveRight()
{
    for (int i = 0; i < 4; i++)
    {
        int comparisonPos = 3;
        int emptyPos = 3;
        for (int j = 2; j >= 0; j--)
        {
            if (gameState[i][j] == 0)
            {
                continue;
            }
            else if (gameState[i][j] == gameState[i][comparisonPos])
            {
                gameState[i][comparisonPos] = gameState[i][j] * 2;
                gameState[i][j] = 0;
                comparisonPos--;
                emptyPos--;
            }
            else if (gameState[i][emptyPos] != 0)
            {
                continue;
            }
            else
            {
                gameState[i][emptyPos] = gameState[i][j];
                gameState[i][j] = 0;
                emptyPos--;
            }
        }
    }
}
int main(int argc, char *args[])
{
    if (init(SCREEN_WIDTH, SCREEN_HEIGHT, argc, args) == 1)
    {

        return 0;
    }
    SDL_GetWindowSize(window, &width, &height);

    int sleep = 0;
    int quit = 0;
    int state = 0;
    int r = 0;
    Uint32 next_game_tick = SDL_GetTicks();

    SDL_PumpEvents();

    // draw_menu();
    bool keep_window_open = true;
    SDL_Event e;

    while (keep_window_open)
    {
        while (SDL_PollEvent(&e) > 0)
        {
            switch (e.type)
            {
            case SDL_QUIT:
                keep_window_open = false;
                break;
            }

            if (e.type == SDL_KEYDOWN)
            {
                switch (e.key.keysym.sym)
                {
                case SDLK_LEFT:
                    moveLeft();
                    break;
                case SDLK_RIGHT:
                    moveRight();
                    break;
                }
            }
            draw_board();
            draw_tiles();
            draw_numbers();
            // draw to the display
            SDL_RenderPresent(renderer);
            SDL_Delay(50);
        }
        SDL_Delay(50);
    }

    // free loaded images
    SDL_FreeSurface(screen);
    SDL_FreeSurface(title);
    SDL_FreeSurface(numbermap);
    SDL_FreeSurface(end);

    // free renderer and all textures used with it
    SDL_DestroyRenderer(renderer);

    // Destroy window
    SDL_DestroyWindow(window);

    // Quit SDL subsystems
    SDL_Quit();

    return 0;
}
