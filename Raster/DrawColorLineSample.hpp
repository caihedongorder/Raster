#pragma once
#include "DrawSample.hpp"
#include "Math.hpp"

namespace Raster
{
	class DrawColorLine :public RenderSample
	{
	public:
		DrawColorLine(int InW,int InH){
			Width = InW;
			Height = InH;
		}

		void OnRender(ColorBuffer& InColorBuffer)
		{
			InColorBuffer.Clear();

			int2 points[] = {
				int2(420, 300),int2(620, 300),
				int2(420, 300),int2(620, 400),
				int2(420, 300),int2(620, 200),
				int2(420, 300),int2(220, 300),
				int2(420, 300),int2(220, 400),
				int2(420, 300),int2(220, 200),


				int2(420, 300),int2(420, 600),
				int2(420, 300),int2(620, 600),
				int2(420, 300),int2(220, 600),
				int2(420, 300),int2(220, 0),
				int2(420, 300),int2(620, 0),
				int2(420, 300),int2(420, 0),
			};
			float4 colors[] = {
				float4(1.0f,0.0f,0.0f,1.0f),float4(0.0f,1.0f,0.0f,1.0f),
				float4(1.0f,0.0f,0.0f,1.0f),float4(0.0f,1.0f,0.0f,1.0f),
				float4(1.0f,0.0f,0.0f,1.0f),float4(0.0f,1.0f,0.0f,1.0f),
				float4(1.0f,0.0f,0.0f,1.0f),float4(0.0f,1.0f,0.0f,1.0f),
				float4(1.0f,0.0f,0.0f,1.0f),float4(0.0f,1.0f,0.0f,1.0f),
				float4(1.0f,0.0f,0.0f,1.0f),float4(0.0f,1.0f,0.0f,1.0f),

				float4(1.0f,0.0f,0.0f,1.0f),float4(0.0f,1.0f,0.0f,1.0f),
				float4(1.0f,0.0f,0.0f,1.0f),float4(0.0f,1.0f,0.0f,1.0f),
				float4(1.0f,0.0f,0.0f,1.0f),float4(0.0f,1.0f,0.0f,1.0f),
				float4(1.0f,0.0f,0.0f,1.0f),float4(0.0f,1.0f,0.0f,1.0f),
				float4(1.0f,0.0f,0.0f,1.0f),float4(0.0f,1.0f,0.0f,1.0f),
				float4(1.0f,0.0f,0.0f,1.0f),float4(0.0f,1.0f,0.0f,1.0f),
			};

			InColorBuffer.VertexPointer(2, VET_INT, 0, points);
			InColorBuffer.ColorPointer(4, VET_FLOAT, 0, colors);
			InColorBuffer.DrawArrays(DM_LINES, 0 , sizeof(points) / sizeof(points[0]));

			//Vector2dInt points1[] = {
			//	Vector2dInt(620, 400),Vector2dInt(420, 300),
			//	Vector2dInt(620, 200),Vector2dInt(420, 300),
			//	Vector2dInt(620, 300),Vector2dInt(420, 300),
			//};
			//InColorBuffer.DrawArrays(DM_LINES, points1, sizeof(points1) / sizeof(points1[0]));
		}
	private:
		int Width, Height;
	};
}
