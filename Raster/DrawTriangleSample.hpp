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

			triangle1[0] = Vector2dInt(0, 100);
			triangle1[1] = Vector2dInt(100, 100);
			triangle1[2] = Vector2dInt(50, 0);

			triangle2[0] = Vector2dInt(100, 100);			triangle2[1] = Vector2dInt(150, 0);			triangle2[2] = Vector2dInt(200, 200);
			triangle3[0] = Vector2dInt(200, 10);			triangle3[1] = Vector2dInt(600, 10);			triangle3[2] = Vector2dInt(400, 0);
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
		Vector2dInt triangle1[3];
		Vector2dInt triangle2[3];
		Vector2dInt triangle3[3];
	};
}
