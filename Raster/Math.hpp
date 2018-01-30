#pragma once
#include <math.h>
#include "Vector.hpp"
#include "RGBA.hpp"

namespace Raster
{
    //#define M_PI       3.14159265358979323846f   // pi
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
		static Vector2d<T> Lerp(const Vector2d<T>& Start, const Vector2d<T>& End, float Alpha)
		{
			Vector2d<T> ReturnValue;
			ReturnValue.x = Lerp<T>(Start.x, End.x, Alpha);
			ReturnValue.y = Lerp<T>(Start.y, End.y, Alpha);
			return ReturnValue;
		}

		template<typename T>
		static Vector3d<T> Lerp(const Vector3d<T>& Start, const Vector3d<T>& End, float Alpha)
		{
			Vector3d<T> ReturnValue;
			ReturnValue.x = Lerp<T>(Start.x, End.x, Alpha);
			ReturnValue.y = Lerp<T>(Start.y, End.y, Alpha);
			ReturnValue.z = Lerp<T>(Start.z, End.z, Alpha);
			return ReturnValue;
		}

		template<typename T>
		static Vector4d<T> Lerp(const Vector4d<T>& Start, const Vector4d<T>& End, float Alpha)
		{
			Vector4d<T> ReturnValue;
			ReturnValue.x = Lerp<T>(Start.x, End.x, Alpha);
			ReturnValue.y = Lerp<T>(Start.y, End.y, Alpha);
			ReturnValue.z = Lerp<T>(Start.z, End.z, Alpha);
			ReturnValue.w = Lerp<T>(Start.w, End.w, Alpha);
			return ReturnValue;
		}

		static RGBA Lerp(const RGBA& Start, const RGBA& End, float Alpha)
		{
			RGBA ReturnValue;
			ReturnValue.R = Lerp<unsigned char>(Start.R, End.R, Alpha);
			ReturnValue.B = Lerp<unsigned char>(Start.B, End.B, Alpha);
			ReturnValue.G = Lerp<unsigned char>(Start.G, End.G, Alpha);
			ReturnValue.A = Lerp<unsigned char>(Start.A, End.A, Alpha);
			return ReturnValue;
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
		template<typename T>
		static inline T Min(T value1, T value2)
		{
			return value1 < value2 ? value1 : value2;
		}
		template<typename T>
		static inline T Max(T value1, T value2)
		{
			return value1 > value2 ? value1 : value2;
		}
		template<typename T>
		static inline T Clamp(T CurrentValue,T min, T max)
		{
			return CurrentValue < min ? min : CurrentValue>max ? max : CurrentValue;
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
