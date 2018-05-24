# 初始化

>初始化真是一个 ...

在初始化之前，得需要了解一下Windows的机制，这样才不会在修仙的道路上迷失方向~~ windows是一个<b>消息驱动</b>系统，


参考百度百科:
>Windows是一个多任务操作系统，所以没有哪一个程序能够独占系统的资源，资源都是由Windows统一管理的。那么某个程序是如何获得用户的信息呢？事实上，Windows在时刻监视着用户的每个举动，并分析用户的动作与哪一个程序相关，然后将动作以消息的形式发送给当前的应用程序。相反，应用程序也在时时等着消息的到来，一旦发现它的消息队列中有未处理的信息，就获取并分析该消息，并根据消息所包含的内容采取适当的动作来响应。这里我们引出另一个概念“消息驱动”。比如当你单击file菜单的时候，首先这个动作被windows所捕获，而不是应用程序。经分析windows知道该动作该由哪个应用程序处理，然后windows就发送WM_COMMAND消息给该应用程序，它告诉应用程序，你单击了file菜单。应用程序得知这一消息后，便采取相应的动作来响应它，进行“消息处理”。Windows为每个线程维护了相应的消息队列，应用程序的任务就是不停地从特定的消息队列中获取消息、分析消息并处理消息，直到消息（WM_QUIT）为止。这个过程的程序结构称为“消息循环”。

在"消息循环"中，和游戏最息息相关的就是"消息处理"了。 因此... 关于"消息处理":
>Windows程序在处理消息时使用了“回调函数”的特殊函数。这个函数由应用程序定义，但并不由应用程序来调用，而是供操作系统或者其子系统来调用的。这种调用通常在某一事件发生，或者在窗口或字体被枚举时发生。Windows向程序员所能发送的消息多达百种，但是，对于一般的应用程序来说，只是其中的一部分有意义。


## 初始化的基本步骤
1. 注册实例(包含了设置"回调函数")
2. 创建窗口
3. 显示窗口
4. <del>消息处理(GetMessage)</del> 实时循环(PeekMessage)

### 1、注册实例
在windows中创建一个窗口之前， 得先注册它， 它就像windows的皮亚诺公理，如同自然数一样，我们经常使用它，但是却并没有关注它是如何定义的，windows中， 我们也经常打开一个又一个窗口， 但是却并没有去关注每个窗口是如何定义的。 因此， 我们要先假装不知道一个窗口到底应该包含什么，而是作为一个创造者，去想象一下一个窗口到可能需要包含哪些东西。 "大小， 样式，背景，菜单栏，光标，窗口的名字， 窗口的图标..." 反正我假装只能想到这些(谁叫我已经知道WNDLCLASSEX已经包含了哪些东西呢？ 不然我可能会说出一个牛逼的东西呢！)  事实上， 注册实例确实就是设置这些东西的，然而...这里面最关键的其实是设置"回调函数"， 因为!windows是"消息驱动系统"! 而回调函数就是用来处理消息的， 因此必须得它设置好来，当我们设置好之后， 它就会智能的供操作系统或者其子系统来调用。

以下是注册实例的代码
```
//HINSTANCE:instance句柄（instance实例）
//ATOM: unsiged short

ATOM MyRegisterClass(HINSTANCE hInstance) {                      
	WNDCLASSEX wc;                                               //窗台类
	wc.cbSize = sizeof(WNDCLASSEX);                              //大小   cb = count of byte
	wc.style = CS_HREDRAW | CS_VREDRAW;                          //CS_HREDRAW: 窗口移动或者宽度改变时完全重新绘制  CS_VREDRAW: 窗口高度调整后完全重新绘制(REDRAW)   CS = Class Styles (所以..h是宽度？ v是高度？ 我不知道...)
	wc.lpfnWndProc = (WNDPROC)WinProc;                           //回调函数(最重要的)
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;                                           //和上一个property一样用来为窗口实例多增加一些额外空间      
	wc.hInstance = hInstance;                                    //设置为函数的参数。 让主窗口知道正在使用的是哪个实例。
	wc.hIcon = NULL;                                             //图标
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);                    //光标
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);      //背景
	wc.lpszMenuName = NULL;                                      //菜单
	wc.lpszClassName = APPTITLE.c_str();                         //窗口名字
	wc.hIconSm = NULL;                                           //和窗口类关联的小图标。如果为NULL, 则把hIcon中的图标转换成大小合适的小图标。(sm什么意思)
	return RegisterClassEx(&wc);                                 //提交~~ 如果返回值为0表示失败，成功则会将值传回给InitInstance.
}
```
这样一比较~~ 除了 lpfnWndProc，wc.cbClsExtra，cbWndExtra，wc.hIconSm，wc.hInstance以及最后的提交(reutrn) 意料之外(但也不难理解)， 其它都是意料之中的property嘛~~

### 2&3、创建窗口 与 显示窗口
如果连注册实例这么不那么直观的东西都理解了，那创建窗口真的就是小菜一碟了。
```
HWND: 窗口句柄。 H表示handle
//Inistance的目的是告诉WinMain启动成功还是失败
bool InitInstance(HINSTANCE hInstance, int nCmdShow) {      //hInstance是它从windows接受来的实例程序。 nCmdShow是从windows中接受的一个参数
	//create a new windsow;
	HWND hWnd = CreateWindow(
		APPTITLE.c_str(),                //窗口的类名  MyRegisterClass注册的lpszClassName
		APPTITLE.c_str(),                //标题条   
		WS_OVERLAPPEDWINDOW,             //窗口风格(有没有边框呀标题呀..之类的..)   WS：Window Style
		CW_USEDEFAULT, CW_USEDEFAULT,    // postion of window   CW: Child Window
		640,480,                         // size of window
		NULL,                            //父窗口 (not used)
		NULL,                            //菜单 (not used)
		hInstance,                       //应用实例
		NULL);                           //balablaabla(not used)

	//was there an error creating the window?
	if (window == 0) return 0;

	ShowWindow(hWnd, nCmdShow);                //显示窗口
	UpdateWindow(hWnd);                        //重新画界面，相当于刷新(WM_PAINT消息并不是一直发的，一般在窗口遮住后再显示时系统自动发，而且它的优先级很低，只有在别的消息处理完后才处理；所以这不同于 循环中更新)

    //WM_PAINT: WndProc处理的第二个消息为WM_PAINT。这个消息在Windows程序设计中是很重要的。当窗口显示区域的一部分显示内容或者全部变为“无效”，以致于必须“更新画面”时，将由这个消息通知程序。

	return 1;
}
```

### 4、<del>消息处理(GetMessage)</del> 实时循环(PeekMessage)
理解完上面那些... 剩下的就是A了吧？
最后便是windows程序的入口--WINAPI WinMain。 其实,也没特别的， 和c/c++中的main地位差不多吧~~

```
WinMain(){
    Message Loop:
        GetMessage
        TranslateMessage
        DispathMessage
    Repeat
}
```

GetMessage的调用是具有决定性的代码。相当于程序的知觉。
TranslateMessage函数用于将虚拟键盘消息翻译成字符信息， 
然后通过DisptachMessage发送给Windows消息系统中。

就这样不停的循环这三部曲。 

```
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	//create the window
	MyRegisterClass(hInstance);
	if (!InitInstance(hInstance, nCmdShow)) return 0;
	// main message loop
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;                                          //wParam: window parameter
}

```

然而~~ 一般程序的循环过程并不能满足游戏的需求， 因为游戏需要不停的循环， 而GetMessage必须要等到队列中有等待处理的消息纯在它才会执行它后面的语句， 因此， 我们需要用PeekMessage, 它则没有这个等待的设计， 如果没有消息， 它只会将控制传递给下一条语句。 PeekMessage显然更绅士一些。

```
BOOL PeekMessage(
    LPMSG lpMsg,            //描述本msg的指针   LP: long pointer
    HWND hWnd,              //与该事件关联的窗口句柄
    UINT wMsgFilterMin,     //已收到的第一条信息
    UINT wMsgFilterMax,     //已收到的最后一条星系
    UINT wRemoveMsg         //当读取该消息之后该如何处理。 PM_NOREMOVE--将消息留在队列中 PM_REMOV--在读取消息将它从队列中移除
);
```


```
//HINSTANCE hInstance：标识被调用的程序的实例
//HINSTANCE hPrevInstance: 前一个实例
//LPSTR IpCmdLine：用于告诉程序使用某些选项。
//int nCmdShow: 窗口显示方式。

//UINT: unsigned int
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow) 
{
	//create the window
	MyRegisterClass(hInstance);
	if (!InitInstance(hInstance, nCmdShow)) return 0;

	//main message loop
	MSG msg;
	if (!Game_Init())return 0;
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
```
### 5、WinProc[p'rɒk]
什么？ 还有5？ emmm..不难发现 从头到尾， 都在强调 "回调函数", 显然它才是游戏的核心啊~ 什么 TranslateMessage啊 DisptachMessage啊，主要目的就是为WinProc创造消息。 比如说当按下A键要去F2 A了， TranslateMessage将虚拟键盘消息翻译成字符信息， 然后DisptachMessage发送给Windows消息系统中， 之后可不就是WinProc来处理这个消息， 让你所有的部队A到对面家里，让对手打GG嘛， 因此..这才是游戏编程的核心， 也是..通向初始化Direct3D的钥匙。

```
LRESULT: long long

LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) 
{
    // \TODO
    if(message == WM_DESTORY){
        gameover = true;
		PostQuitMessage(0);
		break;
	}
    return DefWindowProc(hWnd, message, wParam, lParam);
}
```


- HWND hWnd 窗口句柄，在DirectX程序中，窗口句柄仅在开始时用于创建窗口。
- UINT message  发送给窗口回调过程的信息(消息ID)
- WPARAM wParam 消息参数
- LPARAM lParam 消息参数


- WM_DESTORY 消息标识符 表示窗口该关闭了
- PostQuitMessage(0) 结束程序
>LRESULT DefWindowProc(HWND hWnd，UINT Msg，WPARAM wParam，LPARAM IParam);
>
>DefWindowProc这个函数是默认的窗口处理函数，我们可以把不关心的消息都丢给它来处理。这个函数在处理关闭窗口消息WM_CLOSE时，是调用DestroyWindow函数关闭窗口并且发WM_DESTROY消息给应用程序；而它对WM_DESTROY这个消息是不处理的；我们在应用程序中对这个消息的处理是发出WM_QUIT消息。因此WM_CLOSE、WM_DESTROY、WM_QUIT这三个消息是先后产生的。
