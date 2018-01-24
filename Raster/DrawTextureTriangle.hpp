#pragma once
#include "DrawSample.hpp"
#include "Math.hpp"

namespace Raster
{
	class DrawTextureTriangle :public RenderSample
	{
	public:
		DrawTextureTriangle(int InW,int InH){
			Width = InW;
			Height = InH;

			triangle[0] = Vector2dInt(InW * 0.5f,0);
			triangle[1] = Vector2dInt(0,InH);
			triangle[2] = Vector2dInt(InW,InH);

			colors[0] = Vector4dFloat(1.0f, 0, 0, 1.0f);
			colors[1] = Vector4dFloat(0.0f, 1.0f, 0, 1.0f);
			colors[2] = Vector4dFloat(0.0f, 0, 1.0f, 1.0f);

			texcoords[0] = Vector2dFloat(0.5f,0.0f);			texcoords[1] = Vector2dFloat(0.0f,1.0f);			texcoords[2] = Vector2dFloat(1.0f,1.0f);

			Tex = Image::LoadFromFile("images/pubu.jpg");
		
		}

		void OnRender(ColorBuffer& InColorBuffer)
		{
			AUTO_PROFILE(TEXT("DrawColorTriangle"))

			InColorBuffer.Clear();

			InColorBuffer.SetTexture(Tex);

			InColorBuffer.VertexPointer(2, VET_INT, 0, triangle);
			//InColorBuffer.ColorPointer(4, VET_FLOAT, 0, colors);
			InColorBuffer.TexcoordPointer(2, VET_FLOAT, 0, texcoords);
			
			InColorBuffer.DrawArrays(DM_TRIANGLS, 0, 3);
		}
	private:
		int Width, Height;
		Vector2dInt triangle[3];
		Vector4dFloat colors[3];
		Vector2dFloat texcoords[3];
		Image Tex;
	};
}
