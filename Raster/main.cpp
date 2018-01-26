//Raster.cpp: 定义应用程序的入口点。
//

#include "stdafx.h"
#include <stdio.h>
#include "resource.h"
#include "colorbuffer.hpp"
#include "Math.hpp"
#include "FPS.hpp"
#include "Profile.hpp"
#include "DrawPointSample.hpp"
#include "DrawLineSample.hpp"
#include "DrawCircleSample.hpp"
#include "DrawCircleSample1.hpp"
#include "DrawBezierCurve.hpp"
#include "DrawTriangleSample.hpp"
#include "DrawColorLineSample.hpp"
#include "DrawColorTriangleSample.hpp"
#include "DrawImageSample.hpp"
#include "DrawTextureTriangle.hpp"
#include <mathfu/glsl_mappings.h>
#include "OpenGLDrawQuad.hpp"
#include <memory>

// 此代码模块中包含的函数的前向声明: 
void rundMainLoop(int var);

bool initOpenGL(int argc,char** argv);

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_FPS = 60;
const char* WINDOW_TITLE = "OPEN GL";

std::shared_ptr<OpenGL::RenderSample> Sample ;

void OnRender() {
	glClearColor(1, 0, 0, 0);

	glClear(GL_COLOR_BUFFER_BIT);

	if(Sample) Sample->OnRender();

	glutSwapBuffers();
}

int main(int argc,char** argv)
{
	//Raster::RenderSample* Sample = new Raster::DrawPoint(Width, Height);
	//Raster::RenderSample* Sample = new Raster::DrawLine(Width, Height);
	//Raster::RenderSample* Sample = new Raster::DrawCircle(Width, Height);
	//Raster::RenderSample* Sample = new Raster::DrawCircle1(Width, Height);
	//Raster::RenderSample* Sample = new Raster::DrawBezierCurve(Width, Height);
	//Raster::RenderSample* Sample = new Raster::DrawTriangle(Width, Height);
	//Raster::RenderSample* Sample = new Raster::DrawColorLine(Width, Height);
	//Raster::RenderSample* Sample = new Raster::DrawColorTriangle(Width, Height);
	//Raster::RenderSample* Sample = new Raster::DrawImage(Width, Height);
	//Raster::RenderSample* Sample = new Raster::DrawTextureTriangle(Width, Height);

	initOpenGL(argc,argv);

	Sample = std::shared_ptr<OpenGL::RenderSample>(new OpenGL::DrawQuad);

	Sample->OnInit();

	glutMainLoop();
	
	return 0;
}
void rundMainLoop(int var)
{
	//更新logic

	//渲染
	OnRender();
}
bool initOpenGL(int argc,char** argv) {

	glutInit(&argc, argv);

	glutInitWindowPosition(100, 100);
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);

	glutCreateWindow(WINDOW_TITLE);

	glutDisplayFunc(OnRender);

	glutTimerFunc(1000 / SCREEN_FPS, rundMainLoop, 1);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	return true;
}
