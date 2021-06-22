#include <vector>
#pragma once

#ifndef GAME_FUNC
#define GAME_FUNC
class Rect
{
private:
	static std::vector<Rect*> objList;
public:
	float pos[2] = { 0,0 };
	float hWidth = 0;
	float hHeight = 0;
	u32 colour = 0;
	bool visible = true;
	Rect(float x, float y, float width, float height, u32 colour);

	void drawRect();

	static void updateRects()
	{
		for (int i = 0; i < objList.size(); i++)
		{
			objList[i]->drawRect();
		}
	}
};


class MoveController
{
public:
	Rect* object;
	float velocity[2] = { 0,0 };
	float acceleration[2] = { 0,0 };
	float friction[2] = { 10,10 };
	float bounce = 0;
	MoveController(Rect* object);

	void updateObj(float dt);

	void testCollision(MoveController& Collider, void (*command)(MoveController*, MoveController*, float, int) =
		[](MoveController* cont, MoveController* cont2, float limit, int direction)
		{
			cont->object->pos[direction] = limit;
			cont->velocity[direction] *= cont->bounce;
		});
};
#endif
