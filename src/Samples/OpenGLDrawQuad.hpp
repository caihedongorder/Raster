#pragma once
#include "DrawOpenGLSample.hpp"

namespace OpenGL
{
	class DrawQuad : public RenderSample
	{
	public:
		DrawQuad(){}
		void OnRender(float InDeltaTime){
			glBegin(GL_QUADS);
				glVertex2f(-0.5f, 0.5f);
				glVertex2f(0.5f, 0.5f);
				glVertex2f(0.5f, -0.5f);
				glVertex2f(-0.5f, -0.5f);
			glEnd();
		}
	};
}
