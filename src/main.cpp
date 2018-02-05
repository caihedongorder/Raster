//Raster.cpp: 定义应用程序的入口点。
//

#include "stdafx.h"
#include <stdio.h>
#include <memory>
#include "resource.h"
#include "colorbuffer.hpp"
#include "GameTime.hpp"
#include "Math.hpp"
#include "FPS.hpp"
#include "Profile.hpp"
#include "scene.h"
#include "utils.h"
#include "LOpenGL.h"

unsigned char* LoadFileContent(const char* fileName, int & fileSize) {
	unsigned char* fileContent = nullptr;
	fileSize = 0;
	FILE* file = fopen(fileName, "rb");
	if (file)
	{
		fseek(file, 0, SEEK_END);
		int filelen = ftell(file);
		if (filelen > 0)
		{
			fileContent = new unsigned char[filelen + 1];
			rewind(file);
			fread(fileContent, sizeof(unsigned char), filelen, file);
			fileContent[filelen] = 0;
		}
		fclose(file);
		file = nullptr;
	}

	return fileContent;
}

// 此代码模块中包含的函数的前向声明: 
void rundMainLoop(int var);

bool initOpenGL(int argc,char** argv);

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_FPS = 60;
const char* WINDOW_TITLE = "OPEN GL";

LRESULT CALLBACK GLWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	default:
		break;
	}
	return ::DefWindowProc(hwnd, msg, wParam, lParam);
}


//int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd )
int main(int argc,char** argv)
{
	WNDCLASSEX wnd;
	wnd.cbClsExtra = 0;
	wnd.cbSize = sizeof(wnd);
	wnd.cbWndExtra = 0;
	wnd.hbrBackground = NULL;
	wnd.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	wnd.hIcon = NULL;
	wnd.hIconSm = NULL;
	wnd.hInstance = ::GetModuleHandle(NULL);
	wnd.lpfnWndProc = GLWindowProc;
	wnd.lpszClassName = TEXT("GLWindow");
	wnd.lpszMenuName = NULL;
	wnd.style = CS_VREDRAW | CS_HREDRAW;

	auto atom = ::RegisterClassEx(&wnd);
	if (!atom) {
		::MessageBox(NULL, TEXT("注册窗口失败"), TEXT("出错"), MB_OK);
		return -1;
	}

	RECT rect;
	rect.left = 0;
	rect.top = 0;
	rect.right = 800;
	rect.bottom = 600; 
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW,FALSE);
	int winWidth = rect.right - rect.left;
	int winHeight = rect.bottom - rect.top;

	HWND hwnd = CreateWindowEx(NULL, TEXT("GLWindow"), TEXT("OpenGL Window"), WS_OVERLAPPEDWINDOW,
		100, 100, winWidth, winHeight, NULL, NULL, ::GetModuleHandle(NULL), NULL);
	
	//create gl 
	HDC hdc = ::GetDC(hwnd);
	PIXELFORMATDESCRIPTOR pfd = { 0 };
	pfd.nVersion = 1;
	pfd.nSize = sizeof(pfd);
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 8;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	int formatIndex = ::ChoosePixelFormat(hdc, &pfd);
	::SetPixelFormat(hdc, formatIndex, &pfd);
	HGLRC rc = wglCreateContext(hdc);
	wglMakeCurrent(hdc, rc);
	auto error = glGetError();
	glewInit();

    //关闭垂直同步
    wglSwapIntervalEXT(0);
	error = glGetError();
	Init();
	SetViewportSize(800, 600);

	::ShowWindow(hwnd, SW_SHOW);
	::UpdateWindow(hwnd);

	MSG msg = {0};
    Raster::FPS fps;

	while (true)
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE	))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		Draw();

        fps.Update();

		::SwapBuffers(hdc);

        char szFPS[MAXBYTE] = {0} ;
        sprintf(szFPS,"FPS:%02f",fps.GetFPS());

        /* ::TextOut(hdc,10,10,szFPS,strlen(szFPS)); */
        ::SetWindowTextA(hwnd,szFPS);

	}

	return 0;
}
