#pragma once
#include <FreeImage.h>
#include "RGBA.hpp"
#include "Math.hpp"
#include "Vector.hpp"
#include "Profile.hpp"
#include "Image.hpp"

namespace Raster
{
	enum EDrawMode
	{
		DM_POINTS,
		DM_LINES,
		DM_LINE_STRIP,
		DM_LINE_LOOP,
		DM_TRIANGLS,
	};
	enum EVexterElementType
	{
		VET_BYTE,
		VET_SHORT,
		VET_FIXED,
		VET_FLOAT,
		VET_INT,
	};
	class ColorBuffer
	{
	private:
		struct DrawLineParams 
		{
			const int2* pt1;
			const int2* pt2;
			const float4 *color1;
			const float4 *color2;
			bool hasColor;
			const float2 *texcoord1;
			const float2 *texcoord2;
		};
	public:
		int GetWidth() const { return m_Width; }
		int GetHeight() const { return m_Height; }
	public:
		ColorBuffer(int width, int height, unsigned char* InBuffer)
			:m_Color(255,255,255,255)
		{
			m_Width = width;
			m_Height = height;
			m_Buffer = (RGBA*)InBuffer;
			memset(m_Buffer, 0, m_Width*m_Height * sizeof(RGBA));


			//初始化FreeImage库
			FreeImage_Initialise();
		}
		~ColorBuffer() {
			//卸载FreeImage库	
			FreeImage_DeInitialise();
		}
		void Clear(RGBA InColor = RGBA(0,0,0,0))
		{
			AUTO_PROFILE(_T("ColorBuffer.Clear"));
			for (int i = 0;i < m_Width*m_Height;++i)
			{
				m_Buffer[i] = InColor;
			}
			RGBA* srcColor = m_Buffer+100+ 91*m_Width;
			memset(m_Buffer, 0, m_Width*m_Height * sizeof(RGBA));
		}

		void DrawImage(int destX, int destY,int destWidth,int destHeight, Image* image)
		{
			int xStart = Math::Clamp(destX, 0, m_Width - 1);
			int yStart = Math::Clamp(destY, 0, m_Height - 1);

			int xEnd = Math::Clamp(destX + destWidth,	0, m_Width - 1);
			int yEnd = Math::Clamp(destY + destHeight,  0, m_Height - 1);

			float2 StartUV(0,0);
			float uStep = 1.0f / destWidth;
			float vStep = 1.0f / destHeight;

			for (int y = yStart; y <= yEnd; ++y )
			{
				StartUV.x = 0;
				for (int x = xStart ; x <= xEnd ; ++x )
				{
					RGBA srcColor = PixelAt(x, y);
					RGBA pixelColor = image->PixelFromUV(StartUV);
					RGBA blendColor = Math::Lerp(srcColor, pixelColor, pixelColor.A / 255.0f);
					_SetPixelEx(x, y, blendColor);
					StartUV.x += uStep;
				}
				StartUV.y += vStep;
			}
		}
		void DrawImage(int x, int y, Image* image)
		{
			int xStart = Math::Clamp(x, 0, m_Width - 1);
			int yStart = Math::Clamp(y, 0, m_Height - 1);

			int xEnd = Math::Clamp(x + image->GetWidth(), 0, m_Width - 1);
			int yEnd = Math::Clamp(y + image->GetHeight(), 0, m_Height - 1);

			for (int y = yStart; y <= yEnd; ++y )
			{
				for (int x = xStart ; x <= xEnd ; ++x )
				{
					_SetPixelEx(x, y, image->PixelAt(x - xStart,y - yStart));
				}
			}
		}
		void DrawImageWithColorKey(int x, int y, Image* image,RGBA InKey)
		{
			int xStart = Math::Clamp(x, 0, m_Width - 1);
			int yStart = Math::Clamp(y, 0, m_Height - 1);

			int xEnd = Math::Clamp(x + image->GetWidth(), 0, m_Width - 1);
			int yEnd = Math::Clamp(y + image->GetHeight(), 0, m_Height - 1);

			for (int y = yStart; y <= yEnd; ++y )
			{
				for (int x = xStart ; x <= xEnd ; ++x )
				{
					RGBA pixelColor = image->PixelAt(x - xStart, y - yStart);
					if(InKey != pixelColor )
						_SetPixelEx(x, y, image->PixelAt(x - xStart,y - yStart));
				}
			}
		}
		void DrawImageAlphaTest(int x, int y, Image* image,float InAlpha)
		{
			int xStart = Math::Clamp(x, 0, m_Width - 1);
			int yStart = Math::Clamp(y, 0, m_Height - 1);

			int xEnd = Math::Clamp(x + image->GetWidth(), 0, m_Width - 1);
			int yEnd = Math::Clamp(y + image->GetHeight(), 0, m_Height - 1);

			for (int y = yStart; y <= yEnd; ++y )
			{
				for (int x = xStart ; x <= xEnd ; ++x )
				{
					RGBA pixelColor = image->PixelAt(x - xStart, y - yStart);
					if(pixelColor.A > InAlpha )
						_SetPixelEx(x, y, image->PixelAt(x - xStart,y - yStart));
					//_SetPixelEx(x, y, RGBA(pixelColor.A, pixelColor.A, pixelColor.A, pixelColor.A));
				}
			}
		}
		void DrawImageAlphaBlend(int x, int y, Image* image)
		{
			int xStart = Math::Clamp(x, 0, m_Width - 1);
			int yStart = Math::Clamp(y, 0, m_Height - 1);

			int xEnd = Math::Clamp(x + image->GetWidth(), 0, m_Width - 1);
			int yEnd = Math::Clamp(y + image->GetHeight(), 0, m_Height - 1);

			for (int y = yStart; y <= yEnd; ++y )
			{
				for (int x = xStart ; x <= xEnd ; ++x )
				{
					RGBA srcColor = PixelAt(x, y);
					RGBA pixelColor = image->PixelAt(x - xStart, y - yStart);
					RGBA blendColor = Math::Lerp(srcColor, pixelColor, pixelColor.A / 255.0f);
					_SetPixelEx(x, y,blendColor );
				}
			}
		}
		void SetTexture(Image InTexture) {
			mTexture = InTexture;
		}
		void TexcoordPointer(int ElementCount, EVexterElementType VET, int nStride, void* pTextureCoord)
		{
			m_TexcoordPointer = pTextureCoord;
			m_TexcoordStride = nStride == 0 ? EvaStride(VET,ElementCount) : nStride;
		}
		void VertexPointer(int ElementCount, EVexterElementType	VET, int nStride, void* pVertex)
		{
			m_VertexPointer = pVertex;
			m_VertexStride = nStride == 0 ? EvaStride(VET,ElementCount) : nStride;
		}
		void ColorPointer(int ElementCount, EVexterElementType VET, int nStride, void* pColor)
		{
			m_ColorPointer = pColor;
			m_ColorStride = nStride == 0 ? EvaStride(VET,ElementCount) : nStride;
		}
		void DrawArrays(EDrawMode InDrawMode, int FirstIndex, int PointCount)
		{
			int2* Points = (int2*)(m_VertexPointer);
			float4* Colors = (float4*)(m_ColorPointer);
			switch (InDrawMode)
			{
			case Raster::DM_POINTS:
				{
					for (int i = 0;i<PointCount;++i)
					{
						DrawPoint(GetElement<int2>(Points,i,m_VertexStride));
					}
				}
				break;
			case Raster::DM_LINES:
				{
					PointCount = PointCount / 2 * 2;
					for (int i = 0 ; i < PointCount ; i += 2 )
					{
						DrawLine(i, i + 1);
					}
				}
				break;
			case Raster::DM_LINE_STRIP:
				{
					if (PointCount < 2) return;
					for (int i = 0 ; i < PointCount - 1 ; ++i )
					{
						DrawLine(i, i + 1);
					}
				}
				break;
			case Raster::DM_LINE_LOOP:
				{
					if (PointCount <= 2) return;
					for (int i = 0 ; i < PointCount ; ++i )
					{
						if (i == PointCount - 1)
							DrawLine(0,PointCount-1);
						else 
							DrawLine(i,i+1);
					}
				}
				break;
			case Raster::DM_TRIANGLS:
				{
					if (PointCount <= 2) return;
					int TriangleCount = PointCount / 3;
					for (int i = 0 ; i < TriangleCount ; ++i)
					{
						DrawTriangle(i * 3, i * 3 + 1, i * 3 + 2);
					}
				}

				break;
			default:
				break;
			}
		}
	private:
		inline const int2* GetVertexPosition(int Index) {
			return GetElement<int2>(m_VertexPointer, Index, m_VertexStride);
		}
		inline RGBA PixelAt(int x, int y) {
			x = Math::Clamp(x, 0, m_Width);
			y = Math::Clamp(y, 0, m_Height);
			return m_Buffer[y*m_Width + x];
		}
		inline const float4* GetVertexColor(int Index) {
			return GetElement<float4>(m_ColorPointer, Index, m_ColorStride);
		}
		inline const float2* GetTexcoord(int Index) {
			return GetElement<float2>(m_TexcoordPointer, Index, m_TexcoordStride);
		}
		void DrawTriangle(int pt1,int pt2,int pt3)
		{
			int TopPoint	= pt1;
			int BottomPoint = pt2;
			int OtherPoint	= pt3;
			
			const int2* point1 = GetVertexPosition(pt1);
			const int2* point2 = GetVertexPosition(pt2);
			const int2* point3 = GetVertexPosition(pt3);

			if (point1->y > point2->y) {	Math::Swap(TopPoint, BottomPoint); Math::Swap(point1, point2);	}
			if (point1->y > point3->y) {	Math::Swap(TopPoint, OtherPoint); Math::Swap(point1, point3);	}
			if (point2->y < point3->y) {	Math::Swap(BottomPoint, OtherPoint); Math::Swap(point2, point3);	}

			DrawHalfTriangle(OtherPoint, TopPoint, BottomPoint);
			DrawHalfTriangle(OtherPoint, BottomPoint, TopPoint);

		}
		template<typename T>
		const T* GetElement(void* Base, int Index, int nStride) { return (T*)((unsigned char*)(Base) + Index * nStride); }
		inline int EvaStride(EVexterElementType InType, int ElementCount)
		{
			int BaseSize = 0;
			switch (InType)
			{
			case Raster::VET_BYTE:
				BaseSize = 1;
				break;
			case Raster::VET_SHORT:
				BaseSize = 2;
				break;
			case Raster::VET_FIXED:
				BaseSize = 4;
				break;
			case Raster::VET_FLOAT:
				BaseSize = 4;
				break;
			case Raster::VET_INT:
				BaseSize = 4;
				break;
			default:
				break;
			}
			return BaseSize * ElementCount;
		}
		inline void DrawHalfTriangle(int OtherPoint,int TopPoint,int BottomPoint)
		{
			const int2* pOther = GetVertexPosition(OtherPoint);
			const int2* pTopPoint = GetVertexPosition(TopPoint);
			const int2* pBottom = GetVertexPosition(BottomPoint);

			const float4* OtherColor = GetVertexColor(OtherPoint);
			const float4* TopColor = GetVertexColor(TopPoint);
			const float4* BottomColor = GetVertexColor(BottomPoint);

			const float2* OtherTexcoord = GetTexcoord(OtherPoint);
			const float2* TopTexcoord = GetTexcoord(TopPoint);
			const float2* BottomTexcoord = GetTexcoord(BottomPoint);

			int xOffset1 = pOther->x - pTopPoint->x;
			int yOffset1 = pOther->y - pTopPoint->y;
			float currentX1 = (float)pTopPoint->x;
			float xStep1 = yOffset1 != 0 ? 1.0f*xOffset1 / Math::Abs(yOffset1) : 0;

			int xOffset2 = pBottom->x - pTopPoint->x;
			int yOffset2 = pBottom->y - pTopPoint->y;
			float currentX2 = pTopPoint->x;
			float xStep2 = 1.0f*xOffset2 / Math::Abs(yOffset2);

			int2 LinePoint1;
			int2 LinePoint2;
			float4 LinePointColor1(1.0f,1.0f,1.0f,1.0f);
			float4 LinePointColor2(1.0f,1.0f,1.0f,1.0f);
			float4 currentColorT2O(1.0f,1.0f,1.0f,1.0f);
			float4 currentColorT2B(1.0f,1.0f,1.0f,1.0f);
			float2 currentUVT2O;
			float2 currentUVT2B;
			DrawLineParams drawLineParam = { &LinePoint1,&LinePoint2,&currentColorT2O,&currentColorT2B,TopColor != 0,&currentUVT2O,&currentUVT2B };

			
			float4 byteColorStepT2O;
			if (TopColor) {
				byteColorStepT2O = (*OtherColor - *TopColor) * (1.0f / Math::Abs(pTopPoint->y - pOther->y));
				currentColorT2O = *TopColor;
			}

			float4 byteColorStepT2B;
			if (TopColor) {
				byteColorStepT2B = (*BottomColor - *TopColor) * (1.0f / Math::Abs(pTopPoint->y - pBottom->y));
				currentColorT2B = *TopColor;
			}

			float2 UVT2O;
			if (TopTexcoord) {
				UVT2O = (*OtherTexcoord - *TopTexcoord) * (1.0f / Math::Abs(pTopPoint->y - pOther->y));
				currentUVT2O = *TopTexcoord;
			}
			float2 UVT2B;
			if (TopTexcoord) {
				UVT2B = (*BottomTexcoord - *TopTexcoord) * (1.0f / Math::Abs(pTopPoint->y - pOther->y));
				currentUVT2B = *TopTexcoord;
			}


			int ydir = pTopPoint->y < pBottom->y ? 1 : -1;
			int yStart = Math::Clamp(pTopPoint->y, 0, m_Height - 1);
			int yEnd = Math::Clamp(pOther->y, 0, m_Height - 1) + ydir;
			if (yStart != pTopPoint->y)
			{
				float T2BOffsetScale = (1.0f * Math::Abs(yStart - pTopPoint->y) / Math::Abs(pTopPoint->y - pBottom->y));
				float T2OOffsetScale = (1.0f * Math::Abs(yStart - pTopPoint->y) / Math::Abs(pTopPoint->y - pOther->y));
				currentColorT2B += byteColorStepT2B * T2BOffsetScale;
				currentColorT2O += byteColorStepT2O * T2OOffsetScale;
				currentUVT2B += UVT2B * T2BOffsetScale;
				currentUVT2O += UVT2O * T2OOffsetScale;
				currentX1 += xStep1 * (yStart - pTopPoint->y);
				currentX2 += xStep2 * (yStart - pTopPoint->y);
			}
			for (int y = yStart ; y != yEnd; y += ydir)
			{
				int x1 = Math::Round(currentX1);
				int x2 = Math::Round(currentX2);
				LinePoint1.x = x1;
				LinePoint2.x = x2;
				LinePoint1.y = LinePoint2.y = y;


				DrawLineImpl(drawLineParam);

				currentX1 += xStep1;
				currentX2 += xStep2;
				currentColorT2O += byteColorStepT2O;
				currentColorT2B += byteColorStepT2B;
				currentUVT2O += UVT2O;
				currentUVT2B += UVT2B;
			}
		}
		inline void _SetPixel(unsigned int x, unsigned int y, RGBA InColor)
		{
			if (x >= m_Width || y >= m_Height) return;

			m_Buffer[y*m_Width + x] = InColor;
		}
		inline void _SetPixelEx(int x, int y, RGBA InColor)
		{
			m_Buffer[y*m_Width + x] = InColor;
		}
		void DrawPoint(const int2* pt)
		{
			_SetPixel(pt->x, pt->y, m_Color);
		}
		void DrawLine(int pt1,int pt2)
		{
			const int2* Pt1 = GetElement<int2>(m_VertexPointer, pt1, m_VertexStride);
			const int2* Pt2 = GetElement<int2>(m_VertexPointer, pt2, m_VertexStride);

			const float4* Color1 = GetElement<float4>(m_ColorPointer, pt1, m_ColorStride);
			const float4* Color2 = GetElement<float4>(m_ColorPointer, pt2, m_ColorStride);

			DrawLineImpl({ Pt1, Pt2, Color1, Color2 });
		}

		inline void DrawLineImpl(const DrawLineParams& InParams)
		{
			auto Pt1 = InParams.pt1;
			auto Pt2 = InParams.pt2;
			auto Color1 = InParams.color1;
			auto Color2 = InParams.color2;
			auto uv1 = InParams.texcoord1;
			auto uv2 = InParams.texcoord2;

			if (Pt1->x == Pt2->x && Pt1->y == Pt2->y) { _SetPixel(Pt1->x, Pt1->y, RGBA(*Color1)); return; }

			int xOffset = Pt2->x - Pt1->x;
			int yOffset = Pt2->y - Pt1->y;
			if (Math::Abs(xOffset) > Math::Abs(yOffset))
				//x轴长一点 使用X轴为基轴进行栅格化
			{
				float2 currentUV;
				float2 uvStep;
				if(uv1&&mTexture.IsValid())
				{
					currentUV= *uv1;
					uvStep = (*uv2 - *uv1)*(1.0f / Math::Abs(xOffset));
				}

				float yStep = xOffset != 0 ? 1.0f*yOffset / Math::Abs(xOffset) : 0;
				int xStep = Pt2->x > Pt1->x ? 1 : -1;
				float currentY = Pt1->y;

				int xStart = Math::Clamp(Pt1->x, 0, m_Width - 1);
				int xEnd = Math::Clamp(Pt2->x, 0, m_Width - 1) + xStep;
				if (xStart != Pt1->x) {
					currentY += yStep * (xStart - Pt1->x);
					currentUV += uvStep * float(xStart - Pt1->x);
				}

				float4 currentColor;
				float4 byteColorStep;
				if (Color1) 
				{
					float alphaStep = 1.0f / Math::Abs(Pt2->x - Pt1->x);
					byteColorStep = EvaByteColorStep(Color1, Color2, alphaStep, currentColor);
					//currentColor += (*Color2 - *Color1)*(1.0f * Math::Abs(xStart - Pt1->x) / Math::Abs(Pt2->x - Pt1->x));
					currentColor += byteColorStep* (Math::Abs(xStart - Pt1->x)*1.0f ) ;
				}


				for (int x = xStart; x != xEnd; x += xStep)
				{
					int y = Math::Round(currentY);

					RGBA color = m_Color;

					if (InParams.hasColor)
					{
						color = ByteColor2RGBA(currentColor);
					}
					RGBA texColor;
					if (mTexture.IsValid())
					{
						texColor = mTexture.PixelFromUV(currentUV);
						color = texColor*color ;
					}
					_SetPixelEx(x, y, color);

					currentY += yStep;
					currentColor += byteColorStep;
					currentUV += uvStep;
				}
			}
			else
				//使用Y轴为基轴进行栅格化
			{
				float2 currentUV ;
				float2 uvStep ;
				if (uv1)
				{
					currentUV = *uv1;
					uvStep = (*uv2 - *uv1)*(1.0f / Math::Abs(yOffset));
				}

				float xStep = yOffset != 0 ? 1.0f*xOffset / Math::Abs(yOffset) : 0;
				int yStep = Pt2->y > Pt1->y ? 1 : -1;
				float currentX = Pt1->x;

				int yStart = Math::Clamp(Pt1->y, 0, m_Height - 1);
				int yEnd = Math::Clamp(Pt2->y, 0, m_Height - 1) + yStep;
				if (yStep != Pt1->y) {
					currentX += xStep * (yStart - Pt1->y);
					currentUV += uvStep * float(yStart - Pt1->y);
				}

				float4 currentColor;
				float4 byteColorStep;
				if (Color1) 
				{
					byteColorStep = EvaByteColorStep(Color1, Color2, 1.0f / Math::Abs(Pt2->y - Pt1->y), currentColor);
					currentColor += byteColorStep* (Math::Abs(yStart - Pt1->y)*1.0f ) ;
				}

				for (int y = yStart; y != yEnd; y += yStep)
				{
					int x = Math::Round(currentX);

					RGBA color = m_Color;

					if (InParams.hasColor)
					{
						color = ByteColor2RGBA(currentColor);
					}
					RGBA texColor;
					if (mTexture.IsValid())
					{
						texColor = mTexture.PixelFromUV(currentUV);
						color = texColor ;
					}
					_SetPixelEx(x, y, color);

					currentX += xStep;
					currentColor += byteColorStep;
					currentUV += uvStep;

				}
			}
		}

		inline RGBA ByteColor2RGBA(const float4& InByteColor)
		{
			return RGBA(Math::Round(InByteColor.x),Math::Round(InByteColor.y),Math::Round(InByteColor.z),Math::Round(InByteColor.w));
		}
		inline float4 EvaByteColorStep(const float4* Color1, const float4* Color2, float AlphaStep,float4& OutCurrentByteColor)
		{
			float4 byteColor1 = float4(Color1->x*255.0f, Color1->y*255.0f, Color1->z*255.0f, Color1->w*255.0f);
			float4 byteColor2 = float4(Color2->x*255.0f, Color2->y*255.0f, Color2->z*255.0f, Color2->w*255.0f);
			OutCurrentByteColor = byteColor1;
			return (byteColor2 - byteColor1)*AlphaStep;
		}

	private:
		int m_Width;
		int m_Height;
		RGBA* m_Buffer;
		RGBA m_Color;
		Image mTexture;

		int m_VertexStride = 0;
		void* m_VertexPointer = nullptr;

		int m_ColorStride = 0;
		void* m_ColorPointer = nullptr;

		int m_TexcoordStride = 0;
		void* m_TexcoordPointer = nullptr;

	};
}
