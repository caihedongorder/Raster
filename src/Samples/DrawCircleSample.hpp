#pragma once
#include "DrawSample.hpp"
#include "Math.hpp"

namespace Raster
{
	class DrawCircle :public RenderSample
	{
	public:
		DrawCircle(int InW,int InH){
			Width = InW;
			Height = InH;

			int2 center = { 100,100 };
			float Radius = 80;
			for (int i = 0 ; i < 360 ; ++i )
			{
				points[i] = int2(center.x + Radius * Math::Cos(Math::Degree2Radian(i)), center.y + Radius * Math::Sin(Math::Degree2Radian(i)));
			}
		}

		void OnRender(ColorBuffer& InColorBuffer)
		{
			InColorBuffer.Clear();

			InColorBuffer.VertexPointer(2, VET_INT, 0, points);
			InColorBuffer.DrawArrays(DM_LINE_LOOP, 0, 360);
		}
	private:
		int Width, Height;
		int2 points[360];
	};
}
