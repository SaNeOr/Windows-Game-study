// Beginning Game Programming
// MyGame.cpp

#include "MyDirectX.h"

const string APPTITLE = "Gal Game";
const int SCREENW = 1024;
const int SCREENH = 768;

LPDIRECT3DTEXTURE9 paladin = NULL;
D3DCOLOR color = D3DCOLOR_XRGB(255,255,255);
int frame = 0, colums, width, height;
int startframe, endframe, starttime = 0, delay;
float p_x = 300, p_y = 250;

bool Game_Init(HWND window)
{
    //initialize Direct3D
    Direct3D_Init(window, SCREENW, SCREENH, false);

    //initialize DirectInput
    DirectInput_Init(window);
	
	//load explosion sprite
	paladin = LoadTexture("paladin_walk.png");

	colums = 8;
	width = height = 96;
	delay = 100;

    return true;
}

void Game_Run(HWND window)
{

	//make sure the Direct3D device is valid
	if (!d3ddev) return;

	//update input devices
	DirectInput_Update();

	startframe = endframe;
	if (Key_Down(DIK_LEFT)){
		p_x -= 0.1f;
		startframe = 48;
		endframe = 55;
	}

	if (Key_Down(DIK_RIGHT)){
		p_x += 0.1f;
		startframe = 16;
		endframe = 23;
	}
	if (Key_Down(DIK_UP)){
		p_y -= 0.1f;
		startframe = 0;
		endframe = 7;
	}

	if (Key_Down(DIK_DOWN)){
		p_y += 0.1f;
		startframe = 32;
		endframe = 39;
	}

    //clear the scene
    d3ddev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,100), 1.0f, 0);

    //start rendering
    if (d3ddev->BeginScene())
    {
		
        //*** insert sprite code here ***
		//start drawing
		spriteobj->Begin(D3DXSPRITE_ALPHABLEND);

		//set animation properties
		
		Sprite_Animate(frame, startframe, endframe, 1, starttime, delay);
		Sprite_Transform_Draw(paladin, p_x, p_y, width, height, frame, colums);

		//stop drawing
		spriteobj->End();

        //stop rendering
        d3ddev->EndScene();
        d3ddev->Present(NULL, NULL, NULL, NULL);
    }



    //F1 key ends program
    if (Key_Down(DIK_F1))
        gameover = true;

    //controller Back button also ends
    if (controllers[0].wButtons & XINPUT_GAMEPAD_BACK)
        gameover = true;

}

void Game_End()
{
    //free memory and shut down
	paladin->Release();
    DirectInput_Shutdown();
    Direct3D_Shutdown();
}