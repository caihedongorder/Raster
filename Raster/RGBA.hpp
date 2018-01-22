#pragma once

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

	public:
		unsigned char B;
		unsigned char G;
		unsigned char R;
		unsigned char A;
	};
}
