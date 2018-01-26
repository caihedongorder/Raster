#pragma once
#include "LOpenGL.h"

namespace OpenGL
{
	class RenderSample 
	{
	public:
		RenderSample(){}
		virtual void OnInit(){}
		virtual void OnRender(){
		}
	};
}
