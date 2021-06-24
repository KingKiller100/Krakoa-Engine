#include "Precompile.hpp"

#include "FileDialogFilter.hpp"

#include "../../../Util/Fmt.hpp"

namespace krakoa::os
{
	FileDialogFilter::FileDialogFilter()
		: filters()
		, buffer()
		, currentSize(0)
		, bufSize(1ull << 5)
	{
		RecreateBuffer();
	}

	void FileDialogFilter::AddFilter(const Filter& filter)
	{
		filters.emplace_back(filter);
	}

	void FileDialogFilter::AddFilter(const char* name, const char* extension)
	{
		AddFilter({ name, extension });
	}

	void FileDialogFilter::RemoveFilter(const char* name)
	{
		const auto iter = std::find_if(filters.begin(), filters.end(),
			[name](const decltype(filters)::value_type& filter)
		{
			return filter.name == name;
		});

		filters.erase(iter);
	}

	const char* FileDialogFilter::GetFilter() const noexcept
	{
		return buffer.get();
	}

	void FileDialogFilter::FormatFilter()
	{
		for (auto&& filter : filters)
		{
			const auto& filterName = filter.name;
			const auto& filterExt = filter.extension;

			const auto extensionStr = util::Fmt("*.{0}", filterExt);
			const auto label = util::Fmt("{0} ({1})", filterName, extensionStr);

			AppendFilter(label);
			AppendFilter(extensionStr);
		}
	}

	void FileDialogFilter::AppendFilter(const std::string& text)
	{
		const auto toAddSize = text.size();
		ExpandFilterData(toAddSize);
		std::strcpy(buffer.get() + currentSize, text.data());
		currentSize += toAddSize;
		buffer[currentSize] = '\0';
		++currentSize;
	}

	void FileDialogFilter::ExpandFilterData(size_t toAdd)
	{
		const auto nextPosition = currentSize + toAdd + 1;
		if (nextPosition >= bufSize)
		{
			const auto oldSize = bufSize;
			bufSize += static_cast<size_t>(0.5 * bufSize);

			const auto tempBuf = Solo_Ptr<char[]>(new char[bufSize]{});
			std::memcpy(tempBuf.get(), buffer.get(), sizeof(char) * oldSize);
			RecreateBuffer();
			std::memcpy(buffer.get(), tempBuf.get(), sizeof(char) * oldSize);
		}
	}

	void FileDialogFilter::RecreateBuffer()
	{
		buffer.reset(new char[bufSize]{});
	}
}

