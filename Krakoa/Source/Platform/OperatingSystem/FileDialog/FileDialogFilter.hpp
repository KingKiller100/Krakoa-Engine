#pragma once

#include "../../../Util/Fmt.hpp"
#include "../../../Core/PointerTypes.hpp"
#include "../../../Core/EngineMacros.hpp"

#include <string>
#include <vector>

namespace krakoa::os
{
	template<typename T>
	struct BasicFilter
	{
		std::basic_string<T> name;
		std::basic_string<T> extension;
	};

	template<typename T>
	class BasicFileDialogFilter
	{
	public:
		using CharT = T;

	public:
		BasicFileDialogFilter()
			: filters()
			, buffer()
			, currentSize(0)
			, bufSize(1ull << 5)
		{
			RecreateBuffer();
		}

		~BasicFileDialogFilter()
			= default;

		void AddFilter(const BasicFilter<T>& filter)
		{
			filters.emplace_back(filter);
		}
		void AddFilter(const T* name, const T* extension)
		{
			AddFilter({ name, extension });
		}

		void RemoveFilter(const T* name)
		{
			const auto iter = std::find_if(filters.begin(), filters.end(),
				[name](const typename decltype(filters)::value_type& filter)
			{
				return filter.name == name;
			});

			filters.erase(iter);
		}

		void FormatFilter()
		{
			for (auto&& filter : filters)
			{
				const auto& filterName = filter.name;
				const auto& filterExt = filter.extension;

				const auto extensionStr = util::Fmt<T>(klib::Convert<T>("*.{0}"), filterExt);
				const auto label = util::Fmt<T>(klib::Convert<T>("{0} ({1})"), filterName, extensionStr);

				AppendFilter(label);
				AppendFilter(extensionStr);
			}
		}

		USE_RESULT const T* GetFilter() const noexcept
		{
			return buffer.get();
		}

	private:
		void AppendFilter(const std::basic_string<T>& text)
		{
			const auto toAddSize = text.size();
			ExpandFilterData(toAddSize);
			std::strcpy(buffer.get() + currentSize, text.data());
			currentSize += toAddSize;
			buffer[currentSize] = klib::type_trait::g_NullTerminator<T>;
			++currentSize;
		}
		
		void ExpandFilterData(size_t toAdd)
		{
			const auto nextPosition = currentSize + toAdd + 1;
			if (nextPosition >= bufSize)
			{
				const auto oldSize = bufSize;
				bufSize += static_cast<size_t>(0.5 * bufSize);

				const auto tempBuf = Solo_Ptr<char[]>(new T[bufSize] {});
				std::memcpy(tempBuf.get(), buffer.get(), sizeof(T) * oldSize);
				RecreateBuffer();
				std::memcpy(buffer.get(), tempBuf.get(), sizeof(T) * oldSize);
			}
		}

		void RecreateBuffer()
		{
			buffer.reset(new T[bufSize] {});
		}

	private:
		std::vector<BasicFilter<T>> filters;
		Solo_Ptr<T[]> buffer;
		std::ptrdiff_t currentSize;
		size_t bufSize;
	};
	
	using Filter = BasicFilter<char>;
	using wFilter = BasicFilter<wchar_t>;
	
	using FileDialogFilter = BasicFileDialogFilter<char>;
	using wFileDialogFilter = BasicFileDialogFilter<wchar_t>;
}

