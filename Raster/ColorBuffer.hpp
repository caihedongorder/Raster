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
		void DrawArrays(EDrawMode InDrawMode, Vector2dInt* Points, int PointCount)
		{
			switch (InDrawMode)
			{
			case Raster::DM_POINTS:
				{
					for (int i = 0;i<PointCount;++i)
					{
						DrawPoint(Points[i]);
					}
				}
				break;
			case Raster::DM_LINES:
				{
					PointCount = PointCount / 2 * 2;
					for (int i = 0 ; i < PointCount ; i += 2 )
					{
						DrawLine(Points[i], Points[i + 1]);
					}
				}
				break;
			case Raster::DM_LINE_STRIP:
				{
					if (PointCount < 2) return;
					for (int i = 0 ; i < PointCount - 1 ; ++i )
					{
						DrawLine(Points[i] , Points [i + 1]);
					}
				}
				break;
			case Raster::DM_LINE_LOOP:
				{
					if (PointCount <= 2) return;
					for (int i = 0 ; i < PointCount ; ++i )
					{
						if (i == PointCount - 1)DrawLine(Points[0], Points[PointCount - 1]);
						else DrawLine(Points[i] , Points [i + 1]);
					}
				}
				break;
			default:
				break;
			}
		}
		void DrawTriangle(const Vector2dInt& pt1, const Vector2dInt& pt2,const Vector2dInt& pt3)
		{
			const Vector2dInt* pts[]= { &pt1,&pt2,&pt3 };

			if (pts[0]->y > pts[1]->y) Math::Swap(pts[0],pts[1]);
			if (pts[0]->y > pts[2]->y) Math::Swap(pts[0],pts[2]);
			if (pts[1]->y > pts[2]->y) Math::Swap(pts[1],pts[2]);

			const Vector2dInt* pTopPoint = pts[0];
			const Vector2dInt* pBottom = pts[2];
			const Vector2dInt* pOther = pts[1];

			int xOffset1 = pOther->x - pTopPoint->x;
			int yOffset1 = pOther->y - pTopPoint->y;
			float currentX1 = pTopPoint->x;
			float xStep1 = 1.0f*xOffset1 / Math::Abs(yOffset1);

			int xOffset2 = pBottom->x - pTopPoint->x;
			int yOffset2 = pBottom->y - pTopPoint->y;
			float currentX2 = pTopPoint->x;
			float xStep2 = 1.0f*xOffset2 / Math::Abs(yOffset2);

			for (int y = pTopPoint->y; y <= pOther->y; ++y)
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

			xOffset1 = pOther->x - pBottom->x;
			yOffset1 = pOther->y - pBottom->y;
			currentX1 = pBottom->x;
			xStep1 = 1.0f * xOffset1 / Math::Abs(yOffset1);


			xOffset2 = pTopPoint->x - pBottom->x;
			yOffset2 = pTopPoint->y - pBottom->y;
			currentX2 = pBottom->x;
			xStep2 = 1.0f*xOffset2 / Math::Abs(yOffset2);

			for (int y = pBottom->y; y > pOther->y; --y)
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
	private:
		void _SetPixel(unsigned int x, unsigned int y, RGBA InColor)
		{
			if (x >= m_Width || y >= m_Height) return;

			m_Buffer[y*m_Width + x] = InColor;
		}
		void DrawPoint(const Vector2dInt& pt)
		{
			_SetPixel(pt.x, pt.y, m_Color);
		}
		void DrawLine(const Vector2dInt& Pt1,const Vector2dInt& Pt2)
		{
			int xOffset = Pt2.x - Pt1.x;
			int yOffset = Pt2.y - Pt1.y;
			if (Math::Abs(xOffset) > Math::Abs(yOffset))
			//x轴长一点 使用X轴为基轴进行栅格化
			{
				float yStep = xOffset != 0 ? 1.0f*yOffset / Math::Abs(xOffset) : 0;
				int xStep = Pt2.x > Pt1.x ? 1 : -1;
				float currentY = Pt1.y;
				for (int x = Pt1.x; x != Pt2.x + xStep; x += xStep)
				{
					int y = Math::Round(currentY);
					_SetPixel(x, y,m_Color);

					currentY += yStep;
					
				}
			}
			else
			//使用Y轴为基轴进行栅格化
			{
				float xStep = yOffset != 0 ? 1.0f*xOffset / Math::Abs(yOffset) : 0;
				int yStep = Pt2.y > Pt1.y ? 1 : -1;
				float currentX = Pt1.x;
				for (int y = Pt1.y; y != Pt2.y + yStep; y += yStep)
				{
					int x = Math::Round(currentX);
					_SetPixel(x, y,m_Color);

					currentX += xStep;
					
				}
			}
		}
	private:
		int m_Width;
		int m_Height;
		RGBA* m_Buffer;
		RGBA m_Color;
	};
}
