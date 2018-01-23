#pragma once
#include "RGBA.hpp"
#include "Math.hpp"
#include "Vector.hpp"

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
						DrawTriangle(GetElement<Vector2dInt>(Points, i * 3, m_VertexStride),
							GetElement<Vector2dInt>(Points, i * 3 + 1, m_VertexStride),
							GetElement<Vector2dInt>(Points, i * 3 + 2, m_VertexStride));
					}
				}

				break;
			default:
				break;
			}
		}
	private:
		void DrawTriangle(const Vector2dInt* pt1, const Vector2dInt* pt2,const Vector2dInt* pt3)
		{
			const Vector2dInt* pts[]= { pt1,pt2,pt3 };

			if (pts[0]->y > pts[1]->y) Math::Swap(pts[0],pts[1]);
			if (pts[0]->y > pts[2]->y) Math::Swap(pts[0],pts[2]);
			if (pts[1]->y > pts[2]->y) Math::Swap(pts[1],pts[2]);

			const Vector2dInt* pTopPoint = pts[0];
			const Vector2dInt* pBottom = pts[2];
			const Vector2dInt* pOther = pts[1];

			DrawHalfTriangle(pOther, pTopPoint, pBottom);

			DrawHalfTriangle(pOther, pBottom, pTopPoint);
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
		inline void DrawHalfTriangle(const Vector2dInt* pOther, const Vector2dInt* pTopPoint, const Vector2dInt* pBottom)
		{
			int xOffset1 = pOther->x - pTopPoint->x;
			int yOffset1 = pOther->y - pTopPoint->y;
			float currentX1 = (float)pTopPoint->x;
			float xStep1 = 1.0f*xOffset1 / Math::Abs(yOffset1);

			int xOffset2 = pBottom->x - pTopPoint->x;
			int yOffset2 = pBottom->y - pTopPoint->y;
			float currentX2 = pTopPoint->x;
			float xStep2 = 1.0f*xOffset2 / Math::Abs(yOffset2);

			int ydir = pTopPoint->y < pBottom->y ? 1 : -1;
			for (int y = pTopPoint->y; y != pOther->y + ydir; y += ydir)
			{
				int x1 = Math::Round(currentX1);
				int x2 = Math::Round(currentX2);
				int dir = x1 > x2 ? -1.0f : 1.0f;
				for (int x = x1; x != x2 + dir; x += dir)
				{
					_SetPixel(x, y, m_Color);
				}
				currentX1 += xStep1;
				currentX2 += xStep2;
			}
		}
		void _SetPixel(unsigned int x, unsigned int y, RGBA InColor)
		{
			if (x >= m_Width || y >= m_Height) return;

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

			int xOffset = Pt2->x - Pt1->x;
			int yOffset = Pt2->y - Pt1->y;
			if (Math::Abs(xOffset) > Math::Abs(yOffset))
			//x轴长一点 使用X轴为基轴进行栅格化
			{
				float yStep = xOffset != 0 ? 1.0f*yOffset / Math::Abs(xOffset) : 0;
				int xStep = Pt2->x > Pt1->x ? 1 : -1;
				float currentY = Pt1->y;
				float currentAlpha = 0;
				float AlphaStep = 1.0f / Math::Abs(Pt2->x - Pt1->x);
				Vector4dFloat byteColor1 = Vector4dFloat(Color1->x*255.0f, Color1->y*255.0f, Color1->z*255.0f, Color1->w*255.0f);
				Vector4dFloat byteColor2 = Vector4dFloat(Color2->x*255.0f, Color2->y*255.0f, Color2->z*255.0f, Color2->w*255.0f);
				Vector4dFloat byteColorStep = (byteColor2 - byteColor1)*AlphaStep;
				Vector4dFloat currentColor = byteColor1;

				for (int x = Pt1->x; x != Pt2->x + xStep; x += xStep)
				{
					int y = Math::Round(currentY);

					RGBA color = m_Color;
	
					if (Color1)
					{
						color = RGBA(Math::Round(currentColor.x),Math::Round(currentColor.y),Math::Round(currentColor.z),currentColor.w);
						//currentColor = Math::Lerp(*Color1, *Color2, currentAlpha);
						//color = RGBA(currentColor);
					}
					_SetPixel(x, y, color);

					currentY += yStep;
					currentAlpha += AlphaStep;
					currentColor += byteColorStep;
				}
			}
			else
			//使用Y轴为基轴进行栅格化
			{
				float xStep = yOffset != 0 ? 1.0f*xOffset / Math::Abs(yOffset) : 0;
				int yStep = Pt2->y > Pt1->y ? 1 : -1;
				float currentX = Pt1->x;

				float currentAlpha = 0;
				float AlphaStep = 1.0f / Math::Abs(Pt2->y - Pt1->y);
				Vector4dFloat byteColor1 = Vector4dFloat(Color1->x*255.0f, Color1->y*255.0f, Color1->z*255.0f, Color1->w*255.0f);
				Vector4dFloat byteColor2 = Vector4dFloat(Color2->x*255.0f, Color2->y*255.0f, Color2->z*255.0f, Color2->w*255.0f);
				Vector4dFloat byteColorStep = (byteColor2 - byteColor1)*AlphaStep;
				Vector4dFloat currentColor = byteColor1;

				for (int y = Pt1->y; y != Pt2->y + yStep; y += yStep)
				{
					int x = Math::Round(currentX);

					RGBA color = m_Color;
	
					if (Color1)
					{
						color = RGBA(currentColor.x,currentColor.y,currentColor.z,currentColor.w);
						//currentColor = Math::Lerp(*Color1, *Color2, currentAlpha);
						//color = RGBA(currentColor);
					}
					_SetPixel(x, y,color);

					currentX += xStep;
					currentAlpha += AlphaStep;
					currentColor += byteColorStep;
					
				}
			}
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
