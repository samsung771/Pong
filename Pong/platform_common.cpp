#define is_down(b)   (input->buttons[b].is_down)
#define pressed(b)   (input->buttons[b].is_down && input->buttons[b].changed)
#define released(b)  (!input->buttons[b].is_down && input->buttons[b].changed)

struct Button_State
{
	bool is_down;
	bool changed;
};

enum {
	BUTTON_UP,
	BUTTON_DOWN,
	BUTTON_LEFT,
	BUTTON_RIGHT,
	BUTTON_COUNT
};

struct Input
{
	Button_State buttons[BUTTON_COUNT];
};