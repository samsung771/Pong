#include <vector>

class Rect 
{
private:
	//static std::vector<Rect*> objList;
public:
	float pos[2] = { 0,0 };
	float hWidth = 0;
	float hHeight = 0;
	u32 colour = 0;
	bool visible = true;
	Rect(float x, float y, float width, float height, u32 colour)
	{
		//objList.push_back(this);
		pos[0] = x; pos[1] = y;
		hWidth = width / 2;
		hHeight = height / 2;
		this->colour = colour;
	}
	void drawRect()
	{
		if (visible) draw_rect(pos[0], pos[1], hWidth, hHeight, colour);
	}
	/*static void updateRects()
	{
		for (int i = 0; i < objList.size(); i++)
		{
			objList[i]->drawRect();
		}
	}*/
};


class MoveController
{
	public:
		Rect* object;
		float velocity[2] = { 0,0 };
		float acceleration[2] = { 0,0 };
		float friction[2] = {10,10};
		float bounce = 0;
		MoveController(Rect* object)
		{
			this->object = object;
		}

		void updateObj(float dt)
		{
			acceleration[0] -= velocity[0] * friction[0];
			object->pos[0] += (velocity[0] * dt) + (acceleration[0] * dt * dt) * 0.5f;
			velocity[0] = velocity[0] + (acceleration[0] * dt); 

			acceleration[1] -= velocity[1] * friction[1];
			object->pos[1] += (velocity[1] * dt) + (acceleration[1] * dt * dt) * 0.5f;
			velocity[1] = velocity[1] + (acceleration[1] * dt);

			acceleration[0] = 0; acceleration[1] = 0;
		}

		void testCollision(MoveController& Collider, void (*command)(MoveController*, MoveController*, float, int) = 
														[](MoveController* cont, MoveController* cont2, float limit, int direction)
														{
															cont->object->pos[direction] = limit;
															cont->velocity[direction] *= cont->bounce;
														})
		{
			//right side
			if ((object->pos[0] - object->hWidth < Collider.object->pos[0] + Collider.object->hWidth   &&
				object->pos[0] + object->hWidth > Collider.object->pos[0] - Collider.object->hWidth + 1) &&
				(object->pos[1] > Collider.object->pos[1] - Collider.object->hHeight &&
					object->pos[1] < Collider.object->pos[1] + Collider.object->hHeight))
			{
				command(this, &Collider, Collider.object->pos[0] + Collider.object->hWidth + object->hWidth, 0);
			}
			//left side
			if ((object->pos[0] < Collider.object->pos[0] + Collider.object->hWidth + object->hWidth - 1 &&
				object->pos[0] > Collider.object->pos[0] - Collider.object->hWidth - object->hWidth) &&
				(object->pos[1] > Collider.object->pos[1] - Collider.object->hHeight &&
				 object->pos[1] < Collider.object->pos[1] + Collider.object->hHeight))
			{
				command(this, &Collider, Collider.object->pos[0] - Collider.object->hWidth - object->hWidth, 0);
			}
			//bottom side
			if (object->pos[1] + object->hHeight > Collider.object->pos[1] - Collider.object->hHeight &&
				object->pos[1] - object->hHeight < Collider.object->pos[1] + Collider.object->hHeight - 1 &&
				(object->pos[0] + object->hWidth > Collider.object->pos[0] - Collider.object->hWidth + 1 &&
				 object->pos[0] - object->hWidth < Collider.object->pos[0] + Collider.object->hWidth - 1))
			{
				command(this, &Collider, Collider.object->pos[1] - Collider.object->hHeight - object->hHeight,1);
			}
			//top side
			if (object->pos[1] - object->hHeight < Collider.object->pos[1] + Collider.object->hHeight &&
				object->pos[1] - object->hHeight > Collider.object->pos[1] - Collider.object->hHeight + 1 &&
				(object->pos[0] + object->hWidth > Collider.object->pos[0] - Collider.object->hWidth + 1 &&
					object->pos[0] - object->hWidth < Collider.object->pos[0] + Collider.object->hWidth - 1))
			{
				command(this, &Collider, Collider.object->pos[1] + Collider.object->hHeight + object->hHeight, 1);
			}
		}
}; 