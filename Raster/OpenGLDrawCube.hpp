#pragma once
#include "DrawOpenGLSample.hpp"
#include "image.hpp"
#include "Vector.hpp"

namespace OpenGL
{
	class DrawCube : public RenderSample
	{
	public:
		DrawCube(int width,int height){
			mWidth = width;
			mHeight = height;
		}
		void OnInit() {
			glEnable( GL_TEXTURE_2D );

			img = Raster::Image::LoadFromFile("images/tiger.png");
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.GetWidth(), img.GetHeight(), 0, GL_RGBA	, GL_UNSIGNED_BYTE, img.getData());
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);  
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);  
			glBindTexture(GL_TEXTURE_2D, 0);




			glClearColor(0, 0, 0, 0);

#if 0
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(0, mWidth, mHeight, 0, 100, -100.0f);
#else

			glEnable(GL_CULL_FACE);
			
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			gluLookAt(100, 0, 1000, 0, 0, 0, 0, 1, 0);

			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluPerspective(60, mWidth*1.0f / mHeight, 1, 1200);
#endif
			RotationAngle = 0.0f;

			CamYMaxValue = 450;
			CamY = 300;
			CamChangeSpeed = -CamYMaxValue * 0.016f;


			//glFrontFace(GL_CW);
		}
		void OnRender(){
			DrawRect(-mWidth*0.25f, mHeight*0.25f, mWidth*0.5f, mHeight*0.5f,mathfu::vec4(1,1,1,1));
			//glRasterPos2i(100,479);
			//glRasterPos2i(0,0);
			//glDrawPixels(img.GetWidth(), img.GetHeight(), GL_RGBA, GL_UNSIGNED_BYTE, img.getData());
		}
	private:
		void DrawRect(int l, int t, int w, int h,const mathfu::vec4& color)
		{
			//glMatrixMode(GL_TEXTURE);
			//glLoadIdentity();
			RotationAngle += 0.016*60;
			CamY += CamChangeSpeed;
			if (Raster::Math::Abs(CamY) > CamYMaxValue)
			{
				CamChangeSpeed = -CamChangeSpeed;
				CamY = Raster::Math::Clamp(CamY, -CamYMaxValue, CamYMaxValue);
			}
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			gluLookAt(0,CamY, 800, 0, 0, 0, 0, 1, 0);
			glRotatef(RotationAngle, 0, 1, 0);


			Raster::float3 pts[] = {
				//front
				Raster::float3(l,t,w*0.5f),
				Raster::float3(l + w,t - h ,w*0.5f),
				Raster::float3(l + w,t,w*0.5f),

				Raster::float3(l,t,w*0.5f),
				Raster::float3(l,t - h,w*0.5f),
				Raster::float3(l + w ,t - h ,w*0.5f),

				//back
				Raster::float3(l,t,-w*0.5f),
				Raster::float3(l + w,t,-w*0.5f),
				Raster::float3(l + w,t - h ,-w*0.5f),

				Raster::float3(l,t,-w*0.5f),
				Raster::float3(l + w ,t - h ,-w*0.5f),
				Raster::float3(l,t - h,-w*0.5f),

				//right
				Raster::float3(l + w,t,w*0.5f),
				Raster::float3(l + w,t-h,-w*0.5f),
				Raster::float3(l + w,t,-w*0.5f),

				Raster::float3(l + w,t,w*0.5f),
				Raster::float3(l + w,t-h,w*0.5f),
				Raster::float3(l + w,t-h,-w*0.5f),

				//left
				Raster::float3(l ,t,w*0.5f),
				Raster::float3(l ,t,-w*0.5f),
				Raster::float3(l ,t-h,-w*0.5f),

				Raster::float3(l ,t,w*0.5f),
				Raster::float3(l ,t-h,-w*0.5f),
				Raster::float3(l ,t-h,w*0.5f),

				//top
				Raster::float3(l,t,w*0.5f),
				Raster::float3(l+w,t,w*0.5f),
				Raster::float3(l,t,-w*0.5f),

				Raster::float3(l+w,t,w*0.5f),
				Raster::float3(l+w,t,-w*0.5f),
				Raster::float3(l,t,-w*0.5f),

				//bottom
				Raster::float3(l,t-h,w*0.5f),
				Raster::float3(l,t-h,-w*0.5f),
				Raster::float3(l+w,t-h,w*0.5f),

				Raster::float3(l+w,t-h,w*0.5f),
				Raster::float3(l,t-h,-w*0.5f),
				Raster::float3(l+w,t-h,-w*0.5f),
			};
			Raster::float2 texCoords[] = {
				//front
				Raster::float2(0.0,1.0f),
				Raster::float2(1.0f,0.0f),
				Raster::float2(1.0f,1.0f),

				Raster::float2(0.0f,1.0f),
				Raster::float2(0.0f,0.0f),
				Raster::float2(1.0f,0.0f),

				//back
				Raster::float2(0.0,1.0f),
				Raster::float2(1.0f,1.0f),
				Raster::float2(1.0f,0.0f),

				Raster::float2(0.0f,1.0f),
				Raster::float2(1.0f,0.0f),
				Raster::float2(0.0f,0.0f),

				//right
				Raster::float2(0.0,1.0f),
				Raster::float2(1.0f,0.0f),
				Raster::float2(1.0f,1.0f),

				Raster::float2(0.0f,1.0f),
				Raster::float2(0.0f,0.0f),
				Raster::float2(1.0f,0.0f),

				//left
				Raster::float2(0.0,1.0f),
				Raster::float2(1.0f,1.0f),
				Raster::float2(1.0f,0.0f),

				Raster::float2(0.0f,1.0f),
				Raster::float2(1.0f,0.0f),
				Raster::float2(0.0f,0.0f),

				//top
				Raster::float2(0.0,0.0f),
				Raster::float2(1.0f,0.0f),
				Raster::float2(0.0f,1.0f),

				Raster::float2(1.0f,0.0f),
				Raster::float2(1.0f,1.0f),
				Raster::float2(0.0f,1.0f),

				//bottom
				Raster::float2(0.0,0.0f),
				Raster::float2(0.0f,1.0f),
				Raster::float2(1.0f,0.0f),

				Raster::float2(1.0f,0.0f),
				Raster::float2(0.0f,1.0f),
				Raster::float2(1.0f,1.0f),
			};

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glBindTexture(GL_TEXTURE_2D, texture);
			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);

			glVertexPointer(3, GL_FLOAT, 0, pts);
			glTexCoordPointer(2, GL_FLOAT, 0, texCoords);
			glDrawArrays(GL_TRIANGLES, 0, sizeof(pts) / sizeof(pts[0]));
			//glDrawArrays(GL_TRIANGLES, 0, 3);

			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			glDisable(GL_BLEND);
		}
	private:
		int mWidth, mHeight;
		GLuint texture;
		Raster::Image img;
		float RotationAngle;
		float CamY;
		float CamYMaxValue;
		float CamChangeSpeed;
	};
}
