﻿#pragma once

#include "iTexture.hpp"

namespace krakoa::graphics
{
	class iTexture2D : public iTexture
	{
	public:
		static iTexture2D* Create(const uint32_t width, const uint32_t height, const bool cache = false);
		static iTexture2D* Create(const std::string_view& path, const bool cache = false);
	};
}