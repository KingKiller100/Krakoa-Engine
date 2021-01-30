#include "Precompile.hpp"
#include "ValueDragBoxUI.hpp"


#include <cinttypes>
#include <imgui.h>
#include <unordered_map>

namespace krakoa::ui
{
	
	const char* GetPrintfToken(size_t hashcode)
	{
		static const std::unordered_map<size_t, const char*> tokenMap = {
			{typeid(long double).hash_code(),   "%.6f"},
			{typeid(double).hash_code(),        "%.6f"},
			{typeid(float).hash_code(),         "%.3f"},
			{typeid(std::int8_t).hash_code(),   "%" PRId8},
			{typeid(std::int16_t).hash_code(),  "%" PRId16},
			{typeid(std::int32_t).hash_code(),  "%" PRId32},
			{typeid(long).hash_code(),          "%" PRId32},
			{typeid(std::int64_t).hash_code(),  "%" PRIu64},
			{typeid(std::uint8_t).hash_code(),  "%" PRIu8},
			{typeid(std::uint16_t).hash_code(), "%" PRIu16},
			{typeid(std::uint32_t).hash_code(), "%" PRIu32},
			{typeid(unsigned long).hash_code(), "%" PRIu32},
			{typeid(std::uint64_t).hash_code(), "%" PRIu64},
		};

		return tokenMap.at(hashcode);
	}

	template <>
	bool DrawDragValue(const std::string_view& label, std::int8_t& value, float increment, std::int8_t v_min
		, std::int8_t v_max, const char* format, const UICallBack& instruction)
	{
		if (ImGui::DragScalar(label.data(), ImGuiDataType_S8, &value, increment, &v_min, &v_max, format))
		{
			if (instruction)
				instruction();
			return true;
		}
		return false;
	}

	template <>
	bool DrawDragValue(const std::string_view& label, std::int16_t& value, float increment, std::int16_t v_min
		, std::int16_t v_max, const char* format, const UICallBack& instruction)
	{
		if (ImGui::DragScalar(label.data(), ImGuiDataType_S16, &value, increment, &v_min, &v_max, format))
		{
			if (instruction)
				instruction();
			return true;
		}
		return false;
	}

	template <>
	bool DrawDragValue(const std::string_view& label, std::int32_t& value, float increment, std::int32_t v_min
		, std::int32_t v_max, const char* format, const UICallBack& instruction)
	{
		if (ImGui::DragScalar(label.data(), ImGuiDataType_S32, &value, increment, &v_min, &v_max, format))
		{
			if (instruction)
				instruction();
			return true;
		}
		return false;
	}
	
	template<>
	bool DrawDragValue(const std::string_view& label, std::int64_t& value, float increment, std::int64_t v_min, std::int64_t v_max
		, const char* format, const UICallBack& instruction)
	{
		if (ImGui::DragScalar(label.data(), ImGuiDataType_S64, &value, increment, &v_min, &v_max, format))
		{
			if (instruction)
				instruction();
			return true;
		}
		return false;
	}

	template<>
	bool DrawDragValue(const std::string_view& label, std::uint8_t& value, float increment, std::uint8_t v_min, std::uint8_t v_max
		, const char* format, const UICallBack& instruction)
	{
		if (ImGui::DragScalar(label.data(), ImGuiDataType_U8, &value, increment, &v_min, &v_max, format))
		{
			if (instruction)
				instruction();
			return true;
		}
		return false;
	}
	
	template<>
	bool DrawDragValue(const std::string_view& label, std::uint16_t& value, float increment, std::uint16_t v_min, std::uint16_t v_max
		, const char* format, const UICallBack& instruction)
	{
		if (ImGui::DragScalar(label.data(), ImGuiDataType_U16, &value, increment, &v_min, &v_max, format))
		{
			if (instruction)
				instruction();
			return true;
		}
		return false;
	}

	template<>
	bool DrawDragValue(const std::string_view& label, std::uint32_t& value, float increment, std::uint32_t v_min, std::uint32_t v_max
		, const char* format, const UICallBack& instruction)
	{
		if (ImGui::DragScalar(label.data(), ImGuiDataType_U32, &value, increment, &v_min, &v_max, format))
		{
			if (instruction)
				instruction();
			return true;
		}
		return false;
	}

	template<>
	bool DrawDragValue(const std::string_view& label, std::uint64_t& value, float increment, std::uint64_t v_min, std::uint64_t v_max
		, const char* format, const UICallBack& instruction)
	{
		if (ImGui::DragScalar(label.data(), ImGuiDataType_U64, &value, increment, &v_min, &v_max, format))
		{
			if (instruction)
				instruction();
			return true;
		}
		return false;
	}


	template<>
	bool DrawDragValue(const std::string_view& label, float& value, float increment, float v_min, float v_max
		, const char* format, const UICallBack& instruction)
	{
		if (ImGui::DragScalar(label.data(), ImGuiDataType_Float, &value, increment, &v_min, &v_max, format))
		{
			if (instruction)
				instruction();
			return true;
		}
		return false;
	}

	template<>
	bool DrawDragValue(const std::string_view& label, double& value, float increment, double v_min, double v_max
		, const char* format, const UICallBack& instruction)
	{
		if (ImGui::DragScalar(label.data(), ImGuiDataType_Double, &value, increment, &v_min, &v_max, format))
		{
			if (instruction)
				instruction();
			return true;
		}
		return false;
	}
}
