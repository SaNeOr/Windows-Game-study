# 初始化窗口

## 关于windows的机制
初始化窗口，最难理解的部分主要是奇怪的类型名和不知道如何使用的API, 由于... 我是打算任务驱动（实现一个游戏）去了解这些东西， 因此一切point都是围绕着主线展开。

windows是一个<b>消息驱动</b>系统， 这包含了windows句柄、实例标识符、消息类型， 用于告诉程序有消息发生。

每个windows程序至少包括一个名为WinMain的函数， 大多数还包括一个名为WinProc[p'rɒk]的消息处理器函数用于处理接收消息(比如按键和鼠标移动)。



以下是一个标准的WinMain版本
```
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow) 
{
	//create the window
	MyRegisterClass(hInstance);
	if (!InitInstance(hInstance, nCmdShow)) return 0;	//windows传递给WinMain hinstance变量， 
								//InitInstance 位于程序的更下部分，
								//检查程序是否已经在运行中， 
								//然后创建主程序窗口。

	//main message loop
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {                  //Msg变量用于取得Windows消息的详细信息
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}
```
关于GetMessage()  

          BOOL GetMessage(LPMSG lpMsg,              //向用于处理消息的msg结构指针,用于处理msg指针的。
                          HWND hWnd,                //该值为NULL时，为当前程序实例返回所有消息。
                          UNIT wMsgFilterMin,       //和第四个参数一起告诉GetMessage返回一定范围内的消息(暂时不了解 这个范围指的是什么)
                          UNIT wMsgFilterMax)

       GetMessage的调用是具有决定性的代码。相当于程序的知觉。
       GetMessage 两行核心代码处理其返回的msg
       TranslateMessage函数用于将虚拟键盘消息翻译成字符信息， 
       然后通过DisptachMessage发送给Windows消息系统中。

       主要目的就是为WinProc创造消息。
       

- HWND: 窗口句柄。 H表示handle
- HINSTANCE:instance句柄（instance实例）
- LPSTR:win32中的字符串， 被定义为指向NULL('\0')结尾的32位ANSI字符数组指针。 L代表long 
- UINT: unsigned int
