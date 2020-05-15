#pragma once

#include "../../HelperMacros.hpp"

#include "../Constants.hpp"
#include "../Vectors/Vector3.hpp"
#include "../Matrices/TransformMatrix.hpp"

#include <limits>

namespace kmaths
{
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
		explicit constexpr Quaternion(const T degrees = 0, const T x = CAST(T, 0), const T y = CAST(T, 0), const T z = CAST(T, 0)) noexcept
		{
			constexpr auto zeroPointFive = constants::ZeroPointFive<T>();
			const auto rads = ToRadians(degrees);
			const auto halfRads = rads * zeroPointFive;
			const auto sinRads = sin(halfRads);


			w = cos(halfRads);
			v = Vector3<T>(x * sinRads, y * sinRads, z * sinRads);
		}

		// Vector must be normalized
		explicit constexpr Quaternion(const T degrees, Vector<T, 3> n) noexcept
		{
			constexpr auto zeroPointFive = constants::ZeroPointFive<T>();
			const auto rads = ToRadians(degrees);
			const auto halfRads = rads * zeroPointFive;

			if (n.MagnitudeSQ() != 1)
				n = n.Normalize();

			w = cos(halfRads);
			v = n * sin(halfRads);
		}


		USE_RESULT constexpr T MagnitudeSQ() const noexcept
		{
			return (w * w) + v.MagnitudeSQ();
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
				w = constants::One<T>();
				return;
			}

			const T d = constants::OneOver<T>(Sqrt<T>(magSQ));

			w *= d;
			v *= d;
		}

		USE_RESULT constexpr Quaternion Inverted() const noexcept
		{
			Quaternion q;
			q.w = w;
			q.v = -v;
			return q;
		}

		USE_RESULT constexpr T DotProduct(const Quaternion& other) const noexcept
		{
			const auto dotAxes = v.DotProduct(other.v);
			const auto dotRotation = w * other.w;
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
			w += q.w * zeroPointFive;
			v += (q.v * zeroPointFive);
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
		constexpr void RotateByVector(const Vector3<T>& vector)
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
			q.w = cYaw * cRoll * cPitch + sYaw * sRoll * sPitch;
			q.v[0] = cYaw * sRoll * cPitch - sYaw * cRoll * sPitch;
			q.v[1] = cYaw * cRoll * sPitch + sYaw * sRoll * cPitch;
			q.v[2] = sYaw * cRoll * cPitch - cYaw * sRoll * sPitch;

			return q;
		}

		USE_RESULT static constexpr Quaternion EulerToQuaternions(const Vector<T, 3>& axis) noexcept
		{
			return EulerToQuaternions(axis[0], axis[1], axis[2]);
		}

		USE_RESULT constexpr void Rotate(const T degrees, const Vector<Type, 3>& n) noexcept
		{
			const auto halfA = ToRadians(degrees) * constants::ZeroPointFive<T>();
			const auto c = cos(halfA);
			const auto s = sin(halfA);

			Vector3<T> norm = n;

			if (n.MagnitudeSQ() != 1)
				norm = n.Normalize();

			w = c;
			v = norm * s;
		}

		/**
		 * \brief
		 *		Calculates the new value for the given matrix transformation
		 * \param[in] pos
		 *		Object's current position
		 */
		USE_RESULT constexpr TransformMatrix<T> CalculateTransformMatrix(const Vector3<T>& position) const noexcept
		{
			constexpr auto one = constants::One<T>();
			constexpr auto two = CAST(T, 2);

			const auto i = v[0];
			const auto j = v[1];
			const auto k = v[2];

			TransformMatrix<T> mat;
			mat[0][0] = one - two * (j * j - k * k);
			mat[0][1] = two * (i * j - w * k);
			mat[0][2] = two * (i * k + w * j);
			mat[0][3] = 0;

			mat[1][0] = two * (i * j + w * k);
			mat[1][1] = one - two * (i * i - k * k);
			mat[1][2] = two * (j * k - w * i);
			mat[1][3] = 0;

			mat[2][0] = two * (i * k - w * j);
			mat[2][1] = two * (j * k + w * i);
			mat[2][2] = one - two * (i * i - j * j);
			mat[2][3] = 0;

			mat[3][0] = position[0];
			mat[3][1] = position[1];
			mat[3][2] = position[2];
			mat[3][3] = one;
			return mat;
		}




		// Operators

		USE_RESULT constexpr Quaternion operator+(const Quaternion& other) const noexcept
		{
			Quaternion q;
			q.w = w + other.w;
			q.v = v + other.v;
			return q;
		}

		USE_RESULT constexpr Quaternion operator-(const Quaternion& other) const noexcept
		{
			Quaternion q;
			q.w = w - other.w;
			q.v = v - other.v;
			return q;
		}

		USE_RESULT constexpr Vector<T, 3> operator *(const Vector<T, 3>& vec) const noexcept
		{
			// Quaternion q;
			// q.w = 0;
			// q.v = vec;

			// Could do it this way:

			// const Quaternion& q = (*this);
			// return (q * p * q.Inverted()).v;


			constexpr auto two = CAST(T, 2);
			const auto crossProduct = v.CrossProduct(vec);
			return vec + (crossProduct * (two * w)) + v.CrossProduct(crossProduct) * two;
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
			q.w = (w * other.w) + v.DotProduct(other.v);
			q.v = (v * other.w) + (other.v * w) + v.CrossProduct(other.v);
			/*q.w = w * other.w - v[0] * other.v[0] -
				v[1] * other.v[1] - v[2] * other.v[2];
			q.v[0] = w * other.v[0] + v[0] * other.w +
				v[1] * other.v[2] - v[2] * other.v[1];
			q.v[1] = w * other.v[1] + v[1] * other.w +
				v[2] * other.v[0] - v[0] * other.v[2];
			q.v[2] = w * other.v[2] + v[2] * other.w +
				v[0] * other.v[1] - v[1] * other.v[0];*/
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
			*this = *this * other;
			return *this;
		}

	public:
		T w;	// Holds the real component of the quaternion.
		Vector<T, 3> v;	// Holds 3 component vector of complex components of the quaternion.
	};


	using Quaternionf = Quaternion<float>;
	using Quaterniond = Quaternion<double>;
}
