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

			Vector2dInt center = { 100,100 };
			float Radius = 80;
			for (int i = 0 ; i < 360 ; ++i )
			{
				points[i] = Vector2dInt(center.x + Radius * Math::Cos(Math::Degree2Radian(i)), center.y + Radius * Math::Sin(Math::Degree2Radian(i)));
			}
		}

		void OnRender(ColorBuffer& InColorBuffer)
		{
			InColorBuffer.Clear();

			InColorBuffer.DrawArrays(DM_LINE_LOOP, points, 360);
		}
	private:
		int Width, Height;
		Vector2dInt points[360];
	};
}
