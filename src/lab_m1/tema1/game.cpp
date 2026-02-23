#include "lab_m1/tema1/game.h"
#include <iostream>
#include <math.h> 

using namespace m1;

const int resistance[game::rows][game::cols] = {
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	{ 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 },
	{ 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3 },
	{ 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4 },
	{ 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5}
};


game::game()
{
}

game::~game()
{
}

void game::Init(bool shipPl[ROWS][COLS], render* rendererPtr)
{
	renderer = rendererPtr;
	row = -1;

	int minCol = -1;
	int maxCol = -1;
	int totalSquares = 0;
	const float SQUARE_SIZE = 50.0f;

	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLS; j++) {
			ship[i][j] = shipPl[i][j];

			if (ship[i][j]) {
				if (row == -1) {
					row = i;
				}
				if (i == row) {
					if (minCol == -1 || j < minCol) {
						minCol = j;
					}
					if (j > maxCol) {
						maxCol = j;
					}
				}
			}
		}
	}

	if (minCol != -1) {
		totalSquares = (maxCol - minCol + 1);
	}
	else {
		totalSquares = 2;
	}

	paddle.width = totalSquares * SQUARE_SIZE;
	paddle.height = 20.0f;
	paddle.x = 640.0f - paddle.width * 0.5f;
	paddle.y = 50.0f;
	paddle.speed = paddle_speed;

	ball.radius = 10.0f;
	ball.x = paddle.x + paddle.width * 0.5f;
	ball.y = paddle.y + paddle.height + ball.radius;
	ball.dx = 0.0f;
	ball.dy = 0.0f;
	ball.moving = false;

	InitBricks();

	left = false;
	right = false;
	score = 0;
	lives = 3;
	over = false;
	won = false;
}

void game::InitBricks()
{
	float x = 15.0f;
	float y = 400.0f;

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			int index = i * cols + j;
			if (index >= max) continue;

			bricks[index].x = x + j * (width + space);
			bricks[index].y = y + i * (height + space);
			bricks[index].width = width;
			bricks[index].height = height;

			int brickRes = resistance[i][j];

			bricks[index].hits = brickRes;
			bricks[index].color = brickRes;

			if (i == 3 && (j == 6 || j == 5)) {
				bricks[index].hits = 6;
				bricks[index].color = 99;
			}

			bricks[index].broken = false;
			bool r = resistance == 0;
			bricks[index].destroyed = r;
			bricks[index].scale = 1.0f;
		}
	}
}

void game::Update(float deltaTimeSeconds)
{
	if (over || won) {
		return;
	}

	UpdatePaddle(deltaTimeSeconds);
	UpdateBall(deltaTimeSeconds);
	UpdateBricks(deltaTimeSeconds);
	Collisions();

	if (Destroyed()) {
		won = true;
	}

	if (lives <= 0) {
		over = true;
	}

	glClearColor(0, 0, 0, 1);

	const game::Paddle& paddle = GetPaddle();
	renderer->Rectangle(paddle.x, paddle.y, paddle.width, paddle.height);

	const game::Ball& ball = GetBall();
	renderer->Circle(ball.x, ball.y, ball.radius);

	const game::Brick* bricks = GetBricks();
	int brickCount = GetBrickCount();

	for (int i = 0; i < brickCount; i++) {
		if (bricks[i].destroyed) {
			continue;
		}

		float scale = bricks[i].scale;

		float w = bricks[i].width * scale;
		float h = bricks[i].height * scale;

		float x = bricks[i].x + (bricks[i].width - w) * 0.5f;
		float y = bricks[i].y + (bricks[i].height - h) * 0.5f;

		renderer->Rectangle(x, y, w, h, bricks[i].color);
	}

	renderer->Score(GetScore());
	renderer->Lives(GetLives());
}

void game::UpdateBricks(float deltaTimeSeconds)
{
	for (int i = 0; i < max; i++) {
		Brick& brick = bricks[i];

		if (brick.broken && !brick.destroyed) {
			brick.scale -= deltaTimeSeconds * anim_speed;

			if (brick.scale <= 0.0f) {
				brick.scale = 0.0f;
				brick.destroyed = true;
			}
		}
	}
}

void game::UpdatePaddle(float deltaTimeSeconds)
{
	if (left) {
		paddle.x -= paddle.speed * deltaTimeSeconds;
		if (paddle.x < 0) {
			paddle.x = 0;
		}
	}

	if (right) {
		paddle.x += paddle.speed * deltaTimeSeconds;
		if (paddle.x + paddle.width > 1280.0f) {
			paddle.x = 1280.0f - paddle.width;
		}
	}

	if (!ball.moving) {
		ball.x = paddle.x + paddle.width * 0.5f;
	}
}

void game::UpdateBall(float deltaTimeSeconds)
{
	if (ball.moving == false) {
		return;
	}

	ball.x += ball.dx * deltaTimeSeconds;
	ball.y += ball.dy * deltaTimeSeconds;
}

void game::Collisions()
{
	if (ball.moving != true) {
		return;
	}

	BallWalls();
	BallPaddle();
	BallBricks();
}

void game::BallWalls()
{
	float screenW = 1280.0f;
	float screenH = 720.0f;

	if (ball.x - ball.radius <= 0) {
		ball.x = ball.radius;
		ball.dx = -ball.dx;
	}

	if (ball.y - ball.radius <= 0) {
		lives--;
		ResetBall();
	}

	if (ball.x + ball.radius >= screenW) {
		ball.x = screenW - ball.radius;
		ball.dx = -ball.dx;
	}

	if (ball.y + ball.radius >= screenH) {
		ball.y = screenH - ball.radius;
		ball.dy = -ball.dy;
	}
}

void game::BallBricks()
{
	for (int i = 0; i < max; i++) {
		Brick& brick = bricks[i];

		if (brick.destroyed || brick.broken) {
			continue;
		}

		bool intersectionX = (ball.x + ball.radius >= brick.x) && (ball.x - ball.radius <= brick.x + brick.width);
		bool intersectionY = (ball.y + ball.radius >= brick.y) && (ball.y - ball.radius <= brick.y + brick.height);

		bool collision = intersectionX && intersectionY;

		if (collision) {
			brick.hits = brick.hits - 1;

			if (brick.hits > 0) {
				if (brick.color != 99) {
					brick.color = brick.hits;
				}
				else if (brick.hits == 1) {
					brick.color = 1;
				}
			}

			if (brick.hits <= 0) {
				brick.broken = true;
				brick.hits = 0;

				int row = i / cols;
				int col = i % cols;

				int ogResistance = resistance[row][col];

				if (ogResistance == 99) {
					ogResistance = 2;
				}
				score += ogResistance;
			}

			float ballCenterX = ball.x;
			float ballCenterY = ball.y;
			float brickCenterX = brick.x + brick.width * 0.5f;
			float brickCenterY = brick.y + brick.height * 0.5f;

			float dx = ballCenterX - brickCenterX;
			float dy = ballCenterY - brickCenterY;

			float intersectionsX2 = (dx / brick.width) * (dx / brick.width);

			float intersectionsY2 = (dy / brick.height) * (dy / brick.height);

			if (intersectionsX2 > intersectionsY2) {
				ball.dx = -ball.dx;
			}
			else {
				ball.dy = -ball.dy;
			}
			break;
		}
	}
}

void game::BallPaddle()
{
	if (ball.x + ball.radius < paddle.x)
		return;

	if (ball.x - ball.radius > paddle.x + paddle.width)
		return;

	if (ball.y - ball.radius > paddle.y + paddle.height)
		return;

	if (ball.y - ball.radius < paddle.y)
		return;

	if (ball.dy < 0) {
		ball.y = paddle.y + paddle.height + ball.radius;

		float center = paddle.x + paddle.width * 0.5f;
		float angle = (ball.x - center) / (paddle.width / 0.5f);

		if (angle < -1.0f) angle = -1.0f;
		if (angle > 1.0f) angle = 1.0f;

		const float max = 60.0f * 3.14f / 180.0f;
		float rad = angle * max;

		float speed = sqrtf(ball.dx * ball.dx + ball.dy * ball.dy);

		ball.dx = speed * sinf(rad);
		ball.dy = speed * cosf(rad);

		if (ball.dy < 0) {
			ball.dy = -ball.dy;
		}
	}
}

void game::LaunchBall()
{
	if (ball.moving == false) {
		ball.moving = true;
		ball.dx = speed * 0.5f;
		ball.dy = speed * 0.8f;
	}
}

void game::ResetBall()
{
	paddle.x = 640.0f - paddle.width * 0.5f;
	paddle.y = 50.0f;

	ball.x = paddle.x + paddle.width * 0.5f;
	ball.y = paddle.y + paddle.height + ball.radius;
	ball.dx = 0.0f;
	ball.dy = 0.0f;
	ball.moving = false;
}

bool game::Destroyed()
{
	for (int i = 0; i < max; i++) {
		if (bricks[i].hits > 0) {
			return false;
		}
	}
	return true;
}

void game::OnKeyPress(int key)
{
	if (key == GLFW_KEY_SPACE) {
		LaunchBall();
	}
	else if (key == GLFW_KEY_LEFT) {
		left = true;
	}
	else if (key == GLFW_KEY_RIGHT) {
		right = true;
	}
}

void game::Reset()
{
	paddle.x = 640.0f - paddle.width * 0.5f;
	paddle.y = 50.0f;

	ball.x = paddle.x + paddle.width * 0.5f;
	ball.y = paddle.y + paddle.height + ball.radius;
	ball.dx = 0.0f;
	ball.dy = 0.0f;

	ball.moving = false;

	InitBricks();

	left = false;
	right = false;
	score = 0;
	lives = 3;
	over = false;
	won = false;
}

bool game::Over() const
{
	return over;
}

bool game::Won() const
{
	return won;
}

int game::GetScore() const
{
	return score;
}

int game::GetLives() const
{
	return lives;
}

const game::Brick* game::GetBricks() const
{
	return bricks;
}

int game::GetBrickCount() const
{
	return rows * cols;
}

const game::Paddle& game::GetPaddle() const
{
	return paddle;
}

const game::Ball& game::GetBall() const
{
	return ball;
}

