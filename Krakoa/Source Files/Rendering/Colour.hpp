#pragma once

#include <Maths/Vectors/Vector3.hpp>
#include <Maths/Vectors/Vector4.hpp>

#include <Maths/kAlgorithms.hpp>

#include <cstdint>

namespace krakoa::graphics
{
	class Colour
	{
		struct HSL
		{
			float hue = 0, saturation = 0, lightness = 0;
		};

	public:
		inline static constexpr auto Length = 4;
		inline static constexpr auto Bytes = Length * sizeof(uint8_t);

		inline static constexpr uint8_t MinColourValue = 0;
		inline static constexpr uint8_t MaxColourValue = 255;

	public:
		constexpr Colour() noexcept;
		constexpr Colour(float r, float g, float b, float a = 1.f) noexcept;
		constexpr Colour(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a = MaxColourValue) noexcept;
		constexpr Colour(const kmaths::Vector4f& colourSet) noexcept;
		constexpr Colour(const kmaths::Vector4<uint8_t>& colourSet) noexcept;

		constexpr void SetHSL(HSL hsl) noexcept;
		USE_RESULT constexpr HSL GetHSL(float hue, float saturation, float lightness) const noexcept;

		USE_RESULT constexpr kmaths::Vector4f GetColour() const noexcept;

		USE_RESULT constexpr Colour Inverse() const noexcept;

		// Operators
		USE_RESULT constexpr Colour operator+(const Colour& c) const noexcept;
		USE_RESULT constexpr Colour operator-(const Colour& c) const noexcept;
		
		USE_RESULT Colour operator+=(const Colour& c) noexcept;
		USE_RESULT Colour operator-=(const Colour& c) noexcept;

		template<typename T, class = std::enable_if_t<std::is_arithmetic_v<T>>>
		USE_RESULT constexpr Colour operator*(const T scalar) const noexcept
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
		USE_RESULT Colour operator*=(const T scalar) noexcept
		{
			*this = *this * scalar;
			return *this;
		}
		
		
		template<typename T, class = std::enable_if_t<std::is_arithmetic_v<T>>>
		USE_RESULT Colour operator/=(const T scalar) noexcept
		{
			*this = *this / scalar;
			return *this;
		}
		
	private:
		uint8_t red{};
		uint8_t green{};
		uint8_t blue{};
		uint8_t alpha{};
	};

	namespace colours
	{
		inline static constexpr Colour Red = Colour(Colour::MaxColourValue, uint8_t(), uint8_t());
		inline static constexpr Colour Green(uint8_t(), Colour::MaxColourValue, uint8_t());
		inline static constexpr Colour Blue(uint8_t(), uint8_t(), Colour::MaxColourValue);
		inline static constexpr Colour Magenta(Colour::MaxColourValue, uint8_t(), Colour::MaxColourValue);
		inline static constexpr Colour Cyan(uint8_t(), Colour::MaxColourValue, Colour::MaxColourValue);
		inline static constexpr Colour Yellow(Colour::MaxColourValue, Colour::MaxColourValue, uint8_t());
		inline static constexpr Colour White(Colour::MaxColourValue, Colour::MaxColourValue, Colour::MaxColourValue);
		inline static constexpr Colour Black(Colour::MinColourValue, Colour::MinColourValue, Colour::MinColourValue);
	}
}
