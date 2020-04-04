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
#include "../Vectors/Vector3.hpp"
#include "../Matrices/Matrix4x4.hpp"

#include <limits>

namespace kmaths
{
	template<typename T>
	struct Quaternion
	{
	public:
		static_assert(std::is_floating_point_v<T>, "Must be instantiated with a floating point type");

		using Type = T;

		/**
		* The default constructor creates a quaternion representing
		* a zero rotation.
		*/
		constexpr Quaternion() noexcept
			: r(1), i(0), j(0), k(0)
		{}

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
		explicit constexpr Quaternion(const T r, const T i, const T j, const T k) noexcept
			: r(r), i(i), j(j), k(k)
		{}

		/**
		* normalizes the quaternion to unit length, making it a valid
		* orientation quaternion.
		*/
		constexpr void Normalize() noexcept
		{
			const T magSQ = r * r + i * i + j * j + k * k;

			// Check for zero length quaternion, and use the no-rotation
			// quaternion in that case.
			if (magSQ < std::numeric_limits<T>::epsilon())
			{
				r = 1;
				return;
			}

			const T d = CAST(T, 1) / sqrt(magSQ);

			r *= d;
			i *= d;
			j *= d;
			k *= d;
		}

		/**
		* \brief
		*		Multiplies the quaternion by the given quaternion.
		*
		* \param[in] multiplier
		*		The quaternion by which to multiply.
		*/
		constexpr void operator *=(const Quaternion &multiplier) noexcept
		{
			r = r * multiplier.r - i * multiplier.i -
				j * multiplier.j - k * multiplier.k;
			i = r * multiplier.i + i * multiplier.r +
				j * multiplier.k - k * multiplier.j;
			j = r * multiplier.j + j * multiplier.r +
				k * multiplier.i - i * multiplier.k;
			k = r * multiplier.k + k * multiplier.r +
				i * multiplier.j - j * multiplier.i;
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
		constexpr void addScaledVector(const Vector3<T>& vector, T scale) noexcept
		{
			Quaternion q(0,
				vector.X() * scale,
				vector.Y() * scale,
				vector.Z() * scale);
			q *= *this;
			r += q.r * CAST(T, 0.5);
			i += q.i * CAST(T, 0.5);
			j += q.j * CAST(T, 0.5);
			k += q.k * CAST(T, 0.5);
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
		void rotateByVector(const Vector3<T>& vector)
		{
			Quaternion q(0, vector.X(), vector.Y(), vector.Z());
			(*this) *= q;
		}

		// Converts Euler angles to quaternion angles
		USE_RESULT constexpr Quaternion EulerToQuaternions(const T pitch, const T roll, const T yaw) noexcept
		{
			const T cYaw = cos(yaw   * CAST(T, 0.5));
			const T sYaw = sin(yaw   * CAST(T, 0.5));
			const T cRoll = cos(roll  * CAST(T, 0.5));
			const T sRoll = sin(roll  * CAST(T, 0.5));
			const T cPitch = cos(pitch * CAST(T, 0.5));
			const T sPitch = sin(pitch * CAST(T, 0.5));

			Quaternion q;
			q.r = cYaw * cRoll * cPitch + sYaw * sRoll * sPitch;
			q.i = cYaw * sRoll * cPitch - sYaw * cRoll * sPitch;
			q.j = cYaw * cRoll * sPitch + sYaw * sRoll * cPitch;
			q.k = sYaw * cRoll * cPitch - cYaw * sRoll * sPitch;

			return q;
		}
		/**
		 * \brief
		 *		Calculates the new value for the given matrix transformation
		 * \param[out] outTransformMatrix
		 *		Matrix transformation
		 * \param[in] pos
		 *		Object's current position
		 */
		template<typename T2, typename T3>
		constexpr void CalculateTransformMatrix(Matrix4x4<T2>& outTransformMatrix, const Vector3<T3>& pos)
		{
			outTransformMatrix[0][0] = 1 - 2 * (j*j - k*k);
			outTransformMatrix[0][1] = 2 * (i*j - r*k);
			outTransformMatrix[0][2] = 2 * (i*k + r*j);
			outTransformMatrix[0][3] = 0;

			outTransformMatrix[1][0] = 2 * (i * j + r*k);
			outTransformMatrix[1][1] = 1 - 2 * (i*i - k*k);
			outTransformMatrix[1][2] = 2 * (j*k - r*i);
			outTransformMatrix[1][3] = 0;

			outTransformMatrix[2][0] = 2 * (i*k - r*j);
			outTransformMatrix[2][1] = 2 * (j*k + r*i);
			outTransformMatrix[2][2] = 1 - 2 * (i*i - j*j);
			outTransformMatrix[2][3] = 0;

			outTransformMatrix[3][0] = CAST(T, pos.X());
			outTransformMatrix[3][1] = CAST(T, pos.Y());
			outTransformMatrix[3][2] = CAST(T, pos.Z());
			outTransformMatrix[3][3] = 1;
		}

	public:
		T r;	// Holds the real component of the quaternion.
		T i;	// First complex component of the quaternion.
		T j;	// Second complex component of the quaternion.
		T k;	// Third complex component of the quaternion.
	};

	using Quaternionf = Quaternion<float>;
	using Quaterniond = Quaternion<double>;

	/**
	* Inline function that creates a transform matrix from a
	* position and orientation.
	*/
	//static inline void CalculateTransformMatrix(XMMATRIX &transformMatrix,
	//	const Vector3f &position,
	//	const Quaternion &orientation)
	//{
	//	transformMatrix.r[0] = XMVectorSetX(transformMatrix.r[0], 1 - 2 * orientation.j*orientation.j -
	//		2 * orientation.k*orientation.k);
	//	transformMatrix.r[0] = XMVectorSetY(transformMatrix.r[0], 2 * orientation.i*orientation.j -
	//		2 * orientation.r*orientation.k);
	//	transformMatrix.r[0] = XMVectorSetZ(transformMatrix.r[0], 2 * orientation.i*orientation.k +
	//		2 * orientation.r*orientation.j);
	//	transformMatrix.r[0] = XMVectorSetW(transformMatrix.r[0], 0.0f);

	//	transformMatrix.r[1] = XMVectorSetX(transformMatrix.r[1], 2 * orientation.i*orientation.j +
	//		2 * orientation.r*orientation.k);
	//	transformMatrix.r[1] = XMVectorSetY(transformMatrix.r[1], 1 - 2 * orientation.i*orientation.i -
	//		2 * orientation.k*orientation.k);
	//	transformMatrix.r[1] = XMVectorSetZ(transformMatrix.r[1], 2 * orientation.j*orientation.k -
	//		2 * orientation.r*orientation.i);
	//	transformMatrix.r[1] = XMVectorSetW(transformMatrix.r[1], 0.0f);

	//	transformMatrix.r[2] = XMVectorSetX(transformMatrix.r[2], 2 * orientation.i*orientation.k -
	//		2 * orientation.r*orientation.j);
	//	transformMatrix.r[2] = XMVectorSetY(transformMatrix.r[2], 2 * orientation.j*orientation.k +
	//		2 * orientation.r*orientation.i);
	//	transformMatrix.r[2] = XMVectorSetZ(transformMatrix.r[2], 1 - 2 * orientation.i*orientation.i -
	//		2 * orientation.j*orientation.j);
	//	transformMatrix.r[2] = XMVectorSetW(transformMatrix.r[2], 0.0f);

	//	transformMatrix.r[3] = XMVectorSetX(transformMatrix.r[3], position.x);
	//	transformMatrix.r[3] = XMVectorSetY(transformMatrix.r[3], position.y);
	//	transformMatrix.r[3] = XMVectorSetZ(transformMatrix.r[3], position.z);
	//	transformMatrix.r[3] = XMVectorSetW(transformMatrix.r[3], 1.0f);
	//}
}
