#pragma once
#include "DrawOpenGLSample.hpp"
#include "image.hpp"
#include "Vector.hpp"

namespace OpenGL
{
	class DrawSolarSystem : public RenderSample
	{
	private:
		struct Vectex
		{
			Raster::float3 position;
			Raster::float4 color;
		};
	public:
		DrawSolarSystem(int width,int height){
			mWidth = width;
			mHeight = height;
		}
		void OnInit() {
			glEnable( GL_TEXTURE_2D );

			glClearColor(0, 0, 0, 0);

#if 0
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(0, mWidth, mHeight, 0, 100, -100.0f);
#else

			glEnable(GL_CULL_FACE);
			

			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluPerspective(60, mWidth*1.0f / mHeight, 1, 1200);
#endif
			CamYMaxValue = 5.0f;
			CamChangeSpeed = -CamYMaxValue;
			CamY = CamYMaxValue;

			EarthRotation = 0;
			MoonRotation = 0;

			//glFrontFace(GL_CW);
		}
		void OnRender(float InDeltaTime){
			
			RotationAngle += 60 * InDeltaTime;
			EarthRotation += 90 * InDeltaTime;
			MoonRotation += 360 * InDeltaTime;

			CamY += CamChangeSpeed*InDeltaTime;
			if (Raster::Math::Abs(CamY) > CamYMaxValue)
			{
				CamChangeSpeed = -CamChangeSpeed;
				CamY = Raster::Math::Clamp(CamY, -CamYMaxValue, CamYMaxValue);
			}

			DrawRect(-mWidth*0.25f, mHeight*0.25f, mWidth*0.5f, mHeight*0.5f,mathfu::vec4(1,1,1,1));
		}
	private:
		void DrawRect(int l, int t, int w, int h,const mathfu::vec4& color)
		{
			Vectex pts[] = {
				//front
				{ Raster::float3(-1.0f,1.0f,1.0f),Raster::float4(1,0,0,1)},
				{ Raster::float3(-1.0f,-1.0f,1.0f),Raster::float4(1,0,0,1)},
				{ Raster::float3(1.0f,-1.0f,1.0f) ,Raster::float4(1,0,0,1)},

				{ Raster::float3(-1.0f,1.0f,1.0f),Raster::float4(1,0,0,1)},
				{ Raster::float3(1.0f,-1.0f,1.0f),Raster::float4(1,0,0,1)},
				{ Raster::float3(1.0f,1.0f,1.0f),Raster::float4(1,0,0,1)},
					//back
				{ Raster::float3(-1.0f,1.0f,-1.0f),Raster::float4(1,1,0,1)},
				{ Raster::float3(1.0f,-1.0f,-1.0f),Raster::float4(1,1,0,1)},
				{ Raster::float3(-1.0f,-1.0f,-1.0f),Raster::float4(1,1,0,1)},

				{ Raster::float3(-1.0f,1.0f,-1.0f),Raster::float4(1,1,0,1)},
				{ Raster::float3(1.0f,1.0f,-1.0f),Raster::float4(1,1,0,1)},
				{ Raster::float3(1.0f,-1.0f,-1.0f),Raster::float4(1,1,0,1)},

				//right
				{ Raster::float3(1.0f,1.0f,1.0f),Raster::float4(1,0,1,1)},
				{ Raster::float3(1.0f,-1.0f,1.0f),Raster::float4(1,0,1,1)},
				{ Raster::float3(1.0f,-1.0f,-1.0f),Raster::float4(1,0,1,1)},

				{ Raster::float3(1.0f,1.0f,1.0f),Raster::float4(1,0,1,1)},
				{ Raster::float3(1.0f,-1.0f,-1.0f),Raster::float4(1,0,1,1)},
				{ Raster::float3(1.0f,1.0f,-1.0f),Raster::float4(1,0,1,1)},
				//left
				{ Raster::float3(-1.0f,1.0f,1.0f),Raster::float4(0,1,0,1)},
				{ Raster::float3(-1.0f,-1.0f,-1.0f),Raster::float4(0,1,0,1)},
				{ Raster::float3(-1.0f,-1.0f,1.0f),Raster::float4(0,1,0,1)},

				{ Raster::float3(-1.0f,1.0f,1.0f),Raster::float4(0,1,0,1)},
				{ Raster::float3(-1.0f,1.0f,-1.0f),Raster::float4(0,1,0,1)},
				{ Raster::float3(-1.0f,-1.0f,-1.0f),Raster::float4(0,1,0,1)},
				//top
				{ Raster::float3(-1.0f,1.0f,-1.0f),Raster::float4(0,0,1,1)},
				{ Raster::float3(-1.0f,1.0f,1.0f),Raster::float4(0,0,1,1)},
				{ Raster::float3(1.0f,1.0f,1.0f),Raster::float4(0,0,1,1)},

				{ Raster::float3(-1.0f,1.0f,-1.0f),Raster::float4(0,0,1,1)},
				{ Raster::float3(1.0f,1.0f,1.0f),Raster::float4(0,0,1,1)},
				{ Raster::float3(1.0f,1.0f,-1.0f),Raster::float4(0,0,1,1)},
				//bottom
				{ Raster::float3(-1.0f,-1.0f,-1.0f),Raster::float4(0,1,1,1)},
				{ Raster::float3(1.0f,-1.0f,1.0f),Raster::float4(0,1,1,1)},
				{ Raster::float3(-1.0f,-1.0f,1.0f),Raster::float4(0,1,1,1)},
				
				{ Raster::float3(-1.0f,-1.0f,-1.0f),Raster::float4(0,1,1,1)},
				{ Raster::float3(1.0f,-1.0f,-1.0f),Raster::float4(0,1,1,1)},
				{ Raster::float3(1.0f,-1.0f,1.0f),Raster::float4(0,1,1,1)},
			};

			glColor3f(1, 1, 1);

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			gluLookAt(0, 0, 5.0f, 0, 0, 0, 0, 1, 0);
			glRotated(RotationAngle, 0, 0, 1);
			glScaled(0.2f, 0.2f, 0.2f);


			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_COLOR_ARRAY);

			//绘制太阳
			glVertexPointer(3, GL_FLOAT, sizeof(Vectex), &pts[0].position);
			glColorPointer(4, GL_FLOAT, sizeof(Vectex), &pts[0].color);
			glDrawArrays(GL_TRIANGLES, 0, sizeof(pts) / sizeof(pts[0]));
			
			//绘制地球
			glTranslated(10.0f, 0, 0);
			glRotated(EarthRotation, 0, 0, 1);
			glScaled(0.5f, 0.5f, 0.5f);
			glDrawArrays(GL_TRIANGLES, 0, sizeof(pts) / sizeof(pts[0]));

			//绘制月亮
			glRotated(MoonRotation, 0, 0, 1);
			glTranslated(3.0f, 0, 0);
			glScaled(0.2f, 0.2f, 0.2f);
			glDrawArrays(GL_TRIANGLES, 0, sizeof(pts) / sizeof(pts[0]));

			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_COLOR_ARRAY);
			//glDrawArrays(GL_TRIANGLES, 0, 3);
		}
	private:
		int mWidth, mHeight;
		GLuint texture;
		Raster::Image img;
		float RotationAngle;
		float CamY;
		float CamYMaxValue;
		float CamChangeSpeed;
		float EarthRotation;
		float MoonRotation;
	};
}
