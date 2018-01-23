#pragma once
#include <math.h>

namespace Raster
{
    #define M_PI       3.14159265358979323846f   // pi
	class Math
	{
	public:
		static inline float Rand()
		{
			return 1.0f* rand() / RAND_MAX;
		}
		template<typename T>
		static inline int Round(T InValue)
		{
			return (int)(InValue + 0.5);
		}

		template<typename T>
		static T RandInRange(T Min, T Max)
		{
			return Lerp(Min, Max, Rand());
		}

		template<typename T>
		static T Lerp(T Start, T End, float Alpha)
		{
			return Start + (T)(Alpha * (End - Start));
		}

		template<typename T>
		static inline T Abs(T Value)
		{
			return Value >= 0 ? Value : -Value;
		}

		template<typename T>
		static inline void Swap(T& value1, T& value2)
		{
			T tmp = value2;
			value2 = value1;
			value1 = tmp;
		}
		static inline float Degree2Radian(float InDegree)
		{
			//return InDegree * 0.0174532925f;
			return (float)InDegree * M_PI / 180.0f;
		}
		static inline float Radian2Degree(float InRadian)
		{
			//return InRadian * 57.2957795f ;
			return (float)InRadian * 180.f / M_PI;
		}

		static inline float Sin(float InAngle)
		{
			return (float)::sin(InAngle);
		}
		static inline float Cos(float InAngle)
		{
			return (float)::cos(InAngle);
		}

		static inline float Pow(float InBase, int InE)
		{
			return (float)::pow(InBase, InE);
		}
	};
}
