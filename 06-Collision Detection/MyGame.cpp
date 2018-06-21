// Beginning Game Programming
// MyGame.cpp

#include "MyDirectX.h"
using namespace std;

const string APPTITLE = "Gal Game";
const int SCREENW = 1024;
const int SCREENH = 768;

SPRITE ship, asteroid1, asteroid2;
LPDIRECT3DTEXTURE9 imgShip = NULL;
LPDIRECT3DTEXTURE9 imgAsteroid = NULL;



#define CHECK_OK(obj)	do{\
							if(!obj)return false;\
						}while(0)
bool Game_Init(HWND window)
{
    //initialize Direct3D
    Direct3D_Init(window, SCREENW, SCREENH, false);

    //initialize DirectInput
    DirectInput_Init(window);
	
	//load the sprite texture
	imgShip = LoadTexture("fatship.tga");
	CHECK_OK(imgShip);
	imgAsteroid = LoadTexture("asteroid.tga");
	CHECK_OK(imgAsteroid);

	//set properties for sprites
	ship.x = 450;
	ship.y = 300;
	ship.width = ship.height = 128;


	asteroid1.x = 50;
	asteroid1.y = 200;

	asteroid1.columns = 8;
	asteroid1.width = asteroid1.height = 60;
	asteroid1.endframe = 63;
	//asteroid1.delay = ?
	asteroid1.velx = -0.1f; //??????????

	asteroid2 = asteroid1;
	asteroid2.x = 900;
	asteroid2.y = 500;
	asteroid2.velx = 0.1f;


	

    return true;
}

#define ANIMATE_ASTEROID(asteroid)	do{\
										asteroid.x += asteroid.velx;\
										if(asteroid.x < 0 || asteroid.x > SCREENW - asteroid.width){\
											asteroid.velx *= -1;\
											asteroid.direction *= -1;\
										}\
										Sprite_Animate(asteroid.frame,asteroid.startframe,asteroid.endframe, asteroid.direction,asteroid.starttime,asteroid.delay);\
									}while(0)

#define DRAW(img, obj)	do{\
							Sprite_Transform_Draw(img, obj.x,obj.y,obj.width,obj.height,obj.frame,obj.columns);\
						}while(0)
void Game_Run(HWND window)
{
    //make sure the Direct3D device is valid
    if (!d3ddev) return;

    //update input devices
    DirectInput_Update();

    //clear the scene
    d3ddev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,100), 1.0f, 0);

	//move the ship up/down with arrow keys
	if (Key_Down(DIK_UP)) {
		ship.y -= 0.1f;
		if (ship.y < 0) 
			ship.y = 0;
	}

	if (Key_Down(DIK_DOWN)) {
		ship.y += 0.1f;
		if (ship.y > SCREENH - ship.height) 
			ship.y = SCREENH - ship.height;
	}

	//move and animate the asteroids
	ANIMATE_ASTEROID(asteroid1);
	ANIMATE_ASTEROID(asteroid2);


	//test for collisions
	//if (Collision(ship, asteroid1))
	//	asteroid1.velx *= -1;
	//if (Collision(ship, asteroid2))
	//	asteroid2.velx *= -1;

	if (CollisionD(ship, asteroid1))
		asteroid1.velx *= -1;
	if (CollisionD(ship, asteroid2))
		asteroid2.velx *= -1;

    //start rendering
    if (d3ddev->BeginScene())
    {
        //*** insert sprite code here ***
		spriteobj->Begin(D3DXSPRITE_ALPHABLEND);
		DRAW(imgShip, ship);
		DRAW(imgAsteroid, asteroid1);
		DRAW(imgAsteroid, asteroid2);
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
	if (imgShip) imgShip->Release();
	if (imgAsteroid)imgAsteroid->Release();
    DirectInput_Shutdown();
    Direct3D_Shutdown();
}