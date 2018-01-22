#pragma once
#include "DrawSample.hpp"
#include "Math.hpp"

namespace Raster
{
	class DrawCircle1 :public RenderSample
	{
	public:
		DrawCircle1(int InW,int InH){
			Width = InW;
			Height = InH;

			Vector2dInt center = { (int)(InW*0.5f),(int)(InH * 0.5f) };
			float Radius = 200;
			for (int i = 0 ; i <= 360 ; i+=1 )
			{
				points[i] = Vector2dInt(center.x + Radius * Math::Cos(Math::Degree2Radian(i)), center.y + Radius * Math::Sin(Math::Degree2Radian(i)));
			}
		}

		void OnRender(ColorBuffer& InColorBuffer)
		{
			InColorBuffer.Clear();

			InColorBuffer.DrawArrays(DM_LINE_STRIP, points, pointCount);
		}
	private:
		int Width, Height;
		int pointCount = 361;
		Vector2dInt points[361];
	};
}
