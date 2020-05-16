#include "Precompile.hpp"
#include "Colour.hpp"

namespace  krakoa::graphics
{
	constexpr  Colour::Colour() noexcept
		: red(MinColourValue), green(MinColourValue), blue(MinColourValue), alpha(MaxColourValue)
	{}

	constexpr Colour::Colour(float r, float g, float b, float a) noexcept
		: red(CAST(uint8_t, kmaths::Clamp<float>(r, MinColourValue, MaxColourValue))),
		green(CAST(uint8_t, kmaths::Clamp<float>(g, MinColourValue, MaxColourValue))),
		blue(CAST(uint8_t, kmaths::Clamp<float>(b, MinColourValue, MaxColourValue))),
		alpha(CAST(uint8_t, kmaths::Clamp<float>(a, MinColourValue, MaxColourValue)))
	{}

	constexpr Colour::Colour(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a) noexcept
		: red(r), green(g), blue(b), alpha(a)
	{}

	constexpr Colour::Colour(const kmaths::Vector4f& colourSet) noexcept
		: red(CAST(uint8_t, kmaths::Clamp<float>(colourSet[0], MinColourValue, MaxColourValue))),
		green(CAST(uint8_t, kmaths::Clamp<float>(colourSet[1], MinColourValue, MaxColourValue))),
		blue(CAST(uint8_t, kmaths::Clamp<float>(colourSet[2], MinColourValue, MaxColourValue))),
		alpha(CAST(uint8_t, kmaths::Clamp<float>(colourSet[3], MinColourValue, MaxColourValue)))
	{}

	constexpr Colour::Colour(const kmaths::Vector4<uint8_t>& colourSet) noexcept
		: red(colourSet[0]), green(colourSet[1]),
		blue(colourSet[2]), alpha(colourSet[3])
	{}
	
	void Colour::SetHSL(HSL hsl) noexcept
	{
		const auto flHue6 = hsl.hue / 60.f;
		const auto flChroma = (1.f - kmaths::Abs(2 * hsl.lightness - 1.f)) * hsl.saturation;
		const auto flX = flChroma * (1.f - kmaths::Abs(kmaths::FloatingPointRemainder(flHue6, 2.f) - 1.f));

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

		float flM = hsl.lightness - flChroma / 2;

		*this = Colour(flR1 + flM, flG1 + flM, flB1 + flM);
	}

	constexpr Colour::HSL Colour::GetHSL(float hue, float saturation, float lightness) const noexcept
	{
		const float r = CAST(float, red) / MaxColourValue;
		const float g = CAST(float, green) / MaxColourValue;
		const float b = CAST(float, blue) / MaxColourValue;

		const float maximum = kmaths::Max(kmaths::Max(r, g), b);
		const float minimum = kmaths::Min(kmaths::Min(r, g), b);

		const float flChroma = maximum - minimum;

		if (flChroma == 0)
			hue = 0;
		else if (r > g && r > b)
			hue = kmaths::FloatingPointRemainder((g - b) / flChroma, 6.f) * 60.f;
		else if (g > r&& g > b)
			hue = ((b - r) / flChroma + 2) * 60.f;
		else if (b > r&& b > g)
			hue = ((r - g) / flChroma + 4) * 60.f;

		lightness = (maximum + minimum) * 0.5f;

		if (flChroma > 0)
			saturation = flChroma / (1.f - kmaths::Abs(2.f * lightness - 1.f));

		return { hue, saturation, lightness };
	}

	constexpr kmaths::Vector4f Colour::GetColour() const noexcept
	{
		constexpr auto multiplier = kmaths::constants::OneOver<float>(MaxColourValue);
		const auto r = red * multiplier;
		const auto g = green * multiplier;
		const auto b = blue * multiplier;
		const auto a = alpha * multiplier;
		constexpr auto h = colours::Red;
		return kmaths::Vector4f{ r, g, b, a };
	}

	constexpr Colour Colour::Inverse() const noexcept
	{
		return Colour( CAST(uint8_t, MaxColourValue - red), CAST(uint8_t, MaxColourValue - green), CAST(uint8_t, MaxColourValue  - blue), alpha );
	}

	constexpr Colour Colour::operator+(const Colour& c) const noexcept
	{
		const auto r = kmaths::Clamp<uint8_t>((red + c.red), MinColourValue, MaxColourValue);
		const auto g = kmaths::Clamp<uint8_t>((green + c.green), MinColourValue, MaxColourValue);
		const auto b = kmaths::Clamp<uint8_t>((blue + c.blue), MinColourValue, MaxColourValue);
		const auto a = kmaths::Clamp<uint8_t>((alpha + c.alpha), MinColourValue, MaxColourValue);
		 
		return { r, g, b, a };
	}

	constexpr Colour Colour::operator-(const Colour& c) const noexcept
	{
		const auto r = kmaths::Clamp<uint8_t>((red - c.red), MinColourValue, MaxColourValue);
		const auto g = kmaths::Clamp<uint8_t>((green - c.green), MinColourValue, MaxColourValue);
		const auto b = kmaths::Clamp<uint8_t>((blue - c.blue), MinColourValue, MaxColourValue);
		const auto a = kmaths::Clamp<uint8_t>((alpha - c.alpha), MinColourValue, MaxColourValue);
		
		return { r, g, b, a };
	}

	Colour Colour::operator+=(const Colour& c) noexcept
	{
		*this = *this + c;
		return *this;
	}

	Colour Colour::operator-=(const Colour& c) noexcept
	{
		*this = *this - c;
		return *this;
	}
}
