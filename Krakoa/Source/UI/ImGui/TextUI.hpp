#pragma once
#include "../UITypes.hpp"

#include "../Flags/InputTextFlags.hpp"

#include <Utility/String/kToString.hpp>

namespace krakoa::ui
{
	void DrawRawText(const std::string_view& text);
	
	template<typename T, typename ...Ts>
	void DrawRawText(const std::string_view& fmt, const T& arg, const Ts& ...args)
	{
		DrawRawText(klib::kString::ToString(fmt, arg, args...));
	}

	bool DrawInputTextBox(const std::string_view& label, char* buf, size_t buf_size, InputTextFlags::underlying_t flags,
		const UICallBack& cb = nullptr);
}
