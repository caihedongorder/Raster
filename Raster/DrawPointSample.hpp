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
				InColorBuffer.VertexPointer(2, VET_INT,0,&int2(Math::RandInRange<int>(0, Width), Math::RandInRange<int>(0, Height)));
				InColorBuffer.DrawArrays(DM_POINTS,0,1);
			}
		}
	private:
		int Width, Height;
	};
}
