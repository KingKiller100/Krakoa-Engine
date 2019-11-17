#pragma once

#include "VectorBase.h"

namespace util::kMaths
{
	template <typename T>
	struct Vector3 : VectorBase<T>
	{
	public:
		Vector3()
		: VectorBase<T>()
		{}

		explicit Vector3(const T v)
		: VectorBase<T>(v, v, v, 0)
		{}
		
		explicit Vector3(const T x, const T y, const T z)
		: VectorBase<T>(x, y, z, 0)
		{}

		template<typename T2>
		explicit  Vector3(const Vector3& other)
			: VectorBase<T>(other.X(), other.Y(), other.Z(), 0)
		{}
		
		template<typename T2>
		explicit  Vector3(const Vector3<T2>& other)
			: VectorBase<T>(other.X(), other.Y(), other.Z(), 0)
		{}


		Vector3& operator=(const Vector3& other)
		{
			this->x = other.x;
			this->y = other.y;
			this->z = other.z;

			return *this;
		}

		template<typename  T2>
		Vector3& operator=(const Vector3<T2>& other)
		{
			this->x = other.X();
			this->y = other.Y();
			this->z = other.Z();

			return *this;
		}

		~Vector3()
		= default;

		T& X()
		{
			return this->x;
		}

		[[nodiscard]] T X() const
		{
			return this->x;
		}

		T& Y()
		{
			return this->y;
		}

		[[nodiscard]] T Y() const
		{
			return this->y;
		}

		T& Z() 
		{
			return this->z;
		}
		
		[[nodiscard]] T Z() const
		{
			return this->z;
		}
		
		Vector3 CrossProduct(const Vector3& v) const
		{
			return Vector3(this->y * v.z - this->z * v.y,
				this->z * v.x - this->x * v.z,
				this->x * v.y - this->y * v.x);
		}
	};

	template<typename T1, typename T2>
	static Vector3<T1> CrossProduct(const Vector3<T1>& u, const Vector3<T2>& v)
	{
		return Vector3<T1>(u.CrossProduct( Vector3<T1>(v.X(), v.Y(), v.Z()) ));
	}

	using Vector3s = Vector3 < int		>; // signed integer
	using Vector3f = Vector3 < float	>; // floating point
	using Vector3d = Vector3 < double	>; // double floating point
	using Vector3u = Vector3 < unsigned >; // unsigned integer
		
}

