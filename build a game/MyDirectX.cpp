#include "MyDirectX.h"
#include<iostream>
using namespace std;


//Direct3D variables
LPDIRECT3D9 d3d = NULL;
LPDIRECT3DDEVICE9 d3ddev = NULL;
LPDIRECT3DSURFACE9 backbuffer = NULL;

char keys[256];

//DirectInput variablese
LPDIRECTINPUT8 dinput = NULL;
LPDIRECTINPUTDEVICE8 dikeyboard = NULL;
//TODO

//Direct3D initialization
bool Direct3D_Init(HWND window, int width, int height, bool fullscreen)
{
	//initiallize Direct3D
	d3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (!d3d) return false;

	//set Direct3D presentation paramters
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferCount = 1;
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.BackBufferHeight = height;
	d3dpp.BackBufferWidth = width;
	d3dpp.SwapEffect = D3DSWAPEFFECT_COPY;
	d3dpp.Windowed = (!fullscreen);
	d3dpp.hDeviceWindow = window;

	//create Direct3D device
	d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, window,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &d3ddev);
	if (!d3ddev) return false;


	d3ddev->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backbuffer);

	return true;

}

//Direct3D shutdown
void Direct3D_Shutdown() {
	if (d3ddev) d3ddev->Release();
	if (d3d) d3d->Release();

}

// Draws a surface to the screen using StretchRect
void DrawSurface(LPDIRECT3DSURFACE9 dest, float x, float y, LPDIRECT3DSURFACE9 source)
{
	//get w/h from source surface
	D3DSURFACE_DESC desc;
	source->GetDesc(&desc);

	//create rects for drawing
	RECT source_rect = { 0,0,(long)desc.Width,(long)desc.Height };
	RECT dest_rect = { (long)x, (long)y, (long)x + desc.Width, (long)y + desc.Height };

	//draw the source surface noto the dest 
	d3ddev->StretchRect(source, &source_rect, dest, &dest_rect, D3DTEXF_NONE);
}

//Loads a bitmap file into a surface
LPDIRECT3DSURFACE9 LoadSurface(string filename)
{

	LPDIRECT3DSURFACE9 image = NULL;

	//get width and height from bitmap file
	D3DXIMAGE_INFO info;
	HRESULT result = D3DXGetImageInfoFromFile(filename.c_str(), &info);
	if (result != D3D_OK)
		return NULL;

	//create surface
	result = d3ddev->CreateOffscreenPlainSurface(
		info.Width,
		info.Height,
		D3DFMT_X8R8G8B8,
		D3DPOOL_DEFAULT,
		&image,
		NULL);

	if (result != D3D_OK) return NULL;

	//load surface from file into newly created surface

	result = D3DXLoadSurfaceFromFile(
		image,
		NULL,
		NULL,
		filename.c_str(),
		NULL,
		D3DX_DEFAULT,
		D3DCOLOR_XRGB(0, 0, 0),
		NULL);

	if (result != D3D_OK) return NULL;

	return image;

}

bool DirectInput_Init(HWND hwnd)
{
	//initialize DirectInput object
	HRESULT result = DirectInput8Create(
		GetModuleHandle(NULL),
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&dinput,
		NULL);

	//initialize the keyboard
	dinput->CreateDevice(GUID_SysKeyboard, &dikeyboard, NULL);
	dikeyboard->SetDataFormat(&c_dfDIKeyboard);
	dikeyboard->SetCooperativeLevel(hwnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	dikeyboard->Acquire();

	//initialize the mouse
	// TODO

	return true;
}

void DirectInput_Update()
{
	//update mouse
	//dimouse->GetDeviceState(sizeof(mouse_state),(LPVOID)&mouse_state);

	//update keyboard
	dikeyboard->GetDeviceState(sizeof(keys), (LPVOID)&keys);

}

int Key_Down(int key)
{
	return (keys[key] & 0x80);
}

void DirectInput_Shutdown()
{
	if (dikeyboard)
	{
		dikeyboard->Unacquire();
		dikeyboard->Release();
		dikeyboard = NULL;
	}
}
