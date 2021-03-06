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
#include "Camera.hpp"


// 此代码模块中包含的函数的前向声明: 
void rundMainLoop(int var);

bool initOpenGL(int argc,char** argv);

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_FPS = 60;
const char* WINDOW_TITLE = "OPEN GL";
OpenGL::Camera GCamera;
float GCameraMoveScale = 10.0f;

void UpdateCamera(){
    glm::vec3 CameraMoveDelta = glm::vec3(0.0f);
    short keyState = ::GetKeyState('A');
    bool CameraChange = false;
    glm::vec3 leftOrRightMoveDelta(0.0f);
    glm::vec3 forwardOrBackMoveDelta(0.0f);
    glm::vec3 upOrDownMoveDelta(0.0f);
	auto r = GCamera.getViewMatrixRight();
    if(keyState & 0x8000)
    {
        leftOrRightMoveDelta = -GCamera.getViewMatrixRight();
        CameraChange = true ;
    }
    keyState = ::GetKeyState('D');
    if(keyState & 0x8000)
    {
        leftOrRightMoveDelta = GCamera.getViewMatrixRight();
        CameraChange = true ;
    }
    keyState = ::GetKeyState('W');
    if(keyState & 0x8000)
    {
        forwardOrBackMoveDelta = GCamera.getViewMatrixForward();
        CameraChange = true ;
    }
    keyState = ::GetKeyState('S');
    if(keyState & 0x8000)
    {
        forwardOrBackMoveDelta = -GCamera.getViewMatrixForward();
        CameraChange = true ;
    }
    keyState = ::GetKeyState('Q');
    if(keyState & 0x8000)
    {
        upOrDownMoveDelta = -GCamera.getViewMatrixUp();
        CameraChange = true ;
    }
    keyState = ::GetKeyState('E');
    if(keyState & 0x8000)
    {
        upOrDownMoveDelta = GCamera.getViewMatrixUp();
        CameraChange = true ;
    }

	auto moveDir = glm::normalize(leftOrRightMoveDelta + forwardOrBackMoveDelta + upOrDownMoveDelta);
    if(CameraChange)
        GCamera.move( moveDir * GCameraMoveScale );

}

bool GBeginRotateCam;
glm::ivec2 GRightMouseButtonDownPosition;
LRESULT CALLBACK GLWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
    case WM_RBUTTONDOWN:
        GRightMouseButtonDownPosition.x = LOWORD(lParam);
        GRightMouseButtonDownPosition.y = glm::clamp<int>(HIWORD(lParam),0,SCREEN_HEIGHT);
        GBeginRotateCam = true;
		::SetCapture(hwnd);
        break;
    case WM_RBUTTONUP:
        GBeginRotateCam = false;
		::ReleaseCapture();
        break;
    case WM_CHAR:
        {
            char iCode = wParam;
            printf("iCode:%c\r\n", iCode );
            OnChar( iCode );
        }
        break;
    case WM_MOUSEMOVE:
        {
            if(GBeginRotateCam)
            {
                int x = LOWORD(lParam);
                int y = glm::clamp<int>(HIWORD(lParam),0,SCREEN_HEIGHT);
                float pitch = float(x - GRightMouseButtonDownPosition.x) / SCREEN_WIDTH * 60.0f;
                float roll = float(y - GRightMouseButtonDownPosition.y) / SCREEN_HEIGHT * 60.0f;
                GCamera.rotate(roll,pitch);

				char szMsg[MAXBYTE] = { 0 };
				sprintf(szMsg, "roll:%f,pitch:%f\n", roll, pitch);
				OutputDebugStringA(szMsg);

				GRightMouseButtonDownPosition.x = x;
				GRightMouseButtonDownPosition.y = y;
            }
        }
        break;
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
	Init(&GCamera);
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

        UpdateCamera();

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
