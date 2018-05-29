# 建立一个完整的游戏
>万事俱备，只差...

以一个简单的接炸弹的游戏为例，一般游戏分成三个部分

0. 定义角色
1. Game Init
2. Game Run
3. Game End

## 定义角色
对于接炸弹游戏而言，主要有两个角色:炸弹&接炸弹的。我们构想一下游戏: 一个从空而降的炸弹， 一个在底下接炸弹的篮子，炸弹一旦被接到就会又有一个新的掉下来，如果没被接到，就gg。也就是说到底，都是位置的变化。

```
struct BOMB{
    float x,y;
    void reset(){
        x = (float)(rand()%(SCREENW - 128)); //坐标点实际指的是图片的左上角
        y = 0;        
    }
};

struct BUCKET{
    float x,y;
};

int score = 0; //记分板
LPDIRECT3DSURFACE9 bomb_surf = NULL;
LPDIRECT3DSURFACE9 bucket_surf = NULL;
```

## Game Init
游戏初始化是在游戏运行之前执行一次的，主要完成也是加载d3d、d3dinput、角色位图的作用。 从代码上来看，游戏初始化的函数位于WinMain循环之外(不难理解为啥)。

```
bool Game(HWND window){
    Direct3D_Init(window,SCREENW, SCREENH, false);
    DirectInput_Init(window);

    bomb_surf = LoadSurface("bomb.bmp");
    //if load fail return false;
    bucket_surf = LoadSurface("bomb.bom");
    //if load fail return false;

    //position the bomb
    srand((unsigned int) time(NULL));
    bomb.reset();

    //position the bucket
    bucket.x = 500;
    bucket.y = 630;

    return true;
}
```
## Game Run
游戏运行的内容定义了游戏里的规则，也是最核心的，它是不停的运行的。它便是WinMain主循环里的函数。
```
void Game_Run(HWND window){
    //make sure the d3ddev 
    if(!d3ddev) return;

    //update input dinput
    DirectInput_Update();
    //move the bomb donw screen
    bomb.y += 2.0f;

    //move the bucket with the keyboard;
    // if..else if 意味着如果同时按下， 将会执行if,  此时表现在 bucket会往左移动
    // if...if.. 则会执行两个if 此时，表现在bucket原地不动
    if(Key_Down(DIK_LEFT)) bucket.x -= 6.0f;
    else if(Key_Down(DIK_RIGHT)) bucket.x += 6.0f;

    //keep bucket inside the screen
    if(bucket.x < 9) bucket.x =0 ;
    if(bucket.x > SCREENW - 128) bucket.x = SCREENW - 128;

    //see if bomb hit the floor
    char buf[50];
    sprintf(buf,"u got %d scores",score);
    if(bomb.y > SCREENH){
        MessageBox(0,buf,"Game OVER",0);
        gameover = true;
    }

    //see if bucket caught the bomb
    int cx = (int)bomb.x + 64;
    int cy = (int)bomb.y + 64;
    if(cx > bucket.x && cx < bucket.x +128 && cy > bucket.y &&cy  < bucket.y + 128){
        //update and display score;
        score++;
        std::ostringstrean os;
        os<< APPTITLE << "[SCORE" << score << "]";
        string scoreStr = os.str();
        SetWindowText(window,scoreStr.c_str());

        //restart bomb
        bomb.reset();
    }

    //clear the backbuffer
    d3ddev->ColorFil(backbuffer, NULL, D3DCOLOR_XRGB(0,0,0));

    //start renderding
    if(d3ddev->BeginScene()){
        //draw the bomb
        DrawSurface(buckbuffer, bomb.x, bomb,y, bomb_surf);

        //draw the bucket
        DrawSurface(buckbuffer, bucket.x, bucket.y, bucket_surf);

        //stop rendering;
        d3ddev->EndScene();
        d3ddev->Present(NULL,NULL,NULL,NULL);

    }

        //space or escpace key exits
        if(Key_Down(DIK_SPACE) || Key_Down(DIK_ESCAPE))
            gameober = true;
}
```

## Game End
释放内存的一些操作
```
void Game_End(){
    if(bomb_surf) bomb_surf->Release();
    if(bucket_surf) bucket_surf->Release();
    DirectInput_Shutdown(); //dikeyboard Unacquire、Release
    Direct3D_Shutdown(); //d3ddev& d3d Release
}
```

