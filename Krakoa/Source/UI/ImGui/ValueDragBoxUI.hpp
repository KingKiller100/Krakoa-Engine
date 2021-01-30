#pragma once
#include "../UITypes.hpp"

namespace krakoa::ui
{
	const char* GetPrintfToken(size_t hashcode);
	
	template<typename T>
	bool DrawDragValue(const std::string_view& label, T& value, float increment, T v_min, T v_max
		, const char* format, const UICallBack& instruction = nullptr);

	template<typename T>
	bool DrawDragValue(const std::string_view& label, T& value, float increment, T v_min, T v_max, const UICallBack& instruction = nullptr)
	{
		return DrawDragValue<T>(label, value, increment, v_min, v_max, GetPrintfToken(typeid(T).hash_code()), instruction);
	}
	
	template<typename T>
	bool DrawDragValue(const std::string_view& label, T& value, float increment, const UICallBack& instruction = nullptr)
	{
		return DrawDragValue(label, value, increment, T(0), T(0), instruction);
	}
}
