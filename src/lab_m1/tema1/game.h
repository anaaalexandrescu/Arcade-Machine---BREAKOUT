#pragma once

#include "components/simple_scene.h"
#include "lab_m1/tema1/render.h"

using namespace m2;

namespace m1
{
    class game
    {
    public:
        game();
        ~game();

        void Init(bool ship[ROWS][COLS], render* rendererPtr);
        void Update(float deltaTimeSeconds);
        void OnKeyPress(int key);
        void Reset();

        void InitBricks();
        void UpdatePaddle(float deltaTimeSeconds);
        void UpdateBall(float deltaTimeSeconds);
        void Collisions();
        void BallBricks();
        void BallPaddle();
        void BallWalls();
        void LaunchBall();
        void ResetBall();
        bool Destroyed();

        void UpdateBricks(float deltaTimeSeconds);

        bool Over() const;
        bool Won() const;

        int GetScore() const;
        int GetLives() const;

        render* renderer;

        struct Paddle {
            float x, y;
            float width, height;
            float speed;
        };

        struct Ball {
            float x, y;
            float radius;
            float dx, dy;
            bool moving;
        };

        struct Brick {
            float x, y;
            float width, height;
            int color;

            int hits;   
            bool broken;   
            bool destroyed;  
            float scale;
        };

        Paddle paddle;
        Ball ball;

        static const int rows = 5;
        static const int cols = 12;
        static const int max = rows * cols;

        Brick bricks[rows * cols];

        bool ship[ROWS][COLS];
        int row;

        bool left;
        bool right;

        int score;
        int lives;
        bool over;
        bool won;

        const float width = 100.0f;
        const float height = 50.0f;
        const float space = 5.0f;
        const float speed = 500.0f;
        const float paddle_speed = 700.0f;
        const float anim_speed = 2.0f;

        const Brick* GetBricks() const;
        int GetBrickCount() const;

        const Paddle& GetPaddle() const;
        const Ball& GetBall() const;
    };
}