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

			triangle1[0] = int2(0, 100);
			triangle1[1] = int2(100, 100);
			triangle1[2] = int2(50, 0);

			triangle2[0] = int2(100, 100);			triangle2[1] = int2(150, 0);			triangle2[2] = int2(200, 200);
			triangle3[0] = int2(200, 10);			triangle3[1] = int2(600, 10);			triangle3[2] = int2(400, 0);
		}

		void OnRender(ColorBuffer& InColorBuffer)
		{
			InColorBuffer.Clear();

			InColorBuffer.VertexPointer(2, VET_INT, 0, triangle1);
			InColorBuffer.DrawArrays(DM_TRIANGLS, 0, 3);
			InColorBuffer.VertexPointer(2, VET_INT, 0, triangle2);
			InColorBuffer.DrawArrays(DM_TRIANGLS, 0, 3);
			InColorBuffer.VertexPointer(2, VET_INT, 0, triangle3);
			InColorBuffer.DrawArrays(DM_TRIANGLS, 0, 3);
		}
	private:
		int Width, Height;
		int2 triangle1[3];
		int2 triangle2[3];
		int2 triangle3[3];
	};
}
