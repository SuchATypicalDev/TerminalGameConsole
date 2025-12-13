#include <chrono>
#include <SDL2/SDL.h>
//#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <string>
#include "pong.hpp"

using namespace std;
// class PlayerScore
//{
// public:
// PlayerScore(Vec2 position, SDL_Renderer* renderer, TTF_Font* font)
//	: renderer(renderer), font(font)
//{
//	surface = TTF_RenderText_Solid(font, "0", {0xFF, 0xFF, 0xFF, 0xFF});
//	texture = SDL_CreateTextureFromSurface(renderer, surface);

//	int width, height;
// SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);

//	rect.x = static_cast<int>(position.x);
// rect.y = static_cast<int>(position.y);
// rect.w = width;
// rect.h = height;
//}
// void SetScore(int score)
//{
//	SDL_FreeSurface(surface);
//	SDL_DestroyTexture(texture);

// surface = TTF_RenderText_Solid(font, std::to_string(score).c_str(), {0xFF, 0xFF, 0xFF, 0xFF});
//	texture = SDL_CreateTextureFromSurface(renderer, surface);

//	int width, height;
//	SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);
//	rect.w = width;
// rect.h = height;
//}

//~PlayerScore()
//	{
//	SDL_FreeSurface(surface);
//	SDL_DestroyTexture(texture);
//}

// void Draw()
//{
//	SDL_RenderCopy(renderer, texture, nullptr, &rect);
// }

// SDL_Renderer* renderer;
// TTF_Font* font;
// SDL_Surface* surface{};
// SDL_Texture* texture{};
// SDL_Rect rect{};
//}
//;
class Vec2
{
public:
    Vec2()
        : x(0.0f), y(0.0f)
    {
    }
    Vec2(float x, float y)
        : x(x), y(y)
    {
    }
    Vec2 operator+(Vec2 const &rhs)
    {
        return Vec2(x + rhs.x, y + rhs.y);
    }
    Vec2 &operator+=(Vec2 const &rhs)
    {
        x += rhs.x;
        y += rhs.y;

        return *this;
    }
    Vec2 operator*(float rhs)
    {
        return Vec2(x * rhs, y * rhs);
    }

    float x, y;
};

const int BallWidth = 15;
const int BallHeight = 15;

enum class CollisionType
{
    None,
    Top,
    Middle,
    Bottom,
    Left,
    Right
};

struct Contact
{
    CollisionType type;
    float penetration;
};

const float BallSpeed = 0.75f;
class ball
{
public:
    ball(Vec2 position, Vec2 velocity)
        : position(position), velocity(velocity)
    {
        rect.x = static_cast<int>(position.x);
        rect.y = static_cast<int>(position.y);
        rect.w = BallWidth;
        rect.h = BallHeight;
    }
    void CollideWithPaddle(Contact const &contact)
    {
        position.x += contact.penetration;
        velocity.x = -velocity.x;

        if (contact.type == CollisionType::Top)
        {
            velocity.y = -.75f * BallSpeed;
        }
        else if (contact.type == CollisionType::Middle)
        {
            velocity.y = 0.0f;
        }
        else if (contact.type == CollisionType::Bottom)
        {
            velocity.y = 0.75f * BallSpeed;
        }
    }

    void CollideWithWall(Contact const &contact)
    {
        if ((contact.type == CollisionType::Top) || (contact.type == CollisionType::Bottom))
        {
            position.y += contact.penetration;
            velocity.y = -velocity.y;
        }
        else if (contact.type == CollisionType::Left)
        {
            position.x = 1280 / 2.0f;
            position.y = 720 / 2.0f;
            velocity.x = BallSpeed;
            velocity.y = 0.75f * BallSpeed;
        }
        else if (contact.type == CollisionType::Right)
        {
            position.x = 1280 / 2.0f;
            position.y = 720 / 2.0f;
            velocity.x = -BallSpeed;
            velocity.y = 0.75f * BallSpeed;
        }
    }
    void Update(float dt)
    {
        position += velocity * dt;
    }

    void Draw(SDL_Renderer *renderer)
    {
        rect.x = static_cast<int>(position.x);
        rect.y = static_cast<int>(position.y);

        SDL_RenderFillRect(renderer, &rect);
    }

    Vec2 position;
    Vec2 velocity;
    SDL_Rect rect{};
};

Contact CheckWallCollision(ball const &ball)
{
    float BallLeft = ball.position.x;
    float BallRight = ball.position.x + BallWidth;
    float BallTop = ball.position.y;
    float BallBottom = ball.position.y + BallHeight;
    Contact contact{};

    if (BallLeft < 0.0f)
    {
        contact.type = CollisionType::Left;
    }
    else if (BallRight > 1280)
    {
        contact.type = CollisionType::Right;
    }
    else if (BallTop < 0.0f)
    {
        contact.type = CollisionType::Top;
        contact.penetration = -BallTop;
    }
    else if (BallBottom > 720)
    {
        contact.type = CollisionType::Bottom;
        contact.penetration = 720 - BallBottom;
    }
    return contact;
}

const int PADDLE_WIDTH = 15;
const int PADDLE_HEIGHT = 150;

class Paddle
{
public:
    Paddle(Vec2 position, Vec2 velocity)
        : position(position), velocity(velocity)
    {
        rect.x = static_cast<int>(position.x);
        rect.y = static_cast<int>(position.y);
        rect.w = PADDLE_WIDTH;
        rect.h = PADDLE_HEIGHT;
    }

    void update(float dt)
    {
        position += velocity * dt;

        if (position.y < 0)
        {
            position.y = 0;
        }
        else if (position.y > (720 - PADDLE_HEIGHT))
        {
            position.y = 720 - PADDLE_HEIGHT;
        }
    }

    void draw(SDL_Renderer *renderer)
    {
        rect.y = static_cast<int>(position.y);
        SDL_RenderFillRect(renderer, &rect);
    }

    Vec2 position;
    Vec2 velocity;
    SDL_Rect rect{};
};

const float PaddleSpeed = 0.9f;

enum Buttons
{
    PaddleOneUp = 0,
    PaddleOneDown,
    PaddleTwoUp,
    PaddleTwoDown,
};

Contact CheckPaddleCollision(ball const &ball, Paddle const &paddle)
{
    float BallLeft = ball.position.x;
    float BallRight = ball.position.x + BallWidth;
    float PaddleLeft = paddle.position.x;
    float PaddleRight = paddle.position.x + PADDLE_WIDTH;
    float BallTop = ball.position.y;
    float BallBottom = ball.position.y + BallHeight;
    float PaddleTop = paddle.position.y;
    float PaddleBottom = paddle.position.y + PADDLE_HEIGHT;

    Contact contact{};
    if (BallLeft >= PaddleRight)
    {
        return contact;
    }
    if (BallRight <= PaddleLeft)
    {
        return contact;
    }
    if (BallTop >= PaddleBottom)
    {
        return contact;
    }
    if (BallBottom <= PaddleTop)
    {
        return contact;
    }

    float paddleRangeUpper = PaddleBottom - (2.0f * PADDLE_HEIGHT / 3.0f);
    float paddleRangeMiddle = PaddleBottom - (PADDLE_HEIGHT / 3.0f);
    if (ball.velocity.x < 0)
    {

        contact.penetration = PaddleRight - BallLeft;
    }
    else if (ball.velocity.x > 0)
    {

        contact.penetration = PaddleLeft - BallRight;
    }

    if ((BallBottom > PaddleTop) && (BallBottom < paddleRangeUpper))
    {
        contact.type = CollisionType::Top;
    }
    else if ((BallBottom > paddleRangeUpper) && (BallBottom < paddleRangeMiddle))
    {
        contact.type = CollisionType::Middle;
    }
    else
    {
        contact.type = CollisionType::Bottom;
    }

    return contact;
}

void init_pong()
{

    SDL_Init(SDL_INIT_VIDEO);
    // TTF_Init();

    SDL_Window *window = SDL_CreateWindow("pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

    //  TTF_Font *scoreFont = TTF_OpenFont("DejaVuSansMono.ttf", 40);
    // PlayerScore playerOneScoreText(Vec2(WINDOW_WIDTH / 4, 20), renderer, scoreFont, scoreColor);
    // PlayerScore playerTwoScoreText(Vec2(3 * WINDOW_WIDTH / 4, 20), renderer, scoreFont, scoreColor);
    ball ball(
        Vec2((1280 / 2.0f) - (BallWidth / 2.0f),
             (720 / 2.0f) - (BallWidth / 2.0f)),
        Vec2(BallSpeed, 0.0f));

    Paddle PaddleOne(
        Vec2(50.0f, (720 / 2.0f) - (PADDLE_HEIGHT / 2.0f)),
        Vec2(0.0f, 0.0f));

    Paddle PaddleTwo(
        Vec2(1280 - 50.0f, (720 / 2.0f) - (PADDLE_HEIGHT / 2.0f)),
        Vec2(0.0f, 0.0f));

    {
        bool running = true;
        bool buttons[4] = {};
        int playerOneScore = 0;
        int playerTwoScore = 0;

        float dt = 0.0f;

        SDL_Event event;

        while (running)
        {
            auto StartTime = std::chrono::high_resolution_clock::now();

            while (SDL_PollEvent(&event))
            {

                if (event.type == SDL_QUIT)
                {
                    running = false;
                }
                else if (event.type == SDL_KEYDOWN)
                {
                    if (event.key.keysym.sym == SDLK_ESCAPE)
                    {
                        running = false;
                    }
                    else if (event.key.keysym.sym == SDLK_w)
                    {
                        buttons[Buttons::PaddleOneUp] = true;
                    }
                    else if (event.key.keysym.sym == SDLK_s)
                    {
                        buttons[Buttons::PaddleOneDown] = true;
                    }
                    else if (event.key.keysym.sym == SDLK_UP)
                    {
                        buttons[Buttons::PaddleTwoUp] = true;
                    }
                    else if (event.key.keysym.sym == SDLK_DOWN)
                    {
                        buttons[Buttons::PaddleTwoDown] = true;
                    }
                }
                else if (event.type == SDL_KEYUP)
                {
                    if (event.key.keysym.sym == SDLK_w)
                    {
                        buttons[Buttons::PaddleOneUp] = false;
                    }
                    else if (event.key.keysym.sym == SDLK_s)
                    {
                        buttons[Buttons::PaddleOneDown] = false;
                    }
                    else if (event.key.keysym.sym == SDLK_UP)
                    {
                        buttons[Buttons::PaddleTwoUp] = false;
                    }
                    else if (event.key.keysym.sym == SDLK_DOWN)
                    {
                        buttons[Buttons::PaddleTwoDown] = false;
                    }
                }

                if (buttons[Buttons::PaddleOneUp])
                {
                    PaddleOne.velocity.y = -PaddleSpeed;
                }
                else if (buttons[Buttons::PaddleOneDown])
                {
                    PaddleOne.velocity.y = PaddleSpeed;
                }
                else
                {
                    PaddleOne.velocity.y = 0.0f;
                }

                if (buttons[Buttons::PaddleTwoUp])
                {
                    PaddleTwo.velocity.y = -PaddleSpeed;
                }
                else if (buttons[Buttons::PaddleTwoDown])
                {
                    PaddleTwo.velocity.y = PaddleSpeed;
                }
                else
                {
                    PaddleTwo.velocity.y = 0.0f;
                }
            }
            PaddleOne.update(dt);
            PaddleTwo.update(dt);
            ball.Update(dt);

            if (Contact contact = CheckPaddleCollision(ball, PaddleOne);
                contact.type != CollisionType::None)
            {
                ball.CollideWithPaddle(contact);
            }
            else if (Contact contact = CheckPaddleCollision(ball, PaddleTwo);
                     contact.type != CollisionType::None)
            {
                ball.CollideWithPaddle(contact);
            }
            else if (Contact contact = CheckWallCollision(ball);
                     contact.type != CollisionType::None)
            {
                ball.CollideWithWall(contact);
                //  if (contact.type == CollisionType::Left)
                //{
                //++playerTwoScore;

                // playerTwoScoreText.SetScore(playerTwoScore);
                //}
                // else if (contact.type == CollisionType::Right)
                //{
                //++playerOneScore;

                // playerOneScoreText.SetScore(playerOneScore);
                //}
            }
            SDL_SetRenderDrawColor(renderer, 0x0, 0x0, 0x0, 0xFF);
            SDL_RenderClear(renderer);

            SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

            for (int y = 0; y < 720; ++y)
            {
                if (y % 5)
                {
                    SDL_RenderDrawPoint(renderer, 1280 / 2, y);
                }
            }
            ball.Draw(renderer);
            PaddleOne.draw(renderer);
            PaddleTwo.draw(renderer);
            SDL_RenderPresent(renderer);

            auto StopTime = std::chrono::high_resolution_clock::now();
            dt = std::chrono::duration<float, std::chrono::milliseconds::period>(StopTime - StartTime).count();
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    // TTF_CloseFont(scoreFont);
    // TTF_QUIT;
    SDL_Quit();

}

