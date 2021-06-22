#include <windows.h>

static bool running = true;

struct Render_State
{
	int width, height;
	void* memory;

	BITMAPINFO bitmap_info;
};

static Render_State render_state;

#include "Renderer.cpp"
#include "platform_common.cpp"
#include "game.cpp"

LRESULT CALLBACK window_callback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;
	switch (uMsg)
	{
		//if it gets a close or destroy msg quit while loop
		case WM_CLOSE:
		case WM_DESTROY:
			running = false;
			break;
		//if it gets a resize msg resize the bitmap
		case WM_SIZE: 
		{
			//creates a new rectangle and assigns it to the windows rectangle 
			RECT rect;
			GetClientRect(hwnd, &rect);

			//updates variables
			render_state.width = rect.right - rect.left;
			render_state.height = rect.bottom - rect.top;

			int size = render_state.width * render_state.height * sizeof(u32);

			//frees old memory and allocates it to a new place
			if (render_state.memory) VirtualFree(render_state.memory, 0, MEM_RELEASE);
			render_state.memory = VirtualAlloc(0, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

			//updates variables
			render_state.bitmap_info.bmiHeader.biSize = sizeof(render_state.bitmap_info.bmiHeader);
			render_state.bitmap_info.bmiHeader.biWidth = render_state.width;
			render_state.bitmap_info.bmiHeader.biHeight = render_state.height;
			render_state.bitmap_info.bmiHeader.biPlanes = 1;
			render_state.bitmap_info.bmiHeader.biBitCount = 32;
			render_state.bitmap_info.bmiHeader.biCompression = BI_RGB;
		}
			break;
		default:
			//make sure all messages have been processed
			result = DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return result;
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	//creates new window object
	WNDCLASS window_class = {};
	//sets style
	window_class.style = CS_OWNDC;
	//sets window name to Game Window Class
	window_class.lpszClassName = L"Game Window Class";
	window_class.lpfnWndProc = window_callback;

	//creates register class
	RegisterClass(&window_class);

	//creates window using the window class "Game Window Class", called Pong, that is visible, that is 1280*720
	HWND window = CreateWindow(
		window_class.lpszClassName, 
		L"Pong", 
		WS_OVERLAPPEDWINDOW | WS_VISIBLE, 
		CW_USEDEFAULT, 
		CW_USEDEFAULT, 
		1280, 720, 
		0, 0, 
		hInstance, 
		0
	);
	//assigns device handle
	HDC hdc = GetDC(window);

	Input input = {};

	float delta_time = 0.016666f;

	//gets the start of the frame
	LARGE_INTEGER frame_begin_time;
	QueryPerformanceCounter(&frame_begin_time);
	LARGE_INTEGER frame_end_time;

	//gets the CPU frequency
	float performance_freq;
	LARGE_INTEGER perf;
	QueryPerformanceFrequency(&perf);
	performance_freq = (float)perf.QuadPart;

	MSG message;
	while (running)
	{
		//sets all buttons changed to false
		for (int i = 0; i < BUTTON_COUNT; i++) input.buttons[i].changed = false;

		//checks messages
		while (PeekMessage(&message, window, 0, 0, PM_REMOVE))
		{
			switch (message.message)
			{
				//on key press
				case WM_KEYUP:
				case WM_KEYDOWN:
				{
					//gets key code
					u32 vk_code = (u32)message.wParam;

					bool is_down = ((message.lParam & (1 << 31)) == 0);

					switch (vk_code)
					{
						//creates a macro to process a button
						#define process_button(b, vk)\
							case vk:{\
							input.buttons[b].changed = is_down != input.buttons[b].is_down;\
							input.buttons[b].is_down = is_down;\
						}break;

						process_button(BUTTON_UP, VK_UP);
						process_button(BUTTON_LEFT, VK_LEFT);
						process_button(BUTTON_RIGHT, VK_RIGHT);
						process_button(BUTTON_DOWN, VK_DOWN);
					}

				}break;
				default:
					TranslateMessage(&message);
					DispatchMessageW(&message);
					break;
			}
		}

		update(&input, delta_time);

		//render
		StretchDIBits(
			hdc, 
			0, 0, 
			render_state.width, 
			render_state.height, 
			0, 0, 
			render_state.width,
			render_state.height,
			render_state.memory,
			&render_state.bitmap_info,
			DIB_RGB_COLORS, 
			SRCCOPY
		);
		
		//gets end time
		QueryPerformanceCounter(&frame_end_time);
		//delta time = frame time/cpu frequency
		delta_time = (float)((frame_end_time.QuadPart - frame_begin_time.QuadPart) / performance_freq);
		frame_begin_time = frame_end_time;
	}
}