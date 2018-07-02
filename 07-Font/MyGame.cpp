// Beginning Game Programming, Third Edition
// MyGame.cpp

#include "MyDirectX.h"
#include<string>
using namespace std;

const string APPTITLE = "Gal Game";
const int SCREENW = 1024;
const int SCREENH = 768;
static int SCORE = 0;
//delcare some fount objrects

struct Bucket : SPRITE{
};


struct Bomb :SPRITE{
	Bomb() :SPRITE() {
		reset();
	};
	void reset() {
		x = rand() % SCREENW;
		y = 0;
	}
};

Bucket bucket;
Bomb bomb;
LPDIRECT3DTEXTURE9 BUCKET = NULL;
LPDIRECT3DTEXTURE9 BOMB = NULL;

LPD3DXFONT ARIAL32 = NULL;
bool Game_Init(HWND window)
{
    //initialize Direct3D
    Direct3D_Init(window, SCREENW, SCREENH, false);

    //initialize DirectInput
    DirectInput_Init(window);

	//initialize character
	BUCKET = LoadTexture("bucket.bmp");
	BOMB = LoadTexture("bomb.bmp");
	bucket.width = bucket.height = 128;
	bomb.width = bomb.height = 128;
	bucket.y = SCREENH - 128;
	bucket.x = SCREENW / 2;

	bomb.reset();
	

	//initialize font
	ARIAL32 = MakeFont("arial", 32);
    return true;
}

#define CHARA(c,texture)do{\
							Sprite_Animate(c.frame, c.startframe, c.endframe, c.direction, c.starttime, c.delay);\
							Sprite_Transform_Draw(texture, c.x, c.y, c.width, c.height);\
						}while(0)


void Game_Run(HWND window)
{
    //make sure the Direct3D device is valid
    if (!d3ddev) return;

    //update input devices
    DirectInput_Update();


	//ues keyboard to control bucket
	if (Key_Down(DIK_LEFT)) {
		bucket.x -= 0.1f;
	}

	if (Key_Down(DIK_RIGHT)) {
		bucket.x += 0.1f;
	}

	//bucket boundary test
	if (bucket.x <= 0) bucket.x = 0;
	if (bucket.x >= SCREENW - 128) bucket.x = SCREENW - 128;

	//if caught bomb
	if (CollisionD(bucket, bomb)) {
		SCORE += 10;
		bomb.reset();
	}
	//if not caught
	if (bomb.y - 128 >= SCREENH) {
		bomb.reset();
		SCORE -= 50;
	}

	//bomb drop
	bomb.y += 0.1f;
	

    //clear the scene
    d3ddev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,100), 1.0f, 0);

    //start rendering
    if (d3ddev->BeginScene())
    {
        //*** insert sprite code here ***

		spriteobj->Begin(D3DXSPRITE_ALPHABLEND);
		

		//CHARA(bomb, BOMB);
		CHARA(bucket, BUCKET);

		string score = "SCORE: " + to_string(SCORE);
		FontPrint(ARIAL32, SCREENH, 30, score.c_str());

		spriteobj->End();
        //stop rendering
        d3ddev->EndScene();
        d3ddev->Present(NULL, NULL, NULL, NULL);
    }

    //exit when escape key is pressed
    if (KEY_DOWN(VK_ESCAPE)) gameover = true;

    //controller Back button also ends
    if (controllers[0].wButtons & XINPUT_GAMEPAD_BACK)
        gameover = true;

}

void Game_End()
{
    //free memory and shut down

    DirectInput_Shutdown();
    Direct3D_Shutdown();
}