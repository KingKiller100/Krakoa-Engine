#pragma once

#include "../../../Core/PointerTypes.hpp"
#include "../../../Core/EngineMacros.hpp"

#include <string>
#include <vector>

namespace krakoa::os
{
	struct Filter
	{
		std::string name;
		std::string extension;
	};
	
	class FileDialogFilter
	{
	public:
		FileDialogFilter();

		~FileDialogFilter()
			= default;

		void AddFilter(const Filter& filter);
		void AddFilter(const char* name, const char* extension);
		void RemoveFilter(const char* name);
		void FormatFilter();

		USE_RESULT const char* GetFilter() const noexcept;

	private:
		void CreateBuffer();
		void ExpandFilterData(size_t toAdd);
		void AppendFilter(const std::string& text);

	private:
		std::vector<Filter> filters;
		Solo_Ptr<char[]> buffer;
		std::ptrdiff_t currentSize;
		size_t bufSize;
	};
}

