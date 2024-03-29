﻿#pragma once

#include "../Textures/iTexture2D.hpp"
#include "../Primitives2D/BatchRendererData.hpp"

#include <array>
#include <cstdint>

namespace krakoa::gfx
{
	struct TextureData2D
	{
	public:
		void Reset(const uint32_t index) noexcept
		{
			slotIdx = index;
		}

		virtual void BindTextures() noexcept
		{
			const auto lastIdx = slotIdx;
			for (uint32_t i = 0; i < lastIdx; ++i)
				slots[i]->Bind(i);
		}
		
	public:
		std::array<std::shared_ptr<iTexture2D>, batch::limits::texture::maxSlots> slots;
		uint32_t slotIdx = 1; // White texture index = 0
	};
}