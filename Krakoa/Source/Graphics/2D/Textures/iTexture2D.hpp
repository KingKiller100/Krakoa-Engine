#pragma once

#include "../../Textures/iTexture.hpp"

namespace krakoa::graphics
{
	class iTexture2D : public iTexture
	{
	public:
		static iTexture2D* Create(const uint32_t width, const uint32_t height);
		static iTexture2D* Create(const std::filesystem::path& path, const bool cache = false);
	};
}
