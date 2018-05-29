#include "MyDirectX.h"
using namespace std;
bool gameover = false;

//Windows event handler

//DefWindowProc是默认的窗口处理函数， 当遇到WM_CLOSE消息时， 将会返回WM_DESTROY消息， 它本身时不处理WM_DESTROY， 因此需要手动处理WM_DESTORY消息
LRESULT CALLBACK WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
	case WM_DESTROY:
		gameover = true;
		break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}


//Windows enrty point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//initialize window settings

	WNDCLASSEX wc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	//HGDIOBJ 和 HBRUSH定义时一样的， HBRUSH是具体的句柄
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);	//why first para is null?
	wc.hIcon = NULL;
	wc.hIconSm = NULL;
	wc.hInstance = hInstance;
	wc.lpfnWndProc = WinProc;
	wc.lpszClassName = "MainWindowClass";	//lpsz: long pointer string(s z)以null结尾的string z: zero LPCWSTR:const wchar_t*;
	wc.lpszMenuName = NULL;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClassEx(&wc);

	//create a new window
	HWND window = CreateWindow(
		"MainWindowClass", 
		APPTITLE.c_str(),
		WS_OVERLAPPEDWINDOW, 
		CW_USEDEFAULT, CW_USEDEFAULT,
		SCREENW, SCREENH, 
		NULL, NULL, hInstance, NULL);

	if (window == 0) return 0;

	//display the window
	ShowWindow(window, nCmdShow);
	UpdateWindow(window);

	//initialize the game
	if (!Game_Init(window)) return 0;

	//main message loop
	MSG message;
	while (!gameover) {
		if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}

		//process game loop
		Game_Run(window);

	}
	//shutdown
	Game_End();
	return message.wParam;
}


