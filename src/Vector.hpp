#pragma once
#include <mathfu/glsl_mappings.h>

namespace Raster
{
	template<typename T>
	class Vector2d
	{
	public:
		T x, y;

	public:
		Vector2d(T InX, T InY)
		{
			x = InX;
			y = InY;
		}
		Vector2d()
		{
			x = y = T();
		}
		Vector2d& operator+=(const Vector2d& Other)
		{
			this->x += Other.x;
			this->y += Other.y;
			return *this;
		}

		Vector2d& operator-=(const Vector2d& Other)
		{
			this->x -= Other.x;
			this->y -= Other.y;
			return *this;
		}

		Vector2d operator+(const Vector2d& Other) const
		{
			return Vector2d<T>(this->x + Other.x,
				this->y + Other.y);
		}
		Vector2d operator-(const Vector2d& Other) const
		{
			return Vector2d<T>(this->x - Other.x,
				this->y - Other.y);
		}

		Vector2d operator*(float InScale) const
		{
			return Vector2d<T>(this->x * InScale,
				this->y * InScale	);
		}

		Vector2d& operator*=(float InScale)
		{
			this->x *= InScale;
			this->y *= InScale;
			return *this;
		}
	};
	//typedef Vector2d<int> int2;
	//typedef Vector2d<float> float2;

	typedef mathfu::Vector<int, 2> int2;
	typedef mathfu::Vector<float, 2> float2;
	
	template<typename T>
	class Vector3d
	{
	public:
		T x, y, z;

	public:
		Vector3d(T InX, T InY,T InZ)
		{
			x = InX;
			y = InY;
			z = InZ;
		}
		Vector3d()
		{
			x = y = z = T();
		}
		Vector3d& operator+=(const Vector3d& Other)
		{
			this->x += Other.x;
			this->y += Other.y;
			this->z += Other.z;
			return *this;
		}

		Vector3d& operator-=(const Vector3d& Other)
		{
			this->x -= Other.x;
			this->y -= Other.y;
			this->z -= Other.z;
			return *this;
		}

		Vector3d operator+(const Vector3d& Other) const
		{
			return Vector3d<T>(this->x + Other.x,
				this->y + Other.y,
				this->z + Other.z);
		}
		Vector3d operator-(const Vector3d& Other) const
		{
			return Vector3d<T>(this->x - Other.x,
				this->y - Other.y,
				this->z - Other.z);
		}

		Vector3d operator*(float InScale) const
		{
			return Vector3d<T>(this->x * InScale,
				this->y * InScale,
				this->z * InScale);
		}

		Vector3d& operator*=(float InScale)
		{
			this->x *= InScale;
			this->y *= InScale;
			this->z *= InScale;
			return *this;
		}
		
	};

	//typedef Vector3d<int> int3;
	//typedef Vector3d<float> float3;


	typedef mathfu::Vector<int, 3> int3;
	typedef mathfu::Vector<float, 3> float3;

	template<typename T>
	class Vector4d
	{
	public:
		T x, y, z, w;

	public:
		Vector4d(T InX, T InY,T InZ, T InW)
		{
			x = InX;
			y = InY;
			z = InZ;
			w = InW;
		}
		Vector4d()
		{
			x = y = z = w = T();
		}
		Vector4d& operator+=(const Vector4d& Other)
		{
			this->x += Other.x;
			this->y += Other.y;
			this->z += Other.z;
			this->w += Other.w;
			return *this;
		}

		Vector4d& operator-=(const Vector4d& Other)
		{
			this->x -= Other.x;
			this->y -= Other.y;
			this->z -= Other.z;
			this->w -= Other.w;
			return *this;
		}

		Vector4d operator+(const Vector4d& Other) const
		{
			return Vector4d<T>(this->x + Other.x,
				this->y + Other.y,
				this->z + Other.z,
				this->w + Other.w);
		}
		Vector4d operator-(const Vector4d& Other) const
		{
			return Vector4d<T>(this->x - Other.x,
				this->y - Other.y,
				this->z - Other.z,
				this->w - Other.w);
		}

		Vector4d operator*(float InScale) const
		{
			return Vector4d<T>(this->x * InScale,
				this->y * InScale,
				this->z * InScale,
				this->w * InScale);
		}
		Vector4d operator*(const Vector4d& Other) const
		{
			return Vector4d<T>(this->x * Other.x,
				this->y * Other.y,
				this->z * Other.z,
				this->w * Other.w);
		}

		Vector4d& operator*=(float InScale)
		{
			this->x *= InScale;
			this->y *= InScale;
			this->z *= InScale;
			this->w *= InScale;
			return *this;
		}
	};
	//typedef Vector4d<int> int4;
	//typedef Vector4d<float> float4;

	typedef mathfu::Vector<int, 4> int4;
	typedef mathfu::Vector<float, 4> float4;
}
