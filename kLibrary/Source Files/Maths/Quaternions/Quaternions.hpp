#pragma once
/**
* Holds a three degree of freedom orientation.
*
* Quaternions have
* several mathematical properties that make them useful for
* representing orientations, but require four items of data to
* hold the three degrees of freedom. These four items of data can
* be viewed as the coefficients of a complex number with three
* imaginary parts. The mathematics of the quaternion is then
* defined and is roughly correspondent to the math of 3D
* rotations. A quaternion is only a valid rotation if it is
* normalized: i.e. it has a length of 1.
*
* \note Angular velocity and acceleration can be correctly
* represented as vectors. Quaternions are only needed for
* orientation.
*/

#include "../../HelperMacros.hpp"

#include "../Constants.hpp"
#include "../Vectors/Vector3.hpp"
#include "../Matrices/TransformMatrix.hpp"

#include <limits>

namespace kmaths
{
	template<typename T>
	struct Quaternion
	{
	public:
		static_assert(std::is_floating_point_v<T>, "Must be instantiated with a floating point type");

		using Type = T;
		inline static constexpr Length_Type Length = 4;
		inline static constexpr size_t Bytes = Length * sizeof(T);

		/**
		* \brief
		*		The explicit constructor creates a quaternion with the given components.
		*
		* \param[in] r
		*		The real component of the rigid body's orientation quaternion.
		*
		* \param i
		*		The first complex component of the rigid body's orientation quaternion.
		*
		* \param[in] j
		*		The second complex component of the rigid body's orientation quaternion.
		*
		* \param[in] k
		*		The third complex component of the rigid body's orientation quaternion.
		*
		* \note
		*		The given orientation does not need to be normalized,
		*		and can be zero. This function will not alter the given
		*		values, or normalize the quaternion. To normalize the
		*		quaternion (and make a zero quaternion a legal rotation),
		*		use the normalize function.
		*
		* \see
				Normalize
		*/
		explicit constexpr Quaternion(const T r = CAST(T, 1), const T i = CAST(T, 0), const T j = CAST(T, 0), const T k = CAST(T, 0)) noexcept
			: r(ToRadians<T>(r)), i(i), j(j), k(k)
		{
			if (MagnitudeSQ() != constants::One<T>())
				Normalize();
		}

		explicit constexpr Quaternion(const Vector<T, 3>& v, T degrees) noexcept
			: r(ToRadians<T>(degrees)), i(v[0]), j(v[1]), k(v[2])
		{
			if (MagnitudeSQ() != constants::One<T>())
				Normalize();
		}


		USE_RESULT constexpr T MagnitudeSQ() const noexcept
		{
			return (r * r) + (i * i) + (j * j) + (k * k);
		}

		/**
		* normalizes the quaternion to unit length, making it a valid
		* orientation quaternion.
		*/
		constexpr void Normalize() noexcept
		{
			const T magSQ = MagnitudeSQ();

			// Check for zero length quaternion, and use the no-rotation
			// quaternion in that case.
			if (magSQ <= std::numeric_limits<T>::epsilon())
			{
				r = constants::One<T>();
				return;
			}

			const T d = constants::OneOver<T>(Sqrt<T>(magSQ));

			r *= d;
			i *= d;
			j *= d;
			k *= d;
		}

		USE_RESULT constexpr T DotProduct(const Quaternion& other) const noexcept
		{
			const auto dotAxes = (i * other.i) + (j * other.j) + (k * other.k);
			const auto dotRotation = r * other.r;
			return (dotAxes + dotRotation);
		}

		/**
		* \brief
		*		Adds the given vector to this, scaled by the given amount.
		*		This is used to update the orientation quaternion by a rotation
		*		and time.
		*
		* \param[i] vector
		*		The 3D scaled vector to add.
		*
		* \param[in] scale
		*		Additional scale of the vector
		*/
		constexpr void AddScaledVector(const Vector3<T>& vector, const Vector3<T>& scale) noexcept
		{
			constexpr auto zeroPointFive = constants::ZeroPointFive<T>();
			const auto scaledVector = vector * scale;

			Quaternion q(0,
				scaledVector[0],
				scaledVector[1],
				scaledVector[2]);
			q *= *this;
			r += q.r * zeroPointFive;
			i += q.i * zeroPointFive;
			j += q.j * zeroPointFive;
			k += q.k * zeroPointFive;
		}

		/**
		* \brief
		*		Adds the given vector to this, scaled by the given amount.
		*		This is used to update the orientation quaternion by a rotation
		*		and time.
		*
		* \param[i] vector
		*		The 3D vector to rotate by
		*/
		void RotateByVector(const Vector3<T>& vector)
		{
			Quaternion q(0, vector.X(), vector.Y(), vector.Z());
			(*this) *= q;
		}

		// Converts Euler angles to quaternion angles
		USE_RESULT static constexpr Quaternion EulerToQuaternions(const T pitch, const T roll, const T yaw) noexcept
		{
			constexpr auto zeroPointFive = constants::ZeroPointFive<T>();

			const T cYaw = cos(yaw * zeroPointFive);
			const T sYaw = sin(yaw * zeroPointFive);
			const T cRoll = cos(roll * zeroPointFive);
			const T sRoll = sin(roll * zeroPointFive);
			const T cPitch = cos(pitch * zeroPointFive);
			const T sPitch = sin(pitch * zeroPointFive);

			Quaternion q;
			q.r = cYaw * cRoll * cPitch + sYaw * sRoll * sPitch;
			q.i = cYaw * sRoll * cPitch - sYaw * cRoll * sPitch;
			q.j = cYaw * cRoll * sPitch + sYaw * sRoll * cPitch;
			q.k = sYaw * cRoll * cPitch - cYaw * sRoll * sPitch;

			return q;
		}

		USE_RESULT static constexpr Quaternion EulerToQuaternions(const Vector<T, 3>& axis) noexcept
		{
			return EulerToQuaternions(axis[0], axis[1], axis[2]);
		}

		USE_RESULT constexpr void Rotate(const T degrees, const Vector<Type, 3>& v) noexcept
		{
			const auto halfA = ToRadians(degrees) * constants::ZeroPointFive<T>();
			const auto c = cos(halfA);
			const auto s = sin(halfA);

			Vector3<T> norm = v;

			if (v.MagnitudeSQ() != 1)
				norm = v.Normalize();

			r = c;
			i = norm[0] * s;
			j = norm[1] * s;
			k = norm[2] * s;
		}

		/**
		 * \brief
		 *		Calculates the new value for the given matrix transformation
		 * \param[in] pos
		 *		Object's current position
		 */
		USE_RESULT constexpr TransformMatrix<T> CalculateTransformMatrix(const Vector3<T>& pos) const noexcept
		{
			constexpr auto one = constants::One<T>();
			constexpr auto two = CAST(T, 2);

			TransformMatrix<T> mat;
			mat[0][0] = one - two * (j * j - k * k);
			mat[0][1] = two * (i * j - r * k);
			mat[0][2] = two * (i * k + r * j);
			mat[0][3] = 0;

			mat[1][0] = two * (i * j + r * k);
			mat[1][1] = one - two * (i * i - k * k);
			mat[1][2] = two * (j * k - r * i);
			mat[1][3] = 0;

			mat[2][0] = two * (i * k - r * j);
			mat[2][1] = two * (j * k + r * i);
			mat[2][2] = one - two * (i * i - j * j);
			mat[2][3] = 0;

			mat[3][0] = pos[0];
			mat[3][1] = pos[1];
			mat[3][2] = pos[2];
			mat[3][3] = one;
			return mat;
		}




		// Operators

		USE_RESULT constexpr Quaternion operator+(const Quaternion& other) const noexcept
		{
			Quaternion q;
			q.r = r + other.r;
			q.i = i + other.i;
			q.j = j + other.j;
			q.k = k + other.k;
			return q;
		}

		USE_RESULT constexpr Quaternion operator-(const Quaternion& other) const noexcept
		{
			Quaternion q;
			q.r = r - other.r;
			q.i = i - other.i;
			q.j = j - other.j;
			q.k = k - other.k;
			return q;
		}

		/**
		* \brief
		*		Multiplies the quaternion by the given quaternion.
		*
		* \param[in] other
		*		The quaternion by which to multiply.
		*/
		USE_RESULT constexpr Quaternion operator *(const Quaternion& other) const noexcept
		{
			Quaternion q;
			q.r = r * other.r - i * other.i -
				j * other.j - k * other.k;
			q.i = r * other.i + i * other.r +
				j * other.k - k * other.j;
			q.j = r * other.j + j * other.r +
				k * other.i - i * other.k;
			q.k = r * other.k + k * other.r +
				i * other.j - j * other.i;
			return q;
		}

		constexpr Quaternion& operator +=(const Quaternion& other) noexcept
		{
			*this = *this + other;
			return *this;
		}

		constexpr Quaternion& operator -=(const Quaternion& other) noexcept
		{
			*this = *this - other;
			return *this;
		}

		/**
		* \brief
		*		Multiplies the quaternion by the given quaternion.
		*
		* \param[in] other
		*		The quaternion by which to multiply.
		*/
		constexpr Quaternion& operator *=(const Quaternion& other) noexcept
		{
			r = r * other.r - i * other.i -
				j * other.j - k * other.k;
			i = r * other.i + i * other.r +
				j * other.k - k * other.j;
			j = r * other.j + j * other.r +
				k * other.i - i * other.k;
			k = r * other.k + k * other.r +
				i * other.j - j * other.i;

			return *this;

		}

	public:
		T r;	// Holds the real component of the quaternion.
		T i;	// First complex component of the quaternion.
		T j;	// Second complex component of the quaternion.
		T k;	// Third complex component of the quaternion.
	};


	using Quaternionf = Quaternion<float>;
	using Quaterniond = Quaternion<double>;
}
