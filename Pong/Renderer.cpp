#include "Utils.cpp"
#include <string>

static void render_frame(u32 colour)
{
	unsigned int* pixel = (unsigned int*)render_state.memory;
	for (int y = 0; y < render_state.height; y++)
	{
		for (int x = 0; x < render_state.width; x++)
		{
			*pixel++ = colour;
		}
	}
}

static void draw_rect_pix(int x0, int y0, int x1, int y1, u32 colour)
{
	x0 = clamp(0, x0, render_state.width);
	y0 = clamp(0, y0, render_state.height);
	x1 = clamp(0, x1, render_state.width);
	y1 = clamp(0, y1, render_state.height);

	u32* pixel;
	for (int y = y0; y < y1; y++)
	{
		pixel = (u32*)render_state.memory + x0 + y*render_state.width;
		for (int x = x0; x < x1; x++)
		{
			*pixel++ = colour;
		}
	}
}

static void draw_line_pix(int x1, int y1, int x2, int y2, u32 colour) 
{
	//code addapted from olc's console game engine
	int x, y, dx, dy, dx1, dy1, px, py, xe, ye, i;
	dx = x2 - x1; dy = y2 - y1;
	dx1 = abs(dx); dy1 = abs(dy);
	px = 2 * dy1 - dx1;	py = 2 * dx1 - dy1;
	u32* pixel;
	if (dy1 <= dx1)
	{
		if (dx >= 0)
		{
			x = x1; y = y1; xe = x2;
		}
		else
		{
			x = x2; y = y2; xe = x1;
		}

		if (y > 0 && y < render_state.height && x > 0 && x < render_state.width) {
			pixel = (u32*)render_state.memory + (int)x + y * render_state.width;
			*pixel = colour;
		}

		for (i = 0; x < xe; i++)
		{
			x = x + 1;
			if (px < 0)
				px = px + 2 * dy1;
			else
			{
				if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) y = y + 1; else y = y - 1;
				px = px + 2 * (dy1 - dx1);
			}
			if (y > 0 && y < render_state.height && x > 0 && x < render_state.width) {
				pixel = (u32*)render_state.memory + (int)x + y * render_state.width;
				*pixel = colour;
			}
		}
	}
	else
	{
		if (dy >= 0)
		{
			x = x1; y = y1; ye = y2;
		}
		else
		{
			x = x2; y = y2; ye = y1;
		}

		if (y > 0 && y < render_state.height && x > 0 && x < render_state.width) {
			pixel = (u32*)render_state.memory + (int)x + y * render_state.width;
			*pixel = colour;
		}

		for (i = 0; y < ye; i++)
		{
			y = y + 1;
			if (py <= 0)
				py = py + 2 * dx1;
			else
			{
				if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) x = x + 1; else x = x - 1;
				py = py + 2 * (dx1 - dy1);
			}
			if (y > 0 && y < render_state.height && x > 0 && x < render_state.width) {
				pixel = (u32*)render_state.memory + (int)x + y * render_state.width;
				*pixel = colour;
			}
		}
	}
}

static void draw_rect(float x, float y, float half_size_x, float half_size_y, u32 colour)
{
	x *= render_state.height * 0.01;
	y *= render_state.height * 0.01;
	half_size_x *= render_state.height * 0.01;
	half_size_y *= render_state.height * 0.01;

	x += render_state.width / 2;
	y += render_state.height / 2;

	int x0 = x - half_size_x;
	int y0 = y - half_size_y;
	int x1 = x + half_size_x;
	int y1 = y + half_size_y;

	draw_rect_pix(x0, y0, x1, y1, colour);
}

static void draw_line(float x0, float y0, float x1, float y1, u32 colour)
{
	x0 = (x0 / 1000) * render_state.width;
	y0 = (y0 / 500) * render_state.height;

	x1 = (x1 / 1000) * render_state.width;
	y1 = (y1 / 500) * render_state.height;

	draw_line_pix(x0, y0, x1, y1, colour);
}

static void draw_number(int number, float x, float y, float size, u32 color) {
	float half_size = size * .5f;

	do {
		int digit = number % 10;
		number = number / 10;

		switch (digit) {
		case 0: {
			draw_rect(x - size, y, half_size, 2.5f * size, color);
			draw_rect(x + size, y, half_size, 2.5f * size, color);
			draw_rect(x, y + size * 2.f, half_size, half_size, color);
			draw_rect(x, y - size * 2.f, half_size, half_size, color);
			x -= size * 4.f;
		} break;

		case 1: {
			draw_rect(x + size, y, half_size, 2.5f * size, color);
			x -= size * 2.f;
		} break;

		case 2: {
			draw_rect(x, y + size * 2.f, 1.5f * size, half_size, color);
			draw_rect(x, y, 1.5f * size, half_size, color);
			draw_rect(x, y - size * 2.f, 1.5f * size, half_size, color);
			draw_rect(x + size, y + size, half_size, half_size, color);
			draw_rect(x - size, y - size, half_size, half_size, color);
			x -= size * 4.f;
		} break;

		case 3: {
			draw_rect(x - half_size, y + size * 2.f, size, half_size, color);
			draw_rect(x - half_size, y, size, half_size, color);
			draw_rect(x - half_size, y - size * 2.f, size, half_size, color);
			draw_rect(x + size, y, half_size, 2.5f * size, color);
			x -= size * 4.f;
		} break;

		case 4: {
			draw_rect(x + size, y, half_size, 2.5f * size, color);
			draw_rect(x - size, y + size, half_size, 1.5f * size, color);
			draw_rect(x, y, half_size, half_size, color);
			x -= size * 4.f;
		} break;

		case 5: {
			draw_rect(x, y + size * 2.f, 1.5f * size, half_size, color);
			draw_rect(x, y, 1.5f * size, half_size, color);
			draw_rect(x, y - size * 2.f, 1.5f * size, half_size, color);
			draw_rect(x - size, y + size, half_size, half_size, color);
			draw_rect(x + size, y - size, half_size, half_size, color);
			x -= size * 4.f;
		} break;

		case 6: {
			draw_rect(x + half_size, y + size * 2.f, size, half_size, color);
			draw_rect(x + half_size, y, size, half_size, color);
			draw_rect(x + half_size, y - size * 2.f, size, half_size, color);
			draw_rect(x - size, y, half_size, 2.5f * size, color);
			draw_rect(x + size, y - size, half_size, half_size, color);
			x -= size * 4.f;
		} break;

		case 7: {
			draw_rect(x + size, y, half_size, 2.5f * size, color);
			draw_rect(x - half_size, y + size * 2.f, size, half_size, color);
			x -= size * 4.f;
		} break;

		case 8: {
			draw_rect(x - size, y, half_size, 2.5f * size, color);
			draw_rect(x + size, y, half_size, 2.5f * size, color);
			draw_rect(x, y + size * 2.f, half_size, half_size, color);
			draw_rect(x, y - size * 2.f, half_size, half_size, color);
			draw_rect(x, y, half_size, half_size, color);
			x -= size * 4.f;
		} break;

		case 9: {
			draw_rect(x - half_size, y + size * 2.f, size, half_size, color);
			draw_rect(x - half_size, y, size, half_size, color);
			draw_rect(x - half_size, y - size * 2.f, size, half_size, color);
			draw_rect(x + size, y, half_size, 2.5f * size, color);
			draw_rect(x - size, y + size, half_size, half_size, color);
			x -= size * 4.f;
		} break;
		}

	} while (number);
}