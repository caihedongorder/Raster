#pragma once
#include "ColorBuffer.hpp"


namespace Raster
{
	class RenderSample
	{
	public:
		RenderSample(){}

		virtual void OnRender(ColorBuffer& InColorBuffer){}
	};
}
