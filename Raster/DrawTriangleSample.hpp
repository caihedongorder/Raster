#pragma once
#include "DrawSample.hpp"
#include "Math.hpp"

namespace Raster
{
	class DrawTriangle :public RenderSample
	{
	public:
		DrawTriangle(int InW,int InH){
			Width = InW;
			Height = InH;
			pt1 = Vector2dInt(0, 100);
			pt2 = Vector2dInt(100, 100);
			pt3 = Vector2dInt(50, 0);

			pt4 = Vector2dInt(100, 100);
			pt5 = Vector2dInt(150, 0);
			pt6 = Vector2dInt(200, 200);

			pt7 = Vector2dInt(200, 10);
			pt8 = Vector2dInt(600, 10);
			pt9 = Vector2dInt(400, 0);
		}

		void OnRender(ColorBuffer& InColorBuffer)
		{
			InColorBuffer.Clear();

			InColorBuffer.DrawTriangle(pt1, pt2, pt3);
			InColorBuffer.DrawTriangle(pt4, pt5, pt6);
			InColorBuffer.DrawTriangle(pt7, pt8, pt9);
		}
	private:
		int Width, Height;
		Vector2dInt pt1;
		Vector2dInt pt2;
		Vector2dInt pt3;
		Vector2dInt pt4;
		Vector2dInt pt5;
		Vector2dInt pt6;
		Vector2dInt pt7;
		Vector2dInt pt8;
		Vector2dInt pt9;
	};
}
