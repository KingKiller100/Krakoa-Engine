#pragma once

#include <ratio>

namespace kmaths
{
	enum class BytesType : size_t
	{
		BYTE = 1,
		KILO = std::kilo::num,
		KIBI = 1024,
		MEGA = std::mega::num,
		MEBI = KIBI * 1024,
		GIGA = std::giga::num,
		GIBI = MEBI * 1024,
		TERA = std::tera::num,
		TEBI = GIBI * 1024,
	};
}
