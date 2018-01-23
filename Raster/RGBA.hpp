#pragma once
#include "Vector.hpp"

namespace Raster
{
	class RGBA
	{
	public:
		RGBA(unsigned char InR, unsigned char InG, unsigned char InB, unsigned char InA)
		{
			R = InR;
			G = InG;
			B = InB;
			A = InA;
		}
		RGBA(const Vector4dFloat& InColorf)
		{
			R = (float)InColorf.x*255;
			G = (float)InColorf.y*255;
			B = (float)InColorf.z*255;
			A = (float)InColorf.w*255;
		}

	public:
		unsigned char B;
		unsigned char G;
		unsigned char R;
		unsigned char A;
	};
}
