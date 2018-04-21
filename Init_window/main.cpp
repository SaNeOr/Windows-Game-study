#include<Windows.h>
#include<time.h>
#include<iostream>
using namespace std;
//const string ProgramTitle = "Hello Windows";

const string APPTITLE = "Game Loop";
HWND window;
HDC device;
bool gameover = false; 


void DrawBitmap(const char *filename, int x, int y) {

	HBITMAP image = (HBITMAP)LoadImage(0, filename, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	BITMAP bm;
	GetObject(image, sizeof(BITMAP), &bm);

	HDC hdcImage = CreateCompatibleDC(device);
	SelectObject(hdcImage, image);

	BitBlt(
		device,
		x, y,
		bm.bmWidth, bm.bmHeight,
		hdcImage,
		0, 0,
		SRCCOPY);

	DeleteDC(hdcImage);
	DeleteObject((HBITMAP)image);

}

bool Game_Init() {

	srand(time(NULL));

	return 1;
}

void Game_Run() {
	if (gameover == true) return;

	RECT rect;
	GetClientRect(window, &rect);

	int x = rand() % (rect.right - rect.left);
	int y = rand() % (rect.bottom - rect.top);
	DrawBitmap("c.bmp", x, y);

}

void Game_End() {
	ReleaseDC(window, device);

}

//The window event callback funtion
LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	/*RECT drawRect;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message) {
	case WM_PAINT: {
		hdc = BeginPaint(hWnd, &ps);
		for (int n = 0; n < 20; n++) {
			int x = n * 20;
			int y = n * 20;
			drawRect = { x, y, x + 100, y + 20 };
			DrawText(hdc, APPTITLE.c_str(), ProgramTitle.length(), &drawRect, DT_CENTER);
		}
		EndPaint(hWnd, &ps);
	}
	break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}*/

	switch (message)
	{
	case WM_DESTROY:
		gameover = true;
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

//Helper funciont to set up the window properties
ATOM MyRegisterClass(HINSTANCE hInstance) {

	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = APPTITLE.c_str();
	wc.hIconSm = NULL;
	return RegisterClassEx(&wc);
}

//Helper function to create the windows and refresh it
bool InitInstance(HINSTANCE hInstance, int nCmdShow) {

	//create a new windsow;
	window = CreateWindow(
		APPTITLE.c_str(), //window class
		APPTITLE.c_str(), //title bar
		WS_OVERLAPPEDWINDOW,	//windows style
		CW_USEDEFAULT, CW_USEDEFAULT, // postion of window
		640,480,
		NULL,
		NULL,
		hInstance,
		NULL);

	//was there an error creating the window?
	if (window == 0) return 0;

	ShowWindow(window, nCmdShow);
	UpdateWindow(window);

	device = GetDC(window);

	return 1;
}

//Entry point for a windows program
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow) 
{
	//create the window
	MyRegisterClass(hInstance);
	if (!InitInstance(hInstance, nCmdShow)) return 0;	//windows传递给WinMain hinstance变量， 
														//InitInstance 位于程序的更下部分，
														//检查程序是否已经在运行中， 
														//然后创建主程序窗口。


	//main message loop
	MSG msg;
	//while (GetMessage(&msg, NULL, 0, 0)) { // 
	//	TranslateMessage(&msg);
	//	DispatchMessage(&msg);
	//}

	if (!Game_Init())return 0;
	//bool gameover = false;
	while (!gameover) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		Game_Run();

	}

	Game_End();
	return msg.wParam;
}
