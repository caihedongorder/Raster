#include "scene.h"
#include "Samples/DrawPointSample.hpp"
#include "Samples/DrawLineSample.hpp"
#include "Samples/DrawCircleSample.hpp"
#include "Samples/DrawCircleSample1.hpp"
#include "Samples/DrawBezierCurve.hpp"
#include "Samples/DrawTriangleSample.hpp"
#include "Samples/DrawColorLineSample.hpp"
#include "Samples/DrawColorTriangleSample.hpp"
#include "Samples/DrawImageSample.hpp"
#include "Samples/DrawTextureTriangle.hpp"
#include <mathfu/glsl_mappings.h>
#include "Samples/OpenGLDrawQuad.hpp"
#include "Samples/OpenGLViewport.hpp"
#include "Samples/OpenGLTranslate.hpp"
#include "Samples/OpenGLDrawTexture.hpp"
#include "Samples/OpenGLDrawCube.hpp"
#include "Samples/OpenGLDrawSolarSystem.hpp"
#include "Samples/OpenGLDrawMipmap.hpp"
#include "Samples/OpenGLDrawPointSprite.hpp"
#include "Samples/OpenGLDrawCubeVBO.hpp"
#include "Samples/GLSLDrawQuad.hpp"
#include "GameTime.hpp"


std::shared_ptr<OpenGL::RenderSample> Sample ;

void Init()
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

	//initOpenGL(argc,argv);

	//Sample = std::shared_ptr<OpenGL::RenderSample>(new OpenGL::DrawQuad);
	//Sample = std::shared_ptr<OpenGL::RenderSample>(new OpenGL::Viewport(SCREEN_WIDTH,SCREEN_HEIGHT));
	//Sample = std::shared_ptr<OpenGL::RenderSample>(new OpenGL::Translate(SCREEN_WIDTH,SCREEN_HEIGHT));
	//Sample = std::shared_ptr<OpenGL::RenderSample>(new OpenGL::DrawTexture(SCREEN_WIDTH,SCREEN_HEIGHT));
	//Sample = std::shared_ptr<OpenGL::RenderSample>(new OpenGL::DrawCube(SCREEN_WIDTH,SCREEN_HEIGHT));
	//Sample = std::shared_ptr<OpenGL::RenderSample>(new OpenGL::DrawSolarSystem(SCREEN_WIDTH,SCREEN_HEIGHT));
	//Sample = std::shared_ptr<OpenGL::RenderSample>(new OpenGL::DrawMipmap(SCREEN_WIDTH,SCREEN_HEIGHT));
	//Sample = std::shared_ptr<OpenGL::RenderSample>(new OpenGL::DrawPointSprite(SCREEN_WIDTH,SCREEN_HEIGHT));
	//Sample = std::shared_ptr<OpenGL::RenderSample>(new OpenGL::DrawCubeVBO(SCREEN_WIDTH,SCREEN_HEIGHT));
	Sample = std::shared_ptr<OpenGL::RenderSample>(new OpenGL::GLSLDrawQuad());
	Sample->OnInit();
}

void SetViewportSize(float width, float height)
{

}

Raster::GameTime GGameTime;
void Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	GGameTime.Update();
	float deltaTime = GGameTime.GetDeltaTime();
	
	if(Sample) Sample->OnRender(GGameTime.GetDeltaTime());

}
