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
		RGBA(const float4& InColorf)
		{
			R = (float)InColorf.x*255;
			G = (float)InColorf.y*255;
			B = (float)InColorf.z*255;
			A = (float)InColorf.w*255;
		}
		float4 tofloat4() const {
			return float4(R / 255.0f, G / 255.0f,B / 255.0f, A / 255.0f);
		}
		bool operator==(const RGBA& InOtherColor) { return Color == InOtherColor.Color; }
		bool operator!=(const RGBA& InOtherColor) { return Color != InOtherColor.Color; }

		inline RGBA Multipy(const RGBA& InOther) const
		{
			float4 clr1 = tofloat4();
			float4 clr2 = InOther.tofloat4();
			return RGBA(clr1*clr2);
		}
		RGBA operator* (const RGBA& InOther) const
		{
			return Multipy(InOther);
		}

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
