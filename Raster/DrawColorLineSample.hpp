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

			Vector2dInt points[] = {
				Vector2dInt(420, 300),Vector2dInt(620, 300),
				Vector2dInt(420, 300),Vector2dInt(620, 400),
				Vector2dInt(420, 300),Vector2dInt(620, 200),
				Vector2dInt(420, 300),Vector2dInt(220, 300),
				Vector2dInt(420, 300),Vector2dInt(220, 400),
				Vector2dInt(420, 300),Vector2dInt(220, 200),


				Vector2dInt(420, 300),Vector2dInt(420, 600),
				Vector2dInt(420, 300),Vector2dInt(620, 600),
				Vector2dInt(420, 300),Vector2dInt(220, 600),
				Vector2dInt(420, 300),Vector2dInt(220, 0),
				Vector2dInt(420, 300),Vector2dInt(620, 0),
				Vector2dInt(420, 300),Vector2dInt(420, 0),
			};
			Vector4dFloat colors[] = {
				Vector4dFloat(1.0f,0.0f,0.0f,1.0f),Vector4dFloat(0.0f,1.0f,0.0f,1.0f),
				Vector4dFloat(1.0f,0.0f,0.0f,1.0f),Vector4dFloat(0.0f,1.0f,0.0f,1.0f),
				Vector4dFloat(1.0f,0.0f,0.0f,1.0f),Vector4dFloat(0.0f,1.0f,0.0f,1.0f),
				Vector4dFloat(1.0f,0.0f,0.0f,1.0f),Vector4dFloat(0.0f,1.0f,0.0f,1.0f),
				Vector4dFloat(1.0f,0.0f,0.0f,1.0f),Vector4dFloat(0.0f,1.0f,0.0f,1.0f),
				Vector4dFloat(1.0f,0.0f,0.0f,1.0f),Vector4dFloat(0.0f,1.0f,0.0f,1.0f),

				Vector4dFloat(1.0f,0.0f,0.0f,1.0f),Vector4dFloat(0.0f,1.0f,0.0f,1.0f),
				Vector4dFloat(1.0f,0.0f,0.0f,1.0f),Vector4dFloat(0.0f,1.0f,0.0f,1.0f),
				Vector4dFloat(1.0f,0.0f,0.0f,1.0f),Vector4dFloat(0.0f,1.0f,0.0f,1.0f),
				Vector4dFloat(1.0f,0.0f,0.0f,1.0f),Vector4dFloat(0.0f,1.0f,0.0f,1.0f),
				Vector4dFloat(1.0f,0.0f,0.0f,1.0f),Vector4dFloat(0.0f,1.0f,0.0f,1.0f),
				Vector4dFloat(1.0f,0.0f,0.0f,1.0f),Vector4dFloat(0.0f,1.0f,0.0f,1.0f),
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
