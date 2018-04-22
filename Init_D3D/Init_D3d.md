#初始化D3D
>当学会了如何初始化窗口后，接下终于可以去学习Direct3D了。

DirectX是由微软公司创建的多媒体编程接口, 我们的目的是在于做游戏， 因此对于API的实现其实不用那么纠结。


## 初始化D3D的基本步骤
1. 初始化DirectX 。
2.  创建Direct3D设备(用于生成平滑的图形， 材质等等)。

### 1. 初始化DirectX 
DirectX函数调用储存在 \<d3d9.h> 这个头文件中， 而其预编译的函数储存在d3d9.lib中， vs2013以上都已经将其预安装好了， 因此只需要直接使用就好了。

为了编写D3D的程序，必须创建D3D接口变量 和一个图形设备变量。 

```
LPDIRECT3D9 d3d = NULL;
LPDIRECT3DDEVICE9 d3ddev = NULL;
```

而创建d3d对象也很简单。
```
d3d = Direct3DCreate9(D3D_SDK_VERSION);
```
这个代码已经初始化好D3D了。
没错..初始化Direct3D就...这么简单.....
### 2. 创建Direct3D设备(用于生成平滑的图形， 材质等等)。
初始化Direct3D后， 需要创建D3D将要输出显示内容的设备。 然而..这需要使用d3d来调用这个函数。
```
d3d->CreateDevice(
		D3DADAPTER_DEFAULT,                     //使用默认的视屏卡
		D3DDEVTYPE_HAL,                         //使用硬件渲染
		hwnd,                                   //窗口句柄
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,    //不使用(变换&光照)
		&d3dpp,                                 //presentation parameters
		&d3ddev);                               //d3d设备指针
```

CreateDevice最后两个参数中， 第一个参数是类似于windows注册中给窗口打了一堆property的东西， 因此..得先定义好它。

```
        D3DPRESENT_PARAMETERS d3dpp;
        ZeroMemory(&d3dpp, sizeof(d3dpp));   
        d3dpp.Windowed = TRUE;
        d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
        d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
        d3dpp.BackBufferCount = 1;  
        d3dpp.BackBufferHeight = SCREENH;
        d3dpp.BackBufferWidth = SCREENH;    
        d3dpp.hDeviceWindow = hwnd;
```
当然~~ d3dpp的参数是很多的, 它决定了Direct3D的表现, 毕竟名字叫 "呈现参数" 嘛...


变量             |类型          |注释
-|             - |-             |
BackBufferWidth  |UINT          |后台缓存区宽度
BackBufferHeight| UINT|后台缓存区高度
BackBufferFormat| D3DFORMAT|后台缓存区格式, 都是D3DFMT_(balablala)
BackBufferCount| UINT|后台缓存区数量
MultiSampleType|DWORD|全屏反走样(通常是 3DMULTISAPLE_NONE)
MultiSampleQuality| DWORD| 多采样的质量级别(通常是 0)
SwapEffect|D3DSWAPEFFECT| 后台缓冲区的交换模式
hDeviceWindow| HWND| 本设备的父窗口(CreateWindow创立的那个家伙)
Windowed|BOOL|是否窗口化
EnableAutoDepthStencil|BOOL| 允许D3D控制深度缓存区(通常为TRUE)
AutoDepthStencilFormat|D3DFORMAT| 深度缓存区的格式
Flags|DWORD| 选项标志(通常为0)
FullScreen_RefreshRateInHz|UINT| 全屏刷新率(窗口模式必须是0)
PresentationInterval|UNIT|控制缓冲区交换速率


当我们设置好 Windowed; SwapEffect; BackBufferFormat这三个参数后, 就可以调用CreateDive 创建主D3D绘图表面了.

当然在使用钱需要接口中的所有值清为0
>ZeroMemory(&d3dpp, sizeof(d3dpp));

```
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
```

### 3.窗口模式&全屏模式
窗口模式只需要按照上面的d3dpp设置就行了

而全屏模式 则需要修改两处
1. 修改CreateWindow
2. 修改d3dpp

CteaWindow中,dwStyle要改为WS_EX_TOPMOST | WS_POPUP, 使得当前窗口会出现在其它所用窗口之上, 窗口的初始位置和大小则随便设了(因为已经不重要了). WS_EX_TOPMOST: 当前窗口置顶. 而原来的 WS_OVERLAPPEDWINDOW 就会有个标题栏

d3dpp中, 得先引入一个新的变量  D3DDISPLAYMODE; 利用这个变量区查找屏幕的尺寸和颜色深度.

```
        D3DDISPLAYMODE dm;
	d3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &dm);

	D3DPRESENT_PARAMETERS  d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = FALSE;
	d3dpp.BackBufferFormat = dm.Format;
	d3dpp.BackBufferHeight = dm.Height;
	d3dpp.BackBufferWidth = dm.Width;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
```
### 4.Game Run
纯色清理帧缓冲区(表示背景色)-->换画布,为下一帧做准备
Clear的作用便是清理后台缓冲区的.
```

	//make sure the Direct3D device is valid
	if (!d3ddev) return;
	//clear the backbuffer to bright green
	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 255, 255), 1.0f, 0);

	//start rendering
	if (d3ddev->BeginScene()) {
		//do sm?

		//stop rendering
		d3ddev->EndScene();

		//copy back buffer to the frame buffer
		d3ddev->Present(NULL, NULL, NULL, NULL);
	}
```
### 5.Game End

当运行结束时, 使用 Release() 将 d3d 和d3ddev资源释放.
```
	if (d3ddev) {
		d3ddev->Release();
		d3ddev = NULL;
	}
	if (d3d) {
		d3d->Release();
		d3d = NULL;
	}
```
