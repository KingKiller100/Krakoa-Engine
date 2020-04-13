#pragma once

#include "iTexture.hpp"

#include <memory>
#include <string>

namespace krakoa::graphics
{
	class Texture2D : public iTexture
	{
	public:
		static Texture2D* Create(const std::string_view& path);
	};
}
