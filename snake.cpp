#include <SDL2/SDL.h>
#include <SDL2/SDL_keycode.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include "snake.hpp"
using namespace std;
// dinmensions of window
const int ScreenWidth = 800;
const int ScreenHeight = 600;
const int cell = 20;

struct SnakeSegment {
    // coordinantes of snake on the grid 
    int x, y;
};

SDL_Rect makeRect(int x, int y) {
    SDL_Rect r = { x, y, cell, cell };
    return r;
}

enum Direction { UP, DOWN, LEFT, RIGHT };

// Restart game function
void resetGame(vector<SnakeSegment>& snake, SDL_Rect& apple, vector<SDL_Rect>& walls)
{
    snake.clear();
    snake.push_back({ ScreenWidth / 2, ScreenHeight / 2 });

    apple = makeRect(rand() % (ScreenWidth / cell) * cell,rand() % (ScreenHeight / cell) * cell);

    walls.clear();
    int wallCount = 20; // number of random walls

    for (int i = 0; i < wallCount; i++)
    {
        int wx = rand() % (ScreenWidth / cell) * cell;
        int wy = rand() % (ScreenHeight / cell) * cell;
        walls.push_back(makeRect(wx, wy));
    }
}

void init_snake()
{
    int score = 0;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Snake with Walls", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, ScreenWidth, ScreenHeight, 0);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    srand((unsigned)time(nullptr));

    // Snake
    vector<SnakeSegment> snake;
    SDL_Rect apple;
    vector<SDL_Rect> walls;

    resetGame(snake, apple, walls);
    Direction dir = RIGHT;

    bool running = true;

    while (running)
    {
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
                running = false;

            if (e.type == SDL_KEYDOWN)
            {
                if (e.key.keysym.sym == SDLK_UP && dir != DOWN) dir = UP;
                if (e.key.keysym.sym == SDLK_DOWN && dir != UP) dir = DOWN;
                if (e.key.keysym.sym == SDLK_LEFT && dir != RIGHT) dir = LEFT;
                if (e.key.keysym.sym == SDLK_RIGHT && dir != LEFT) dir = RIGHT;
				if (e.key.keysym.sym == SDLK_ESCAPE) running = false;//added by SuchATypicalDev
            }
        }

        // Move body
        for (int i = snake.size() - 1; i > 0; i--) snake[i] = snake[i - 1];

        // Move head
        if (dir == UP)    snake[0].y -= cell;
        if (dir == DOWN)  snake[0].y += cell;
        if (dir == LEFT)  snake[0].x -= cell;
        if (dir == RIGHT) snake[0].x += cell;

        // wall collision
        for (auto& w : walls)
        {
            if (snake[0].x == w.x && snake[0].y == w.y)
            {
                resetGame(snake, apple, walls);
                dir = RIGHT;
            }
        }

        // Self collision
        for (int i = 1; i < (int)snake.size(); i++)
        {
            if (snake[0].x == snake[i].x && snake[0].y == snake[i].y)
            {
                resetGame(snake, apple, walls);
                dir = RIGHT;
            }
        }

        // Wall Boundary collision (screen edges)
        if (snake[0].x < 0 || snake[0].x >= ScreenWidth ||
            snake[0].y < 0 || snake[0].y >= ScreenHeight)
        {
            resetGame(snake, apple, walls);
            dir = RIGHT;
        }

        // Apple collision
    if (snake[0].x == apple.x && snake[0].y == apple.y)
    {
        snake.push_back({ snake.back().x, snake.back().y });

        score++;  
        cout << "Score = " << score << endl;

        apple = makeRect(rand() % (ScreenWidth / cell) * cell, rand() % (ScreenHeight / cell) * cell);
    }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Draw apple
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &apple);

        // Draw random walls
        SDL_SetRenderDrawColor(renderer, 100, 100, 255, 255);
        for (auto& w : walls)
            SDL_RenderFillRect(renderer, &w);

        // Draw snake
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        for (auto& s : snake)
        {
            SDL_Rect r = makeRect(s.x, s.y);
            SDL_RenderFillRect(renderer, &r);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(100);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return ;
}
