#pragma once

#include <Maths/Vectors/Vector3.hpp>
#include <Maths/Vectors/Vector4.hpp>

#include <Maths/kAlgorithms.hpp>
#include <Template/kSimpleOperators.hpp>

#include <cstdint>

namespace krakoa::graphics
{
	using namespace kmaths;

	class Colour final
	{
		struct HSL
		{
			float hue = 0, saturation = 0, lightness = 0;
		};

		class ColourComponent : klib::kTemplate::SimpleComparisonOperators<ColourComponent>
		{
		public:
			constexpr ColourComponent(std::uint8_t value = 0)
				: value(value)
			{}

			constexpr std::uint8_t GetValue() const noexcept
			{
				return value;
			}

			ColourComponent operator+(const ColourComponent& other) const
			{
				const auto opVal = static_cast<int>(value) + static_cast<int>(other.value);
				const auto res = kmaths::Clamp(opVal, 0, 255);
				return { static_cast<std::uint8_t>(res) };
			}

			ColourComponent operator-(const ColourComponent& other) const
			{
				const auto opVal = static_cast<int>(value) - static_cast<int>(other.value);
				const auto res = kmaths::Clamp(opVal, 0, 255);
				return { static_cast<std::uint8_t>(res) };
			}

			ColourComponent operator*(const ColourComponent& other) const
			{
				const auto opVal = static_cast<int>(value) * static_cast<int>(other.value);
				const auto res = kmaths::Clamp(opVal, 0, 255);
				return { static_cast<std::uint8_t>(res) };
			}

			ColourComponent operator/(const ColourComponent& other) const
			{
				const auto opVal = static_cast<float>(value) / static_cast<float>(other.value);
				const auto res = kmaths::Clamp(opVal, 0, 255);
				return { static_cast<std::uint8_t>(res) };
			}

			template<typename T>
			ColourComponent operator+(const T& other) const
			{
				const auto opVal = static_cast<int>(value) + static_cast<int>(other);
				const auto res = kmaths::Clamp(opVal, 0, 255);
				return { static_cast<std::uint8_t>(res) };
			}

			template<typename T>
			ColourComponent operator-(const T& other) const
			{
				const auto opVal = static_cast<int>(value) - static_cast<int>(other);
				const auto res = kmaths::Clamp(opVal, 0, 255);
				return { static_cast<std::uint8_t>(res) };
			}

			template<typename T>
			ColourComponent operator*(const T& other) const
			{
				const auto opVal = static_cast<int>(value) * static_cast<int>(other);
				const auto res = kmaths::Clamp(opVal, 0, 255);
				return { static_cast<std::uint8_t>(res) };
			}

			template<typename T>
			ColourComponent operator/(const T& other) const
			{
				const auto opVal = static_cast<float>(value) / static_cast<float>(other);
				const auto res = kmaths::Clamp(opVal, 0, 255);
				return { static_cast<std::uint8_t>(res) };
			}
			
			template<typename T>
			constexpr operator T() const noexcept
			{
				return value;
			}

		private:
			std::uint8_t value;
		};

	public:
		inline static constexpr auto Length = 4;
		inline static constexpr auto Bytes = Length * sizeof(std::uint8_t);

		inline static constexpr std::uint8_t MinColourValue = 0;
		inline static constexpr std::uint8_t MaxColourValue = 255;

	public:
		constexpr Colour() noexcept
			= default;

		constexpr Colour(const Colour& other) noexcept
		{
			*this = other;
		}

		constexpr Colour(Colour&& other) noexcept
		{
			*this = other;
		}

		/**
		 * \brief
		 *		Assumes floats in the range of 0..1 and creates a colour from the values
		 * \param r
		 *		red percentage
		 * \param g
		 *		green percentage
		 * \param b
		 *		blue percentage
		 * \param a
		 *		alpha percentage
		 */
		constexpr Colour(const float r, const float g, const float b, const float a = 1.f) noexcept
			: red(FloatToColour(r)),
			green(FloatToColour(g)),
			blue(FloatToColour(b)),
			alpha(FloatToColour(a))
		{}

		/**
		 * \brief
		 *		Assumes uint8_t in the range of 0..255 and creates a colour from the values
		 * \param r
		 *		red amount
		 * \param g
		 *		green amount
		 * \param b
		 *		blue amount
		 * \param a
		 *		alpha amount
		 */
		constexpr Colour(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a = MaxColourValue) noexcept
			: red(r), green(g), blue(b), alpha(a)
		{}

		constexpr Colour(const Vector3f& rgb, const float a = 1.f) noexcept
			: red(FloatToColour(rgb[0])),
			green(FloatToColour(rgb[1])),
			blue(FloatToColour(rgb[2])),
			alpha(FloatToColour(a))
		{}

		constexpr Colour(const Vector4f& rgba) noexcept
			: red(FloatToColour(rgba[0])),
			green(FloatToColour(rgba[1])),
			blue(FloatToColour(rgba[2])),
			alpha(FloatToColour(rgba[3]))
		{}

		constexpr Colour(const Vector3<uint8_t>& rgb, const uint8_t a = MaxColourValue) noexcept
			: red(rgb[0]), green(rgb[1]),
			blue(rgb[2]), alpha(a)
		{}

		constexpr Colour(const Vector4<uint8_t>& rgba) noexcept
			: red(rgba[0]), green(rgba[1]),
			blue(rgba[2]), alpha(rgba[3])
		{}

		constexpr Colour(const Colour& rgb, uint8_t alpha) noexcept
			: red(rgb.red), green(rgb.green),
			blue(rgb.blue), alpha(alpha)
		{}

		~Colour() noexcept
			= default;

		USE_RESULT constexpr uint8_t Red() const noexcept
		{
			return red;
		}

		USE_RESULT constexpr uint8_t Green() const noexcept
		{
			return green;
		}

		USE_RESULT constexpr uint8_t Blue() const noexcept
		{
			return blue;
		}

		USE_RESULT constexpr uint8_t Alpha() const noexcept
		{
			return alpha;
		}

		void SetHSL(HSL hsl) noexcept
		{
			const auto flHue6 = hsl.hue / 60.f;
			const auto flChroma = (1.f - Abs(2 * hsl.lightness - 1.f)) * hsl.saturation;
			const auto flX = flChroma * (1.f - Abs(FloatRemainder(flHue6, 2.f) - 1.f));

			float flR1 = 0, flG1 = 0, flB1 = 0;
			if (flHue6 < 1)
			{
				flR1 = flChroma;
				flG1 = flX;
				flB1 = 0;
			}
			else if (flHue6 < 2)
			{
				flR1 = flX;
				flG1 = flChroma;
				flB1 = 0;
			}
			else if (flHue6 < 3)
			{
				flR1 = 0;
				flG1 = flChroma;
				flB1 = flX;
			}
			else if (flHue6 < 4)
			{
				flR1 = 0;
				flG1 = flX;
				flB1 = flChroma;
			}
			else if (flHue6 < 5)
			{
				flR1 = flX;
				flG1 = 0;
				flB1 = flChroma;
			}
			else
			{
				flR1 = flChroma;
				flG1 = 0;
				flB1 = flX;
			}

			const float flM = hsl.lightness - flChroma * .5f;

			*this = Colour(flR1 + flM, flG1 + flM, flB1 + flM);
		}

		USE_RESULT constexpr HSL GetHSL(float hue, float saturation, float lightness) const noexcept
		{
			const float r = CAST(float, red) / MaxColourValue;
			const float g = CAST(float, green) / MaxColourValue;
			const float b = CAST(float, blue) / MaxColourValue;

			const float maximum = Max(Max(r, g), b);
			const float minimum = Min(Min(r, g), b);

			const float flChroma = maximum - minimum;

			if (flChroma == 0)
				hue = 0;
			else if (r > g && r > b)
				hue = FloatRemainder((g - b) / flChroma, 6.f) * 60.f;
			else if (g > r && g > b)
				hue = ((b - r) / flChroma + 2) * 60.f;
			else if (b > r && b > g)
				hue = ((r - g) / flChroma + 4) * 60.f;

			lightness = (maximum + minimum) * 0.5f;

			if (flChroma > 0)
				saturation = flChroma / (1.f - Abs(2.f * lightness - 1.f));

			return { hue, saturation, lightness };
		}

		USE_RESULT constexpr Vector3f GetRGB() const noexcept
		{
			constexpr auto multiplier = kmaths::constants::OneOver<float>(MaxColourValue);
			const auto r = red.GetValue() * multiplier;
			const auto g = green.GetValue() * multiplier;
			const auto b = blue.GetValue() * multiplier;

			return Vector3f{ r, g, b };
		}

		USE_RESULT constexpr Vector4f GetRGBA() const noexcept
		{
			constexpr auto multiplier = kmaths::constants::OneOver<float>(MaxColourValue);
			const auto rgb = GetRGB();
			const auto a = alpha.GetValue() * multiplier;

			return Vector4f{ rgb[0], rgb[1], rgb[2], a };
		}

		USE_RESULT constexpr Colour Inverse() const noexcept
		{
			return Colour(static_cast<uint8_t>(MaxColourValue - red.GetValue())
				, CAST(uint8_t, MaxColourValue - green.GetValue())
				, CAST(uint8_t, MaxColourValue - blue.GetValue())
				, alpha);
		}

		// Operators
		USE_RESULT constexpr Colour operator+(const Colour& c) const noexcept
		{
			const auto r = kmaths::Clamp<std::uint8_t>((red + c.red), MinColourValue, MaxColourValue);
			const auto g = kmaths::Clamp<std::uint8_t>((green + c.green), MinColourValue, MaxColourValue);
			const auto b = kmaths::Clamp<std::uint8_t>((blue + c.blue), MinColourValue, MaxColourValue);
			const auto a = kmaths::Clamp<std::uint8_t>((alpha + c.alpha), MinColourValue, MaxColourValue);

			return { r, g, b, a };
		}

		USE_RESULT constexpr Colour operator+(const kmaths::Vector4f& v) const noexcept
		{
			const auto r = kmaths::Clamp<std::uint8_t>(red + FloatToColour(v[0]), MinColourValue, MaxColourValue);
			const auto g = kmaths::Clamp<std::uint8_t>((green + FloatToColour(v[1])), MinColourValue, MaxColourValue);
			const auto b = kmaths::Clamp<std::uint8_t>((blue + FloatToColour(v[2])), MinColourValue, MaxColourValue);
			const auto a = kmaths::Clamp<std::uint8_t>((alpha + FloatToColour(v[3])), MinColourValue, MaxColourValue);

			return { r, g, b, a };
		}

		USE_RESULT constexpr Colour operator-(const Colour& c) const noexcept
		{
			const auto r = kmaths::Clamp<uint8_t>((red - c.red), MinColourValue, MaxColourValue);
			const auto g = kmaths::Clamp<uint8_t>((green - c.green), MinColourValue, MaxColourValue);
			const auto b = kmaths::Clamp<uint8_t>((blue - c.blue), MinColourValue, MaxColourValue);
			const auto a = kmaths::Clamp<uint8_t>((alpha - c.alpha), MinColourValue, MaxColourValue);

			return { r, g, b, a };
		}

		USE_RESULT constexpr Colour operator-(const kmaths::Vector4f& v) const noexcept
		{
			const auto r = kmaths::Clamp<std::uint8_t>(red - FloatToColour(v[0]), MinColourValue, MaxColourValue);
			const auto g = kmaths::Clamp<std::uint8_t>((green - FloatToColour(v[1])), MinColourValue, MaxColourValue);
			const auto b = kmaths::Clamp<std::uint8_t>((blue - FloatToColour(v[2])), MinColourValue, MaxColourValue);
			const auto a = kmaths::Clamp<std::uint8_t>((alpha - FloatToColour(v[3])), MinColourValue, MaxColourValue);

			return { r, g, b, a };
		}

		USE_RESULT Colour& operator+=(const Colour& c) noexcept
		{
			*this = *this + c;
			return *this;
		}

		USE_RESULT Colour& operator-=(const Colour& c) noexcept
		{
			*this = *this - c;
			return *this;
		}

		template <typename T, class = std::enable_if_t<std::is_arithmetic_v<T>>>
		constexpr Colour operator*(const T scalar) noexcept
		{
			const auto r = kmaths::Clamp<uint8_t>((red * scalar), MinColourValue, MaxColourValue);
			const auto g = kmaths::Clamp<uint8_t>((green * scalar), MinColourValue, MaxColourValue);
			const auto b = kmaths::Clamp<uint8_t>((blue * scalar), MinColourValue, MaxColourValue);

			return { r, g, b, alpha };
		}

		template<typename T, class = std::enable_if_t<std::is_arithmetic_v<T>>>
		USE_RESULT constexpr Colour operator/(const T scalar) const noexcept
		{
			const auto r = kmaths::Clamp<uint8_t>((red / scalar), MinColourValue, MaxColourValue);
			const auto g = kmaths::Clamp<uint8_t>((green / scalar), MinColourValue, MaxColourValue);
			const auto b = kmaths::Clamp<uint8_t>((blue / scalar), MinColourValue, MaxColourValue);

			return { r, g, b, alpha };
		}


		template<typename T, class = std::enable_if_t<std::is_arithmetic_v<T>>>
		USE_RESULT Colour& operator*=(const T scalar) noexcept
		{
			*this = *this * scalar;
			return *this;
		}


		template<typename T, class = std::enable_if_t<std::is_arithmetic_v<T>>>
		USE_RESULT Colour& operator/=(const T scalar) noexcept
		{
			*this = *this / scalar;
			return *this;
		}

		constexpr Colour& operator=(const Colour& other) noexcept
			= default;

		template<class Float_t = float>
		USE_RESULT constexpr Vector4<Float_t> ToFloats() const noexcept
		{
			static_assert(std::is_floating_point_v<Float_t>, "Type must be a floating point data type");

			const auto r = constants::Divide<Float_t>(red, MaxColourValue);
			const auto g = constants::Divide<Float_t>(green, MaxColourValue);
			const auto b = constants::Divide<Float_t>(blue, MaxColourValue);
			const auto a = constants::Divide<Float_t>(alpha, MaxColourValue);
			return Vector4<Float_t>{ r, g, b, a };
		}

		USE_RESULT constexpr std::uint32_t ToU32() const noexcept
		{
			std::uint32_t value = 0;
			value |= (red.GetValue() << 24);
			value |= (green.GetValue() << 16);
			value |= (blue.GetValue() << 8);
			value |= alpha.GetValue();
			return value;
		}
		
		template<class T>
		USE_RESULT constexpr auto ToArray() const noexcept
		{
			if _CONSTEXPR17(std::is_floating_point_v<T>)
			{
				const auto vec = ToFloats<T>();
				return std::array<T, 4>{
					vec[0]
						, vec[1]
						, vec[2]
						, vec[3]
				};
			}
			else
			{
				return std::array<T, 4>{
					static_cast<T>(red)
						, static_cast<T>(green)
						, static_cast<T>(blue)
						, static_cast<T>(alpha)
				};
			}
		}

	private:
		USE_RESULT constexpr std::uint8_t FloatToColour(float c) const
		{
			c = kmaths::Clamp<float>(c, MinColourValue, 1.f);

			return CAST(uint8_t, c * MaxColourValue);
		}


	private:
		ColourComponent red{};
		ColourComponent green{};
		ColourComponent blue{};
		ColourComponent alpha{255};
	};

	template <typename T, class = std::enable_if_t<std::is_arithmetic_v<T>>>
	constexpr Colour operator*(const T scalar, const Colour& c) noexcept
	{
		const auto r = kmaths::Clamp<uint8_t>((c.Red() * scalar), Colour::MinColourValue, Colour::MaxColourValue);
		const auto g = kmaths::Clamp<uint8_t>((c.Green() * scalar), Colour::MinColourValue, Colour::MaxColourValue);
		const auto b = kmaths::Clamp<uint8_t>((c.Blue() * scalar), Colour::MinColourValue, Colour::MaxColourValue);

		return { r, g, b, c.Alpha() };
	}

	template <typename T, class = std::enable_if_t<std::is_arithmetic_v<T>>>
	constexpr Colour operator/(const T scalar, const Colour& c) noexcept
	{
		const auto multiplier = constants::OneOver<float>(scalar);

		const auto r = kmaths::Clamp<uint8_t>((c.Red() / multiplier), Colour::MinColourValue, Colour::MaxColourValue);
		const auto g = kmaths::Clamp<uint8_t>((c.Green() / multiplier), Colour::MinColourValue, Colour::MaxColourValue);
		const auto b = kmaths::Clamp<uint8_t>((c.Blue() / multiplier), Colour::MinColourValue, Colour::MaxColourValue);

		return { r, g, b, c.Alpha() };
	}

	namespace colours
	{
		inline static constexpr Colour Red(Colour::MaxColourValue, Colour::MinColourValue, Colour::MinColourValue);
		inline static constexpr Colour Green(Colour::MinColourValue, Colour::MaxColourValue, Colour::MinColourValue);
		inline static constexpr Colour Blue(Colour::MinColourValue, Colour::MinColourValue, Colour::MaxColourValue);
		inline static constexpr Colour Magenta(Colour::MaxColourValue, Colour::MinColourValue, Colour::MaxColourValue);
		inline static constexpr Colour Cyan(Colour::MinColourValue, Colour::MaxColourValue, Colour::MaxColourValue);
		inline static constexpr Colour Yellow(Colour::MaxColourValue, Colour::MaxColourValue, Colour::MinColourValue);
		inline static constexpr Colour White(Colour::MaxColourValue, Colour::MaxColourValue, Colour::MaxColourValue);
		inline static constexpr Colour Black(Colour::MinColourValue, Colour::MinColourValue, Colour::MinColourValue);
		inline static constexpr Colour Orange(0.85f, 0.35f, 0.f, 1.f);
		inline static constexpr Colour Invisible(White.GetRGB(), 0);
	}
}
