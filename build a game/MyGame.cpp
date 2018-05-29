#include "MyDirectX.h"

const string APPTITLE = "LG&SW";
const int SCREENW = 1024;
const int SCREENH = 768;

LPDIRECT3DSURFACE9 bomb_surf = NULL;
LPDIRECT3DSURFACE9 bucket_surf = NULL;

struct BOMB
{
	float x, y;

	void reset()
	{
		x = (float)(rand() % (SCREENW - 128));
		y = 0;
	}

};


BOMB bomb;

struct BUCKET
{
	float x, y;
};

BUCKET bucket;

int score = 0;

bool Game_Init(HWND window)
{
    Direct3D_Init(window, SCREENW, SCREENH, false);

    DirectInput_Init(window);

    bomb_surf = LoadSurface("bomb.bmp");
    if (!bomb_surf) {
        MessageBox(window, "Error loading bomb","Error",0);
        return false;
    }

    bucket_surf = LoadSurface("bucket.bmp");
    if (!bucket_surf) {
        MessageBox(window, "Error loading bucket","Error",0);
        return false;
    }

    //position the bomb
    srand( (unsigned int)time(NULL) );
    bomb.reset();

    //position the bucket
    bucket.x = 500;
    bucket.y = 630;

    return true;
}


void Game_Run(HWND window)
{
	//make sure the D3Ddev is valid
	if (!d3ddev)return;

	//upadte input dpint
	DirectInput_Update();
	//move the bomb down the screen
	bomb.y += 2.0f;

	//see if bomb hit the floor
	char buf[50];
	sprintf(buf, "u got %d", score);
	if (bomb.y > SCREENH)
	{
		MessageBox(0,buf, "GAME OVER", 0);
		gameover = true;
	}

	//move the bucket with the keyboard
	if (Key_Down(DIK_LEFT))
		bucket.x -= 6.0f;
	if (Key_Down(DIK_RIGHT))
		bucket.x += 6.0f;


	//keep bucket inside the screen
	if (bucket.x < 0) bucket.x = 0;
	if (bucket.x > SCREENW - 128) bucket.x = SCREENW - 128;

	//see if bucket caught the bomb
	int cx = (int)bomb.x + 64;
	int cy = (int)bomb.y + 64;
	if (cx > bucket.x && cx < bucket.x + 128 && cy > bucket.y && cy < bucket.y + 128)
	{
		//update and display score
		score++;
		std::ostringstream os;
		os << APPTITLE << " [SCORE " << score << "]";
		string scoreStr = os.str();
		SetWindowText(window, scoreStr.c_str());
		//restart bomb
		bomb.reset();
	}
	//clear the backbuffer
	d3ddev->ColorFill(backbuffer, NULL, D3DCOLOR_XRGB(0, 0, 0));

	//start rendering
	if (d3ddev->BeginScene())
	{
		//draw the bomb
		DrawSurface(backbuffer, bomb.x, bomb.y, bomb_surf);

		//draw the bucket
		DrawSurface(backbuffer, bucket.x, bucket.y, bucket_surf);

		//stop rendering
		d3ddev->EndScene();
		d3ddev->Present(NULL, NULL, NULL, NULL);
	}

	//space or escape key exits
	if (Key_Down(DIK_SPACE) || Key_Down(DIK_ESCAPE))
		gameover = true;
}
void Game_End()
{
	if (bomb_surf) bomb_surf->Release();
	if (bucket_surf) bucket_surf->Release();
	DirectInput_Shutdown();
	Direct3D_Shutdown();
}
