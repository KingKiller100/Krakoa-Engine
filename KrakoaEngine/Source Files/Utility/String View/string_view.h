#pragma once

namespace util
{
	namespace string_view
	{
		template<class CharT, class Trait>
		class StringView
		{
			StringView()
			: string(nullptr),
			size(0)
			{
				
			}

			StringView(const CharT* stringLiteral, const size_t size)
			: string(stringLiteral),
			size(size)
			{
				
			}

		private:
			CharT* string;
			size_t size;
		};
	}
}