#pragma once
#include "DrawSample.hpp"
#include "Math.hpp"
#include "Image.hpp"

namespace Raster
{
	class DrawImage :public RenderSample
	{
	public:
		DrawImage(int InW,int InH){
			Width = InW;
			Height = InH;

			triangle[0] = int2(InW * 0.5f,0);
			triangle[1] = int2(0,InH);
			triangle[2] = int2(InW,InH);

			colors[0] = float4(1.0f, 0, 0, 1.0f);
			colors[1] = float4(0.0f, 1.0f, 0, 1.0f);
			colors[2] = float4(0.0f, 0, 1.0f, 1.0f);

			Img = Image::LoadFromFile("images/tiger.png");
			ImgBG = Image::LoadFromFile("images/pubu.jpg");
		}

		void OnRender(ColorBuffer& InColorBuffer)
		{
			AUTO_PROFILE(TEXT("DrawColorTriangle"))

			InColorBuffer.Clear();

			//InColorBuffer.DrawImage(100, 50, &Img);
			//InColorBuffer.DrawImageWithColorKey(100, 50, &Img,RGBA(0xffffff));
			//InColorBuffer.DrawImageAlphaTest(100, 50, &Img,0);
			//InColorBuffer.DrawImage(0, 0, &ImgBG);
			//InColorBuffer.DrawImageAlphaBlend(100, 50, &Img);
			//InColorBuffer.DrawImage(100, 50,2000,2000, &Img);
			InColorBuffer.DrawImage(100, 50,200,200, &Img);

		}
	private:
		int Width, Height;
		int2 triangle[3];
		float4 colors[3];
		Image ImgBG;
		Image Img;
	};
}
