#pragma once

#include "iTexture.hpp"

#include <memory>
#include <string>

namespace krakoa::graphics
{
	class iTexture2D : public iTexture
	{
	public:
		static iTexture2D* Create(const std::string_view& path, const bool cache = false);
	};
}
