#pragma once

namespace util
{
	namespace kMaths
	{

		template<typename T>
		struct VectorBase;

		// Pure length of the vector
		template<typename T>
		T VectorMagnitudeSQ(const VectorBase<T>& v);

		// Square rooted length of the vector
		template<typename T>
		T VectorMagnitude(const VectorBase<T>& v);

		// Normalizes a vector
		template<typename T>
		VectorBase<T> VectorNormalize(const VectorBase<T>& v);

		// Produces the dot product
		template<typename T>
		T VectorDotProduct(const VectorBase<T>& v, const VectorBase<T>& u);


		template <typename T>
		struct VectorBase
		{
			VectorBase() : x(0), y(0), z(0), w(0)
			{
				Magnitude();
			}

			explicit VectorBase(const T _v, const T _w = 0) : x(_v), y(_v), z(_v), w(_w)
			{
				Magnitude();
			}

			explicit VectorBase(const T _x, const T _y, const T _z, const T _w = 0) : x(_x), y(_y), z(_z), w(_w)
			{
				Magnitude();
			}

			virtual ~VectorBase()
				= default;

			constexpr T Magnitude() const
			{
				return VectorMagnitude(*this);
			}

			// Restricts vector value to max value
			void Truncate(const T max)
			{
				if (VectorBase::Magnitude() > max)
				{
					*this = VectorNormalize(*this) * max;
				}
			}

			// Reassigns values to be positives
			void ToPositives()
			{
				x = x < 0 ? -x : x;
				y = y < 0 ? -y : y;
				z = z < 0 ? -z : z;
			}


			// Calculates distance between two 3D objects
			T Distance(const VectorBase& v)
			{
				return VectorMagnitude(v - *this);
			}


			// Returns vector times by -1 - does not reassign values
			VectorBase ReverseVector()
			{
				this->x *= -1;
				this->y *= -1;
				this->z *= -1;

				return *this;
			}

			// Sets all values of the vector to zero
			void Zero()
			{
				*this = VectorBase();
			}

			T& operator[](size_t index)
			{
				return *(reinterpret_cast<T*>(this) + index);
			}

			// Overloads + operator to add two vectors objects
			VectorBase operator+(const VectorBase& v) const
			{
				return VectorBase(this->x + v.x, this->y + v.y, this->z + v.z);
			}

			// Overloads - operator to subtract two vectors objects
			VectorBase operator-(const VectorBase& v) const
			{
				return VectorBase(this->x - v.x, this->y - v.y, this->z - v.z);
			}

			// Overloads * operator to multiply a vector and float object
			VectorBase operator*(const T f) const
			{
				return VectorBase(this->x * f, this->y * f, this->z * f);
			}

			// Overloads * operator to multiply two vector objects
			VectorBase operator*(const VectorBase& v) const
			{
				return VectorBase(this->x * v.x, this->y * v.y, this->z * v.z);
			}

			// Overloads / operator to divide a vector and float object
			VectorBase operator/(const T f) const
			{
				return VectorBase(this->x / f, this->y / f, this->z / f);
			}

			// Overloads / operator to divide two vectors objects
			VectorBase operator/(const VectorBase& v) const
			{
				return VectorBase(this->x / v.x, this->y / v.y, this->z / v.z);
			}

			// adds to current vector3 value
			VectorBase& operator+=(const VectorBase& v)
			{
				*this = *this + v;
				return *this;
			}

			// divides current vector3 value
			VectorBase& operator-=(const VectorBase& v)
			{
				*this = *this - v;
				return *this;
			}

			// divides current vector3 value and sets variable to it
			VectorBase& operator/=(const VectorBase& v)
			{
				*this = *this / v;
				return *this;
			}

			// divides current vector3 value by a float and sets variable to it
			VectorBase& operator/=(const T f)
			{
				*this = *this / f;
				return *this;
			}

			// multiplies current vector3 value and sets variable to it
			VectorBase& operator*=(const VectorBase& v)
			{
				*this = *this * v;
				return *this;
			}

			// multiply current vector3 value by a float and sets variable to it
			VectorBase& operator*=(const T f)
			{
				*this = *this * f;
				return *this;
			}
			
			// Overloads = operator to make one vector axis values equal to another
			VectorBase& operator=(const VectorBase& v)
			{
				x = v.x;
				y = v.y;
				z = v.z;
				w = v.w;

				return *this;
			}

			// bool operator == returns true if both VectorBaseD values are equal				
			bool operator==(const VectorBase& v) const
			{
				return (this->x == v.x && this->y == v.y && this->z == v.z);
			}

			// bool operator != returns true if both VectorBaseD values are NOT equal			
			bool operator!=(const VectorBase& v) const
			{
				return !(*this == v);
			}

			constexpr bool operator()() const
			{
				return x != 0 || y != 0 || z != 0;
			}
			
		public:
			T x, y, z, w;
		};


		// Pure length of the vector
		template<typename T>
		T VectorMagnitudeSQ(const VectorBase<T>& v)
		{
			return (v.x * v.x) + (v.y * v.y) + (v.z * v.z);
		}

		// Square rooted length of the vector
		template<typename T>
		T VectorMagnitude(const VectorBase<T>& v)
		{
			return std::sqrt(VectorMagnitudeSQ(v));
		}

		// Normalizes a vector
		template<typename T>
		VectorBase<T> VectorNormalize(const VectorBase<T>& v)
		{
			const T mag = v.Magnitude();
			return mag != 0 ? v / mag : v;
		}

		// Produces the dot product
		template<typename T>
		T VectorDotProduct(const VectorBase<T>& v, const VectorBase<T>& u)
		{
			return (u.x * v.x) + (u.y * v.y) + (u.z * v.z);
		}

	}
}