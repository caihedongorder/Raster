#pragma once
#include "DrawOpenGLSample.hpp"
#include "image.hpp"
#include "Vector.hpp"

namespace OpenGL
{
	class DrawTexture : public RenderSample
	{
	public:
		DrawTexture(int width,int height){
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

			glMatrixMode(GL_PROJECTION);
			glOrtho(0, mWidth, mHeight, 0, -1, 1.0f);
		}
		void OnRender(){
			DrawRect(mWidth*0.25f, mHeight*0.25f, mWidth*0.5f, mHeight*0.5f,mathfu::vec4(1,1,1,1));
			//glRasterPos2i(100,479);
			//glRasterPos2i(0,0);
			//glDrawPixels(img.GetWidth(), img.GetHeight(), GL_RGBA, GL_UNSIGNED_BYTE, img.getData());
		}
	private:
		void DrawRect(int l, int t, int w, int h,const mathfu::vec4& color)
		{
			//glMatrixMode(GL_TEXTURE);
			//glLoadIdentity();

			Raster::float2 pts[] = {
				Raster::float2(l,t),
				Raster::float2(l + w,t),
				Raster::float2(l + w,t + h),
				Raster::float2(l,t + h),
			};
			Raster::float2 texCoords[] = {
				Raster::float2(0,1.0f),
				Raster::float2(1.0f,1.0f),
				Raster::float2(1.0f,0.0f),
				Raster::float2(0.0f,0.0f),
			};

			glBindTexture(GL_TEXTURE_2D, texture);
#if 0
			glBegin(GL_QUADS);
				glVertex2f(l, t);
				glTexCoord2f(0.0, 1.0f);
				glVertex2f(l+w, t);
				glTexCoord2f(1.0, 1.0f);
				glVertex2f(l+w, t+h);
				glTexCoord2f(1.0, 0.0f);
				glVertex2f(l, t+h);
				glTexCoord2f(0.0, 0.0f);
			glEnd();
#else
			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);

			glVertexPointer(2, GL_FLOAT, 0, pts);
			glTexCoordPointer(2, GL_FLOAT, 0, texCoords);
			glDrawArrays(GL_QUADS, 0, sizeof(pts) / sizeof(pts[0]));
#endif


			//glDisableClientState(GL_VERTEX_ARRAY);
			//glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			//glColor3f(color.x, color.y, color.z);
		}
		void DrawRectwithViewport(int l, int t, int w, int h,const mathfu::vec4& color)
		{
			glViewport(l, (mHeight - t) - h, w, h);
			glColor3f(color.x, color.y, color.z);
			glBegin(GL_QUADS);
				glVertex2f(0,0);
				glVertex2f(mWidth,0);
				glVertex2f(mWidth,mHeight);
				glVertex2f(0,mHeight);
			glEnd();
		}
	private:
		int mWidth, mHeight;
		GLuint texture;
		Raster::Image img;
	};
}
