#pragma once

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
	};
	typedef Vector2d<int> Vector2dInt;
	typedef Vector2d<float> Vector2dFloat;
	
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
	};

	typedef Vector3d<int> Vector3dInt;
	typedef Vector3d<float> Vector3dFloat;

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
	};
	typedef Vector4d<int> Vector4dInt;
	typedef Vector4d<float> Vector4dFloat;
}
