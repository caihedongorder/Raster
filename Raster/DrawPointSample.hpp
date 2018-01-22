#pragma once
#include "DrawSample.hpp"
#include "Math.hpp"

namespace Raster
{
	class DrawPoint :public RenderSample
	{
	public:
		DrawPoint(int InW,int InH){
			Width = InW;
			Height = InH;
		}

		void OnRender(ColorBuffer& InColorBuffer)
		{
			InColorBuffer.Clear();
			for (int i =0; i < 100;++i)
			{
				InColorBuffer.DrawArrays(DM_POINTS,&Vector2dInt(Math::RandInRange<int>(0, Width), Math::RandInRange<int>(0, Height)),1);
			}
		}
	private:
		int Width, Height;
	};
}
