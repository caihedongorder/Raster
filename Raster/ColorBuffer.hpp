#pragma once
#include "RGBA.hpp"
#include "Math.hpp"
#include "Vector.hpp"
#include "Profile.hpp"

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
			const Vector2dInt* pt1;
			const Vector2dInt* pt2;
			const Vector4dFloat *color1;
			const Vector4dFloat *color2;
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
		}
		void Clear(RGBA InColor = RGBA(0,0,0,0))
		{
			AUTO_PROFILE(_T("ColorBuffer.Clear"));
			for (int i = 0;i < m_Width*m_Height;++i)
			{
				m_Buffer[i] = InColor;
			}
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
			Vector2dInt* Points = (Vector2dInt*)(m_VertexPointer);
			Vector4dFloat* Colors = (Vector4dFloat*)(m_ColorPointer);
			switch (InDrawMode)
			{
			case Raster::DM_POINTS:
				{
					for (int i = 0;i<PointCount;++i)
					{
						DrawPoint(GetElement<Vector2dInt>(Points,i,m_VertexStride));
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
		void DrawTriangle(int pt1,int pt2,int pt3)
		{
			const Vector2dInt* point1 = GetElement<Vector2dInt>(m_VertexPointer,pt1,m_VertexStride);
			const Vector2dInt* point2 = GetElement<Vector2dInt>(m_VertexPointer,pt2,m_VertexStride);
			const Vector2dInt* point3 = GetElement<Vector2dInt>(m_VertexPointer,pt3,m_VertexStride);

			int TopPoint	= pt1;
			int BottomPoint = pt2;
			int OtherPoint	= pt3;

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
			const Vector2dInt* pOther = GetElement<Vector2dInt>(m_VertexPointer,OtherPoint,m_VertexStride);
			const Vector2dInt* pTopPoint = GetElement<Vector2dInt>(m_VertexPointer,TopPoint,m_VertexStride);
			const Vector2dInt* pBottom = GetElement<Vector2dInt>(m_VertexPointer,BottomPoint,m_VertexStride);

			const Vector4dFloat* OtherColor = GetElement<Vector4dFloat>(m_ColorPointer, OtherPoint, m_ColorStride);
			const Vector4dFloat* TopColor = GetElement<Vector4dFloat>(m_ColorPointer, TopPoint, m_ColorStride);
			const Vector4dFloat* BottomColor = GetElement<Vector4dFloat>(m_ColorPointer, BottomPoint, m_ColorStride);

			int xOffset1 = pOther->x - pTopPoint->x;
			int yOffset1 = pOther->y - pTopPoint->y;
			float currentX1 = (float)pTopPoint->x;
			float xStep1 = 1.0f*xOffset1 / Math::Abs(yOffset1);

			int xOffset2 = pBottom->x - pTopPoint->x;
			int yOffset2 = pBottom->y - pTopPoint->y;
			float currentX2 = pTopPoint->x;
			float xStep2 = 1.0f*xOffset2 / Math::Abs(yOffset2);

			Vector2dInt LinePoint1;
			Vector2dInt LinePoint2;
			Vector4dFloat LinePointColor1(1.0f,1.0f,1.0f,1.0f);
			Vector4dFloat LinePointColor2(1.0f,1.0f,1.0f,1.0f);

			
			Vector4dFloat currentColorT2O = *TopColor;
			Vector4dFloat byteColorStepT2O;
			if (OtherColor) byteColorStepT2O = (*OtherColor - *TopColor) * (1.0f / Math::Abs(pTopPoint->y - pOther->y));

			Vector4dFloat currentColorT2B = *TopColor;
			Vector4dFloat byteColorStepT2B;
			if (OtherColor) byteColorStepT2B = (*BottomColor - *TopColor) * (1.0f / Math::Abs(pTopPoint->y - pBottom->y));

			DrawLineParams drawLineParam = { &LinePoint1,&LinePoint2,&currentColorT2O,&currentColorT2B };

			int ydir = pTopPoint->y < pBottom->y ? 1 : -1;
			int yStart = Math::Clamp(pTopPoint->y, 0, m_Height);
			int yEnd = Math::Clamp(pOther->y, 0, m_Height) + ydir;
			if (yStart != pTopPoint->y)
			{
				currentColorT2B += byteColorStepT2B * (1.0f * Math::Abs(yStart - pTopPoint->y) / Math::Abs(pTopPoint->y - pBottom->y));
				currentColorT2O += byteColorStepT2O * (1.0f * Math::Abs(yStart - pTopPoint->y) / Math::Abs(pTopPoint->y - pOther->y));
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
		void DrawPoint(const Vector2dInt* pt)
		{
			_SetPixel(pt->x, pt->y, m_Color);
		}
		void DrawLine(int pt1,int pt2)
		{
			const Vector2dInt* Pt1 = GetElement<Vector2dInt>(m_VertexPointer, pt1, m_VertexStride);
			const Vector2dInt* Pt2 = GetElement<Vector2dInt>(m_VertexPointer, pt2, m_VertexStride);

			const Vector4dFloat* Color1 = GetElement<Vector4dFloat>(m_ColorPointer, pt1, m_ColorStride);
			const Vector4dFloat* Color2 = GetElement<Vector4dFloat>(m_ColorPointer, pt2, m_ColorStride);

			DrawLineImpl({ Pt1, Pt2, Color1, Color2 });
		}

		inline void DrawLineImpl(const DrawLineParams& InParams)
		{
			auto Pt1 = InParams.pt1;
			auto Pt2 = InParams.pt2;
			auto Color1 = InParams.color1;
			auto Color2 = InParams.color2;
			if (Pt1->x == Pt2->x && Pt1->y == Pt2->y) { _SetPixel(Pt1->x, Pt1->y, RGBA(*Color1)); return; }

			int xOffset = Pt2->x - Pt1->x;
			int yOffset = Pt2->y - Pt1->y;
			if (Math::Abs(xOffset) > Math::Abs(yOffset))
				//x轴长一点 使用X轴为基轴进行栅格化
			{
				float yStep = xOffset != 0 ? 1.0f*yOffset / Math::Abs(xOffset) : 0;
				int xStep = Pt2->x > Pt1->x ? 1 : -1;
				float currentY = Pt1->y;

				int xStart = Math::Clamp(Pt1->x,0,m_Width);
				int xEnd = Math::Clamp(Pt2->x, 0, m_Width) + xStep;

				Vector4dFloat currentColor;
				Vector4dFloat byteColorStep;
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

					if (Color1)
					{
						color = ByteColor2RGBA(currentColor);
					}
					_SetPixelEx(x, y, color);

					currentY += yStep;
					currentColor += byteColorStep;
				}
			}
			else
				//使用Y轴为基轴进行栅格化
			{
				float xStep = yOffset != 0 ? 1.0f*xOffset / Math::Abs(yOffset) : 0;
				int yStep = Pt2->y > Pt1->y ? 1 : -1;
				float currentX = Pt1->x;

				int yStart = Math::Clamp(Pt1->y, 0, m_Height);
				int yEnd = Math::Clamp(Pt2->y, 0, m_Height) + yStep;

				Vector4dFloat currentColor;
				Vector4dFloat byteColorStep;
				if (Color1) 
				{
					byteColorStep = EvaByteColorStep(Color1, Color2, 1.0f / Math::Abs(Pt2->y - Pt1->y), currentColor);
					currentColor += byteColorStep* (Math::Abs(yStart - Pt1->y)*1.0f ) ;
				}

				for (int y = yStart; y != yEnd; y += yStep)
				{
					int x = Math::Round(currentX);

					RGBA color = m_Color;

					if (Color1)
					{
						color = ByteColor2RGBA(currentColor);
					}
					_SetPixelEx(x, y, color);

					currentX += xStep;
					currentColor += byteColorStep;

				}
			}
		}

		inline RGBA ByteColor2RGBA(const Vector4dFloat& InByteColor)
		{
			return RGBA(Math::Round(InByteColor.x),Math::Round(InByteColor.y),Math::Round(InByteColor.z),Math::Round(InByteColor.w));
		}
		inline Vector4dFloat EvaByteColorStep(const Vector4dFloat* Color1, const Vector4dFloat* Color2, float AlphaStep,Vector4dFloat& OutCurrentByteColor)
		{
			Vector4dFloat byteColor1 = Vector4dFloat(Color1->x*255.0f, Color1->y*255.0f, Color1->z*255.0f, Color1->w*255.0f);
			Vector4dFloat byteColor2 = Vector4dFloat(Color2->x*255.0f, Color2->y*255.0f, Color2->z*255.0f, Color2->w*255.0f);
			OutCurrentByteColor = byteColor1;
			return (byteColor2 - byteColor1)*AlphaStep;
		}

	private:
		int m_Width;
		int m_Height;
		RGBA* m_Buffer;
		RGBA m_Color;

		int m_VertexStride = 0;
		void* m_VertexPointer = nullptr;

		int m_ColorStride = 0;
		void* m_ColorPointer = nullptr;

	};
}
