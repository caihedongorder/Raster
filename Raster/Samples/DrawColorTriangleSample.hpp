#pragma once
#include "DrawSample.hpp"
#include "Math.hpp"

namespace Raster
{
	class DrawColorTriangle :public RenderSample
	{
	public:
		DrawColorTriangle(int InW,int InH){
			Width = InW;
			Height = InH;

			triangle[0] = int2(InW * 0.5f,0);
			triangle[1] = int2(0,InH);
			triangle[2] = int2(InW,InH);

			colors[0] = float4(1.0f, 0, 0, 1.0f);
			colors[1] = float4(0.0f, 1.0f, 0, 1.0f);
			colors[2] = float4(0.0f, 0, 1.0f, 1.0f);
		}

		void OnRender(ColorBuffer& InColorBuffer)
		{
			AUTO_PROFILE(TEXT("DrawColorTriangle"))

			InColorBuffer.Clear();

			InColorBuffer.VertexPointer(2, VET_INT, 0, triangle);
			InColorBuffer.ColorPointer(4, VET_FLOAT, 0, colors);
			InColorBuffer.DrawArrays(DM_TRIANGLS, 0, 3);
		}
	private:
		int Width, Height;
		int2 triangle[3];
		float4 colors[3];
	};
}
