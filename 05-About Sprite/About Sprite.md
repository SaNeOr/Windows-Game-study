# About sprite

1. 什么是sprite
2. texture
3. 精灵渲染器
4. sprite表
5. 骑士漫步

## 1.什么是sprite
在"build a game" 中， 我们使用blit的方法(SURFACE), 完成了一个简单的接炸弹的游戏，但是很显然，无论是炸弹还是篮子，都很明显的可以看到图片的背景色， 要是能把这个背景去掉该多好~ 因此， 利用sprite的透明处理，就能很轻松(仅仅是设置你想要透明的颜色的值就行了)的完成这个效果， 当然sprite的功能远远不止与此，旋转、缩放、移动都可以通过sprite轻松完成。

## 2.texture
blit使用的对象是LPDIRECT3DSURFACE9， 也就是表面(surface)， 但是sprite则使用的是贴图(texture)，所以这次使用的对象是LPDIRECT3DTEXTURE9(事实上，这么做的原因在于GPU会使用 纹理映射 系统来渲染sprite，这比 surface的渲染将会快得多， 但是在制卷动背景时，我们还是会使用surface的)。
> sprite的对象是LPDIRECT3DTEXTURE9

因此，创建游戏精灵首先需要创建一个用于加载精灵位图 的 <b>图像的纹理对象</b>。
```
LPDIRECT3DTEXTURE9 texture = NULL;
```

之后和surface一样， 我们需要加载一个位图。
```
D3DXIMAGE_INFO info;
result = D3DXGetImageInfoFromFile("image.bmp",&info);
```
(如果文件存在)我们将会获得这个图像的Width和Height，这显然对下一步很有用， 因此我们可以将 图像的宽和高保存到 D3DXVECTOR2中， 因此我们可以写一个函数便于重用， 当然如果你的游戏里面可能需要获得图像的info...这个函数可以加入MyDirectX.cpp和MyDirectX.h豪华午餐。
```

D3DXVECTOR2 GetBitmapSize(string filename){
    D3DXIMAGE_INFO info;
    D3DXVECTOR2 size = D3DXVECTOR(0.0f, 0.0f);
    HRESULT result = D3DXGetImageInfoFromFile(filename.c_str(), &info);
    if(restult == D3D_OK)
        size = D3DXVECTOR2((float)info.Width, (float)info.Height);
    else
        size = D3DXVECTOR2((float)info.Width, (float)info.Height);
    return size;
}
```
接着还是和surface一样， 将位图文件加载到texture中。不过，这里用的不再是D3DXLoadSurfaceFromFile， 而是D3DXCreateTextureFromFileEx(surface的加载用的是load，而texture用的是Create... 是不是很奇怪233...)
我们来看下D3DXCreateTextureFromFileEx的声明
```
HRESULT D3DXCreateTextureFromFileEx(
    LPDIRECTDDEVICE9    pDevice,
    LPCTSTR             pSrcFile,
    UINT                Width,
    UINT                Height,
    UINT                MipLevels,
    DWORD               Usage,
    D3DFORMATE          Format,
    D3DPOOL             Pool,
    DWORD               Filter,
    DWORD               MipFilter,
    D3DCOLOR            ColorKey,
    D3DXIMAGE_INFO      *pSrcInfo,
    PALETTEENTRY        *pPalette,
    LPDIRECT3DTEXTURE   *ppTexture
);
```
虽然看上去密密麻麻一大堆,但是实际上绝大多数参数都是设置默认值和NULL就行， 因此我们写一个函数来处理这些默认设置就好了，既然surface里面叫load我们加载texture的这个函数也叫load多好。
```
LPDIRECT3DTEXTURE9 LoadTexture(string filename, D3DCOLOR transcolor){
    LPDIRECT3DTEXUTRE9 texture = NULL;
    //get width height from bimap file
    D3DXIMAGE_INFO info;
    HRESULT result = D3DXGetImageInfoFromFile(filename.c_str(), &info);
    if(result != D3D_OK) return NULL;

    //create the texture by loading a bitmap image file
    D3DXCreateTextureFromFileEx(
        d3ddev;             //Direct3D device object
        filename.c_srt();   //bitmap filename
        info.Width;         //bitmap image width
        info.Height;        //bitmap image Height
        1,                  //mip-map levels(1 for no chain)
                            //mip-map帮助避免了不想要的锯齿边缘
        
        D3DPOOL_DEFAULT,    //the type of surface (default) 
                            //D3DPOOL_DEAFULT = 0
        D3DFMT_UNKNOWN,     //surface format(default) 
        D3DPOOL_DEFALUT,    //memory class for the texture
        D3DX_DEFAULT,       //image fliter
        D3DX_DEFALUT,       //mip filter
        transcolor,         //color key for transparency设置颜色见 用来设置哪个颜色透明的
                            //支持alpha通道
        &info,              //bitmap file info(from loaded file)
        NULL,               //color palette,
        &texture);          //destination texture
}
//make sure the bimap texture was loaded correctly
if(result != D3D_OK)    return NULL;
return texture;
```

>FYI:事实上...surface里面也有transparency的设置，而且按照MSDN的里说法，是将设置的颜色键设置为透明的黑色，并且这两个参数的说明是一样的，但是我给surface设置transparency的时候，显示的是不透明的黑色。emmm...不解欸~

## 3.精灵渲染器
了解了texture的处理，接下来就是了解如何使用sprite了。
精灵处理器D3D中有两种方式, 一种是 ID3DXSprite 还有一种是指针版， 也就是LP3DXSPRITE， 这里使用指针版，因为它更好用。

关于精灵渲染器一共有4步
1. 初始化精灵渲染器
2. 启动精灵渲染器
3. 绘制精灵&定时器
4. 停止渲染

#### 1. 初始化精灵渲染器
```
LP3DXSPRITE spriteobj = NULL;
```
>再定义新对象的时候我们总是给它设置为NULL, 因为如果不这么设置的话，在做测试时， 对象将无法与NULL比较，也就是说如果不初始化， if(spriteboj == NULL) 返回的将不是false，而是程序崩溃。

定义完sprtie后，便是初始化它了(D3D所有的对象不都是这个尿性嘛，先定义， 然后初始化)。 通过调用D3DXCreateSprite函数来初始化对象。这个函数的作用时将精灵处理器附着在Direct3D设备上， 以便它知道如何在后台缓冲区中绘制精灵。
```
HRESULT WINAPI D3DCreateSprite(
    LPDIRECT3DDEVICE9   pDevice,
    LPD3DXSPRITE        *ppSprite
);

eg:
result = D3DXCreateSprite(d3ddev, &spriteobj);
```

#### 2. 启动精灵渲染器
简单来说，首先要先锁住表面。 通过Begin方法就好了。
```
//ID3DXSprite::Begin
HRESULT Begin(DWORD Flags);
eg:
spriteobj->Begin(D3DSPRIETE_ALPHABLEN);
```

#### 3. 绘制精灵
虽然只有Draw这一个方法，但是..这个方法是真的牛逼，通过它就可以完成旋转、缩放、移动的操作， 当然...还有变色(有了它， 就可以做出马里奥吃了星星的效果了)。
因此， 让我们来看下它的声明。
```
HRESULT Draw(
    LPDIRECT3DTEXURE9 pTexture,     //指定精灵所使用的源图像的纹理
    CONST RECT *pSrcRect,           //选择源图像指定区域，可用于动画帧
    CONST D3DXVECTOR3 *pCenter,     //旋转中心点
    CONST D3DXVECTOR3 *pPosition,   //指定精灵位置
    D3DCOLOR color                  //对精灵进行色彩变更
);

//D3DXVECTOR3 
typdef struct D3DXVECTOR3{
    FLOAT x;
    FLOAT y;
    FLOAT z;
}D3DXVECTOR3;

//在2d屏幕表面中移动精灵只需要x,y就行了
```

#### 4.停止渲染
在完成精灵绘制之后，并且在d3ddev调用EndScene之前，必须调用spriteobj的End方法来对表面解锁， 以便其它进程使用。
```
//ID3DXSprite::Begin
HRESULT END(VOID);
eg:
spriteobj->End();
```

#### 关于transparency
>事实上, 游戏开发中图片应该预先处理好，设置好alpha，颜色键其实是很少使用到的。

## 4.sprite表&定时器
>在说到Draw这个关键方法的时候，我们特意说了第二个参数， 可以用于动画帧，这里我们就是特别着重来看这一个参数。

首先，我们都知道帧率是指每秒显示的帧数， 也就是每秒显示了多少张图片，但是如果我们把一张一张图单独存放实在是太麻烦了， 因此我们会在一个源图中编排所有的动画帧， 然后通过指定区域的方法(或者说切割源图的方法)来制作动画效果。那么这样的源图，我们就称之为精灵表(sprite sheet)，我们会把精灵表当作矩阵来处理，每个小部分就是一帧，通过Draw第二个参数来完成切割操作。

那么,我们需要知道如何计算图片单元。 我们以一个有5行6列的源图像为例，也就是这是一个可以绘制出30帧的精灵表。 假如我们现在要绘制第20帧， 那么其将会在 第3行第1列(行和列以及当前是第几帧都是从0开始算)。 对了, 我们的代表点是左上角那个。

```
left = (current frame % number of columns) * sprite width
top  = (current frame / number of columns) *sprite height
```

以刚刚那个例子为例:
```
left = (19 % 6) * sprite width(第20帧， 从0开始， 就是 19帧)
top  = (19 / 6) * sprite height

left = 1sprite width
top  = 3sprite height
```
当然啦，Draw的第二个参数是RECT，因此还得需要知道右下角的那个点的坐标。
```
right = left + width
bottom = top + height
```
掌握了基本的sprite sheet切割原理后， 我们便可以写一个可以处理动画帧的函数，来调用Draw。
```
void Sprite_Draw_Frame(LPDIRECTURE3DTEXTURE9 texture, int destx, int dest y, int curframe, int framew, int frameh, int columns){
    D3DXVECTOR position((float)destx, (float)desty, 0);
    D3DCOLOR white = D3DCOLOR_XRGB(255,255,255);
    RECT rect;
    rect.left   = (curframe % colums) * framew;
    rect.top    = (curframe / colums) * frameh;
    rect.right  = rect.left + framew;
    rect.bottom = rect.top  + frameh;
    spriteobj->Draw(texture, &rect, &postion, white);
}
```
但是这里有个问题, 我们要绘制单帧精灵怎么办呢？ 也就是说没有curframe,colunms。 因此，我们可以使用默认值，要注意的是， C++中使用默认值是不能跳的，也就是必须按照函数参数的声明顺序来调用。 因此我们需要按照不用程度来设置参数顺序， 不用程度最高的放在最后面。

```
//Sprite_Draw_Frame  declaration
void Sprite_Draw_Frame(
    LPDIRECT3DTEXTURE9 texutre = NULL;
    int destx = 0;
    int desty = 0;
    int framw = 64;         //游戏精灵中非常常规的一个尺寸
    int frameh = 64;
    int framenum = 0;
    int colunms = 1;
);
```
当然...我们还得设置每帧之间隔多久时间，也就是帧速率。接用Windows API的GetTickCount()， 我们来看下msdn里对这个函数的介绍:
>Retrieves the number of milliseconds that have elapsed since the system was started, up to 49.7 days.
检索自系统启动以来经过的毫秒数，最长为49.7天。

因此我们通过设置每隔多少毫秒来切换到下一张图片来设置帧率。
```
void Sprite_Animate(int &frame, int startframe, int endframe, int direction, int &starttime, int delay){
    if((int)GetTickCount() > starttime + delay){
        starttime = GetTickCount();
        frame += direction;
        if(frame > endframe) frame = startframe;
        if(frame < startframe) frame = endframe;
        //如果direction +1 那么是正放， -1则是倒放
    }
}
```

举个例子, 我们要设置一个30帧的动画， 每30ms一帧, 将其绘制在窗口上(200,200)的位置上，帧的长宽都是128， sprite sheet有6列。
```
//首先设置两个全局变量
int frame = 0;
int starttime   = 0;

//animate and draw the sprite
Sprite_Animate(frame, 0, 29, 1, starttime, 30);
SPrite_Draw_Frame(texture,200,200,frame,128,128,6);
```

## 5. 骑士漫步
呼，了解了这么多后， 我们可以来做一个游戏中最常见的一幕了！ 人物移动!! 这和接炸弹那种 单纯的位移是不同的， 这里的移动是伴随着动画的，并且是通过矩阵来处理移动的。d3dx中以已经帮我们完成了矩阵的处理，我们只需要调用它就行了，怎么调用呢？在draw之前调用setTransform就行了，它的参数是一个矩阵，而设置矩阵是通过D3DXMatrixTransformation2D()函数来实现的(关于这个函数的声明看<a href="https://msdn.microsoft.com/query/dev15.query?appId=Dev15IDEF1&l=ZH-CN&k=k(D3DX9MATH/D3DXMatrixTransformation2D);k(D3DXMatrixTransformation2D);k(DevLang-C++);k(TargetOS-Windows)&rd=true)">msdn</a>就行了)。 因此...我们得再创建一个新的以draw和setTransform为核心的函数，就叫它Sprite_Transform_Draw好了。 顺便一提，我们之前说过了，sprite不仅仅支持移动还支持缩放、旋转。 这些操作通过矩阵都可以完成。
```
void Sprite_Transform_Draw(LPDIRECT3DTEXTURE9 image, int x, int y, int width, int height, 
    int frame, int columns, float rotation, float scaling, D3DCOLOR color)
{
    //create a scale vector
    D3DXVECTOR2 scale( scaling, scaling );

    //create a translate vector
    D3DXVECTOR2 trans( x, y );

    //set center by dividing width and height by two
    D3DXVECTOR2 center( (float)( width * scaling )/2, (float)( height * scaling )/2);

    //create 2D transformation matrix
    D3DXMATRIX mat;
    D3DXMatrixTransformation2D( &mat, NULL, 0, &scale, &center, rotation, &trans );
    
    //tell sprite object to use th
    e transform
    spriteobj->SetTransform( &mat );

    //calculate frame location in source image
    int fx = (frame % columns) * width;
    int fy = (frame / columns) * height;
    RECT srcRect = {fx, fy, fx + width, fy + height};

    //draw the sprite frame
    spriteobj->Draw( image, &srcRect, NULL, NULL, color );
}
```
基本思路就是首先用 两个全局变量记录骑士的位置， 以及必要的frame和startime
```
LPDIRECT3DTEXTURE9 paladin = NULL;

int frame = 0, colums, width, height;
int startframe, endframe, starttime = 0, delay;
float p_x = 300, p_y = 250;
```
然后根据源图在Game_Init()中设置好 colums, width, height, delay 以及加载位图。当然，一开始就定义好也行。
```
	paladin = LoadTexture("paladin_walk.png");

	colums = 8;
	width = height = 96;
	delay = 100;
```

之后便是设置帧区间，以及移动。
这里有一个简单的方法，按下某个键，就将startframe和endframe设置成对应的区间左右两边的值就好了。 不过别忘了设置 startframe = endframe， 不然你不按键盘的时候，应该使用那张帧呢？ 
```
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
```

最后就是用那两个我们写好的两个好兄弟函数--<b>设置帧率</b>和<b>新的可以支持transform的draw</b>。
```
    Sprite_Animate(frame, startframe, endframe, 1, starttime, delay);
    Sprite_Transform_Draw(paladin, p_x, p_y, width, height, frame, colums);
```
