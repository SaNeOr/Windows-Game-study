# 文字
>益智游戏中的得分？动作游戏的combo？对话？这些东西对于我这种没有什么艺术细菌的人来说，用符号或者图来表达实在是太困难了。所以...文字或许是游戏中去表达设计者意图的最直接同时也是最后的一个方法。

这章比较简单
1. 如何在窗口中打印文本？
2. 给我们的接炸弹游戏来个得分吧💣

## 1.如何在窗口中打印文本
类似于 输入设备呀， surface呀， sprite呀..关于font 也是 声明 定义 创建 使用 这四个步骤。

声明font对象
```
LPD3DXFONT font;
```
定义font对象，或者更专业点说，设置对象的属性。首先看下D3DXFONT_DESC结构的属性。
```
D3DXFONT_DESC{
    INT Height,
    UINT Width,
    UINT Weight,
    UINT MipLevels,
    BOOL ltalic,
    BYTE CharSet,
    BYTE OutputPrecision,
    BYTE Quality,
    BYTE PitchAndFamily,
    CHAR FaceName[LF_FACESIZE]
};
```
从参数的名字，都能猜出来大概是要设置什么了。 在设置上和之前一样...依旧是很多属性都是设置成默认值。
而在这里 Height和FaceName是最关键的两个参数，一个是字体大小， 一个是字体样式， 因此我们可以重新写一个只用这两个变量作参数的函数。
```
//MakeFont
LPD3DXFONT MakeFont(string name, int size){
    LPD3DXFONT font = NULL;
    D3DXFONT_DESC desc = {
        size,                   //height             
        0,                      //width
        0,                      //weight
        0,                      //miplevels
        false,                  //ltalic
        DEFAULT_CHARSET,        //charset
        OUT_TT_PRECISE,         //OutputPrecision
        CLIP_DEFAULT_PRECISE,   //Quality
        DEFAULT_PITCH,          //PitchAndFamily
        *name                   //font name
    };
    D3DXCreateFontIndirect(d3ddev, &desc, &font);
    return font;
}

//关于D3DXCreateFontIndirect，是用来创建字体对象的。
//第一个参数是D3D设备， 第二个是 设置好的字体的describe，第三个则是字体变量对象。
```
接下来是通过ID3DXFont::DrawText()方法来在窗口中打印font
首先来看下DrawTextyou有哪些参数
```
//LPD3DXSPRITE  pSprite     精灵渲染对象，如果不设置将会自动为font创建一个sprite
LPCSTR          pString     要打印的文本
INT             cont        文本长度        
LPRECT          pRect       指定位置和边界矩形
DWORD           format      指定文本格式
D3DCOLOR        color       文本的输出颜色  
```
当然， 我们依旧可以为其创建一个函数，来方便调用它。
由于颜色我们一般设置为版色，在声明的时候可以给它一个默认参数。
```
//FontPrint
//void FontPrint(LPD3DXFONT font, int x, int y, string text, D3DCOLOR color){
    //figure out the text boundary
    RECT rect = {x,y,0,0};
    font->DrawText(NULL, text.c_str(), text.length(), &rect, DT_CALCRECT, color);
    //print the text
    font->DrawText(spriteobj, text.c_str(), text.length(), &rect, DT_LEFT, color);
}
//关于format， 我们设置的是DT_LEFT,它将会是横着一排的表达方式。
//使用DT_CALCRECT可以帮我们及所处rect的大小，从而在设置rect的时候只需要设置左上角的点就行了。
//如果想让文本折行，使用DT_WORDBREAK就行了，不过这个时候就得手动去设置rect的大小了。
```


## 2.给我们的接炸弹游戏来个得分
>当然...我们不会用surface了，而是改用sprite。接炸弹的测试直接用碰撞检测去完成。

这里主要说明文字方面。
首先我们先定义一个全局变量SCORE。然后, 当bucket接到炸弹的时候score += 10； 当bomb没被接住时score -= 50；

```
//set font and SCORE
static int SCORE = 0;
LPD3DXFONT ARIAL32 = NULL;

//bool Game_Init(HWND window){
    ARIAL32 = MakeFont("arial", 32);
    /* balbalala*/
}

//if caught bomb
if (CollisionD(bucket, bomb)) {
    SCORE += 10;
    /* balbalala*/
}

//if not caught
if (bomb.y - 128 >= SCREENH) {
    SCORE -= 50;
    /* balbalala*/
}

```
最后就是将当前得分显示在窗口中，这里有一点不解， 如果把DrawText的pSprite参数设置成spriteobj， 当我们只绘制bomb的时候font会和bomb一起下降，而如果bomb和bucket都绘制的话或者仅仅绘制bucket，font并没显示。 或许...每个对象都得设置成自己独立的sprite吧？ 也许后面我就会知道答案了。 这里...我们把font的pSprite姑且设置成NULL，这样它就会为font创建自己的sprite了。
```
//MyDirectX.cpp
void FontPrint(LPD3DXFONT font, int x, int y, string text, D3DCOLOR color) {
	//figure out the text boundary
	RECT rect = { x, y, 0, 0 };
	font->DrawText(NULL, text.c_str(), text.length(), &rect, DT_CALCRECT, color);
	
	//print the text
	font->DrawText(NULL, text.c_str(), text.length(), &rect, DT_LEFT, color);
}
```

最后显示在屏幕上的代码，可以调用string的to_string()函数， 将int转换为string。
```
string score = "SCORE: " + to_string(SCORE);
FontPrint(ARIAL32, SCREENH, 30, score.c_str());
```





