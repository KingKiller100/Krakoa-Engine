﻿#pragma once

#include <Utility/Enum/kEnum.hpp>

#include <filesystem>

struct ImFont;

namespace krakoa::gfx
{
	class Font
	{
	public:
		ENUM_STRUCT(Modifiers, short,
			None = 0,
			Regular = BIT_SHIFT(0),
			Bold = BIT_SHIFT(1),
			Italic = BIT_SHIFT(2),
			Light = BIT_SHIFT(3),
			Thin = BIT_SHIFT(4),
			Medium = BIT_SHIFT(5),
			Semi = BIT_SHIFT(6),
			Extra = BIT_SHIFT(7),
			Black = BIT_SHIFT(8)
		);

	public:
		Font();
		Font(const std::filesystem::path& filepath, float fontSize);
		~Font();

		[[nodiscard]] float GetSize() const;

		[[nodiscard]] float GetScale() const;

		[[nodiscard]] std::string_view GetName() const;

		void Load(const std::filesystem::path& filepath, float fontSize);

		[[nodiscard]] std::vector<Modifiers> GetModifiers() const;
		[[nodiscard]] Modifiers::Underlying_t GetModifiersMask() const;

		bool Empty() const noexcept;

		operator ImFont* () const
		{
			return impl;
		}

		friend class FontLibrary;

	private:
		void DeduceModifiers(const std::filesystem::path& path);

	private:
		std::string name;
		Modifiers::Underlying_t modifiers;
		float size;
		ImFont* impl;
	};

}
