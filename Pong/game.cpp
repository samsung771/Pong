#include <cmath>
#include <stdlib.h>
#include <windows.h>
#include <time.h>

#include "gameFunc.cpp"

#define AIon 1

u32 score[2] = { 0,0 };

//creates rectangle objects
Rect top(0, 47, 200, 7, 0x101010);
Rect bottom(0, -47, 200, 7, 0x101010);

Rect ball(0, 0, 4, 4, 0xf0f0ff);

Rect player1(80, 0, 4, 20, 0xf0f0ff);
Rect player2(-80, 0, 4, 20, 0xf0f0ff);

//creates controllers for objects
MoveController player1C(&player1);
MoveController player2C(&player2);
MoveController ballC(&ball);

//used for collisions
MoveController topC(&top);
MoveController bottomC(&bottom);

static int init()
{
	srand(time(NULL));
	ballC.velocity[0] = 125; ballC.velocity[1] = rand() % 100 - 50;
	ballC.friction[0] = 0; ballC.friction[1] = 0;
	ballC.bounce = -1;
	return NULL;
}

int place = init();

static void update(Input* input, float dt)
{
	render_frame(0x141414);

	player1.drawRect();
	player2.drawRect();
	ball.drawRect();
	top.drawRect();
	bottom.drawRect();

	draw_number(score[0], -5, 42, 1.5, 0xffffff);
	draw_number(score[1], 5, 42, 1.5, 0xffffff);

	draw_line(10, 10, 300, 500, 0xFFFFFF);
	
	draw_rect(0, 42, 1, 6, 0xf0f0ff);

	if (is_down(BUTTON_UP)) player1C.acceleration[1] += 1000;
	if (is_down(BUTTON_DOWN)) player1C.acceleration[1] -= 1000;

#if !AIon
	if (is_down(BUTTON_LEFT)) player2C.acceleration[1] += 1000;
	if (is_down(BUTTON_RIGHT)) player2C.acceleration[1] -= 1000;
#else
	if (ball.pos[1] + 5 > player2.pos[1]) player2C.acceleration[1] += 500;
	if (ball.pos[1] - 5 < player2.pos[1]) player2C.acceleration[1] -= 500;
#endif

	player1C.testCollision(topC);
	player1C.testCollision(bottomC);
	player2C.testCollision(topC);
	player2C.testCollision(bottomC);

	player1C.updateObj(dt);
	player2C.updateObj(dt);
	ballC.updateObj(dt);
	
	ballC.testCollision(topC);
	ballC.testCollision(bottomC);

	//ball collision with players
	ballC.testCollision(player1C, [](MoveController* cont, MoveController* cont2, float limit, int direction) {
									cont->object->pos[direction] = limit;
									cont->velocity[direction] *= cont->bounce;
									cont->velocity[1] = ((cont->object->pos[1] - cont2->object->pos[1]) * 2 + cont2->velocity[1] * 0.75);
									});
	ballC.testCollision(player2C, [](MoveController* cont, MoveController* cont2, float limit, int direction) {
									cont->object->pos[direction] = limit;
									cont->velocity[direction] *= cont->bounce;
									cont->velocity[1] = ((cont->object->pos[1] - cont2->object->pos[1]) * 2 + cont2->velocity[1] * 0.75);
									});

	#define resetBall()\
	ball.pos[0] = 0;\
	ball.pos[1] = 0;\
	ballC.velocity[0] *= -1; ballC.velocity[1] = rand() % 100 - 50;

	//goes of the screen
	if (ball.pos[0] > 100)
	{
		score[0]++;
		resetBall();
	}else if (ball.pos[0] < -100)
	{
		score[1]++; 
		resetBall();
	}
}