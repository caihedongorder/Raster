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
		}

		void OnRender(ColorBuffer& InColorBuffer)
		{
			InColorBuffer.Clear();

			InColorBuffer.DrawTriangle(pt1, pt2, pt3);
			InColorBuffer.DrawTriangle(pt4, pt5, pt6);
		}
	private:
		int Width, Height;
		Vector2dInt pt1;
		Vector2dInt pt2;
		Vector2dInt pt3;
		Vector2dInt pt4;
		Vector2dInt pt5;
		Vector2dInt pt6;
	};
}
