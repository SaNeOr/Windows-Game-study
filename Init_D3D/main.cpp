#include<Windows.h>
#include<time.h>
#include<iostream>
#include<d3d9.h>

using namespace std;
#pragma comment(lib, "d3d9.lib")


//program setting
const string APPTITLE = "D3D_Wnd";
const int SCREENW = 1024;
const int SCREENH = 768;

//Direct3D objects
LPDIRECT3D9 d3d = NULL;
LPDIRECT3DDEVICE9 d3ddev = NULL;
bool gameover = false;
//HWND window;
//HDC device;

//macro to detect key presses
#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1:0)


//Game initialization function
bool Game_Init(HWND hwnd) {

	//initialize Direct3D
	d3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (d3d == NULL) {
		MessageBox(hwnd, "Error initializing Direct3D", "Error", MB_OK);
		return FALSE;
	}
	//set Direct3D presentation parameters

	//windowed
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;

	//full screen
	//D3DDISPLAYMODE dm;
	//d3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &dm);
	//D3DPRESENT_PARAMETERS  d3dpp;
	//ZeroMemory(&d3dpp, sizeof(d3dpp));
	//d3dpp.Windowed = FALSE;
	//d3dpp.BackBufferFormat = dm.Format;
	//d3dpp.BackBufferHeight = dm.Height;
	//d3dpp.BackBufferWidth = dm.Width;
	//d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;


	//create Direct3D device
	d3d->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hwnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		&d3ddev);
	if (d3ddev == NULL) {
		MessageBox(hwnd, "Error creating Direct3D", "Error", MB_OK);
		return FALSE;
	}

	return true;
}

//Game Update function
void Game_Run(HWND hwnd) {

	//make sure the Direct3D device is valid
	if (!d3ddev) return;
	//clear the backbuffer to bright green
	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 255, 255), 1.0f, 0);

	//start rendering
	if (d3ddev->BeginScene()) {
		//do sm?

		//stop rendering
		d3ddev->EndScene();

		//copy back buffer to the frame buffer
		d3ddev->Present(NULL, NULL, NULL, NULL);
	}

	//check for escape key(to exit program)
	if (KEY_DOWN(VK_ESCAPE))
		PostMessage(hwnd, WM_DESTROY, 0, 0);
}

//Game shutdown function
void Game_End(HWND hwnd) {
	if (d3ddev) {
		d3ddev->Release();
		d3ddev = NULL;
	}
	if (d3d) {
		d3d->Release();
		d3d = NULL;
	}
}

//The window event callback funtion
LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

	switch (message)
	{
	case WM_DESTROY:
		gameover = true;
		break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

//Entry point for a windows program
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow) 
{

	WNDCLASSEX wc;
	MSG msg;

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = NULL;
	wc.hIconSm = NULL;
	wc.lpszMenuName = NULL;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszClassName = APPTITLE.c_str();

	if (!RegisterClassEx(&wc))
		return FALSE;

	//windowed
	HWND hwnd = CreateWindow(APPTITLE.c_str(), APPTITLE.c_str(), WS_OVERLAPPEDWINDOW, 30, 50, SCREENW, SCREENH, NULL, NULL, hInstance, NULL);
	//full screen
	//HWND hwnd = CreateWindow(APPTITLE.c_str(), APPTITLE.c_str(), WS_EX_TOPMOST | WS_POPUP, 30, 50, SCREENW, SCREENH, NULL, NULL, hInstance, NULL);
	//CreateWindow()
	if (hwnd == 0) return 0;

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	if (!Game_Init(hwnd))return 0;
	//bool gameover = false;
	while (!gameover) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		Game_Run(hwnd);

	}

	Game_End(hwnd);
	return msg.wParam;
}
