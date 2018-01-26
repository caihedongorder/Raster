#pragma once
#include "DrawOpenGLSample.hpp"

namespace OpenGL
{
	class Viewport : public RenderSample
	{
	public:
		Viewport(int width,int height){
			mWidth = width;
			mHeight = height;
		}
		void OnInit() {
			glClearColor(0, 0, 0, 0);

			glMatrixMode(GL_PROJECTION);
			glOrtho(0, mWidth, mHeight, 0, -1, 1.0f);
		}
		void OnRender(){
			//DrawRect(mWidth*0.1f, mHeight*0.1f, mWidth*0.25f, mHeight*0.25f,mathfu::vec4(1,0,0,0));
			//DrawRect(mWidth*0.1f, mHeight*0.65f, mWidth*0.25f, mHeight*0.25f,mathfu::vec4(0,1,0,0));
			//DrawRect(mWidth*0.65f, mHeight*0.1f, mWidth*0.25f, mHeight*0.25f,mathfu::vec4(0,0,1,0));
			//DrawRect(mWidth*0.65f, mHeight*0.65f, mWidth*0.25f, mHeight*0.25f,mathfu::vec4(1,1,0,0));

			DrawRectwithViewport(mWidth*0.1f, mHeight*0.1f, mWidth*0.25f, mHeight*0.25f,mathfu::vec4(1,0,0,0));
			DrawRectwithViewport(mWidth*0.1f, mHeight*0.65f, mWidth*0.25f, mHeight*0.25f,mathfu::vec4(0,1,0,0));
			DrawRectwithViewport(mWidth*0.65f, mHeight*0.1f, mWidth*0.25f, mHeight*0.25f,mathfu::vec4(0,0,1,0));
			DrawRectwithViewport(mWidth*0.65f, mHeight*0.65f, mWidth*0.25f, mHeight*0.25f,mathfu::vec4(1,1,0,0));

		}
	private:
		void DrawRect(int l, int t, int w, int h,const mathfu::vec4& color)
		{
			glColor3f(color.x, color.y, color.z);
			glBegin(GL_QUADS);
				glVertex2f(l,t);
				glVertex2f(l+w,t);
				glVertex2f(l+w,t+h);
				glVertex2f(l,t+h);
			glEnd();
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
	};
}
