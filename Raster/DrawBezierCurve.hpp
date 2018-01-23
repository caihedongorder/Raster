#pragma once
#include "DrawSample.hpp"
#include "Math.hpp"

namespace Raster
{
	class DrawBezierCurve :public RenderSample
	{
	public:
		DrawBezierCurve(int InW,int InH){
			Width = InW;
			Height = InH;

			P0 = { 100,100 };
			P3 = { 400,100 };
			P1 = { 200,200 };
			P2 = { 300,0 };
			int i = 0;
			for (float t = 0 ; i < pointCount ; i+=1,t+=0.01 )
			{
				points[i].x = P0.x*Math::Pow(1 - t, 3) +
					3 * P1.x*t*Math::Pow(1.0f - t, 2) +
					3 * P2.x*t*t*(1.0f - t)+
					P3.x*t*t*t;
				points[i].y = P0.y*Math::Pow(1 - t, 3) +
					3 * P1.y*t*Math::Pow(1.0f - t, 2) +
					3 * P2.y*t*t*(1.0f - t)+
					P3.y*t*t*t;
			}
		}

		void OnRender(ColorBuffer& InColorBuffer)
		{
			InColorBuffer.Clear();

			InColorBuffer.VertexPointer(2, VET_INT, 0, points);
			InColorBuffer.DrawArrays(DM_LINE_STRIP, 0, pointCount);
		}
	private:
		int Width, Height;
		int pointCount = 100;
		Vector2dInt P0;
		Vector2dInt P1;
		Vector2dInt P2;
		Vector2dInt P3;
		Vector2dInt points[100];
	};
}
