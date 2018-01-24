#pragma once
#include "Vector.hpp"

namespace Raster
{
	class RGBA
	{
	public:
		RGBA() {
			R = G = B = A = 0;
		}
		RGBA(unsigned char InR, unsigned char InG, unsigned char InB, unsigned char InA)
		{
			R = InR;
			G = InG;
			B = InB;
			A = InA;
		}
		RGBA(unsigned int InColor)
		{
			Color = InColor;
		}
		RGBA(const RGBA& Other)
		{
			R = Other.R;
			G = Other.G;
			B = Other.B;
			A = Other.A;
		}
		RGBA(const Vector4dFloat& InColorf)
		{
			R = (float)InColorf.x*255;
			G = (float)InColorf.y*255;
			B = (float)InColorf.z*255;
			A = (float)InColorf.w*255;
		}
		bool operator==(const RGBA& InOtherColor) { return Color == InOtherColor.Color; }
		bool operator!=(const RGBA& InOtherColor) { return Color != InOtherColor.Color; }

	public:
		union 
		{
			struct 
			{
				unsigned char B;
				unsigned char G;
				unsigned char R;
				unsigned char A;
			};
			unsigned int Color;
		};
	};
}
