#pragma once

#include "../../Core/EngineCore.h"
#include "../Debug Helper/kAssert.h"

#include <limits>
#include <string>
#include <type_traits>
#include <xutility>

namespace util
{
	namespace kString_View
	{
		template<class CharType, class Traits>
		class Template_String_View
		{
		public:
			using Type			= CharType;
			using Ptr			= CharType*;
			using Ref			= CharType&;
			using Const_Ptr		= const CharType*;
			using Const_Ref		= const CharType&;
			using Size			= size_t;
			
			static constexpr auto npos{ static_cast<Size>(-1) };

			static_assert(std::is_same_v<CharType, typename Traits::char_type>,
				"Type entered into string_view class does not match a char type object (char, wchar_t, char32_t, etc)!");
			
			constexpr Template_String_View() noexcept
				: string(nullptr),
				length(0)
			{}

			constexpr Template_String_View( const Template_String_View& ) noexcept = default;
			
			constexpr explicit Template_String_View(const Const_Ptr stringLiteral, const Size size, const Size offset = 0) noexcept
				: string(stringLiteral),
				length(size)
			{
				CheckOffsetWithinLength(offset, "Offset greater than length");
				string = string + offset;
				length -= offset;
			}

			constexpr Template_String_View(const Const_Ptr stringLiteral) noexcept
				: string(stringLiteral)
			{
				length = GetStrLength(string);
			}

			constexpr Template_String_View(const std::basic_string<CharType>& str) noexcept
				: string(str.c_str()),
				length(str.size())
			{			}
			
			constexpr Template_String_View& operator=( const Template_String_View& other) noexcept
			{
				this->string = other.string;
				this->length = other.length;
				return *this;
			}

			constexpr Template_String_View& operator=(const std::basic_string<CharType>& other) noexcept
			{
				this->string = other.c_str();
				this->length = other.size();
				return *this;
			}

			// Operators
			constexpr bool operator==(const CharType item) const
			{
				return Length() == 1 && *Data() == item;
			}
			
			constexpr bool operator==(const Template_String_View& other) const
			{
				return Length() == other.length && other.Data() == string;
			}

			constexpr bool operator==(Const_Ptr str) const
			{
				const Template_String_View temp(str);
				return Length() == temp.Length() && temp.Data() == string;
			}

			constexpr operator Const_Ptr() const
			{
				return string;
			}
			
			USE_RESULT constexpr Const_Ref operator[](const Size index) const noexcept
			{
				return at(index);
			}

			USE_RESULT constexpr Const_Ref at(const Size index) const
			{
				CheckOffsetWithinLength(index,
					"Index must be within 0 and length of string");

				return string[index];				
			}

			USE_RESULT constexpr Const_Ptr Data() const
			{
				return string;
			}
			
			USE_RESULT constexpr Size Length() const
			{
				return length;
			}

			USE_RESULT constexpr bool Empty() const
			{
				return length == 0;
			}

			constexpr void Clear()
			{
				string = nullptr;
				length = 0;
			}
					   
			USE_RESULT constexpr Const_Ref Front() const
			{
				kAssert(string != nullptr, "string is null");
				return string[0];
			}

			USE_RESULT constexpr Const_Ref Back() const
			{
				kAssert(string != nullptr, "string is null");
				return length < 1 ? string[0] : string[length - 1];
			}

			constexpr bool StartsWith(const CharType item) noexcept
			{
				return !Empty() && Front() == item;
			}

			constexpr bool StartsWith(const Template_String_View& other)
			{
				if (other.length > length  || Empty())
					return false;

				auto currentChar = string;
				auto otherChar = other.string;

				while (*otherChar != CharType('\0'))
				{
					if (*currentChar != *otherChar)
						return false;

					++currentChar;
					++otherChar;
				}

				return true;
			}

			constexpr bool StartsWith(Const_Ptr const literal) 
			{
				return StartsWith(Template_String_View(literal));
			}
			
			constexpr bool EndsWith(const Type item) noexcept
			{
				return !Empty() && Back() == item;
			}

			constexpr bool EndsWith(const Template_String_View& other)
			{
				if (other.length > length)
					return false;

				auto lengthDiff = length - other.length;
				
				auto currentChar = string + lengthDiff;
				auto otherChar = other.string;

				while (*otherChar != CharType('\0'))
				{
					if (*currentChar != *otherChar)
						return false;

					++currentChar;
					++otherChar;
				}

				return true;
			}

			constexpr bool EndsWith(Const_Ptr const literal)
			{
				return EndsWith(Template_String_View(literal));
			}

			
			constexpr void Swap(Template_String_View& other) noexcept
			{
				const auto temp = other;
				other = *this;
				*this = temp;
			}

			USE_RESULT constexpr Const_Ptr SubData(const Size amount, const Size offset = 0) const
			{
				CheckOffsetWithinLength(amount + offset,
					"Amount and offset provided give an index greater than the length of the string");

				return Template_String_View( string + offset, amount ).Data();
			}

			USE_RESULT constexpr Template_String_View Substr(const Size amount, const Size offset = 0) const
			{
				CheckOffsetWithinLength(amount + offset, 
					"Amount and offset provided give an index greater than the length of the string");

				return Template_String_View(string + offset, amount);
			}

			constexpr Size FirstInstanceOf(const Type item, const Size offset = 0, Size searchLimit = (std::numeric_limits<Size>::max)())
			{
				CheckOffsetWithinLength(offset, 
					"Offset is greater than the length of string");
				
				auto currentChar = string + offset;
				Size count(0);
				while (*currentChar != CharType('\0') 
					&& searchLimit > 0)
				{
					if (*currentChar == item)
						return count;
					
					++count;
					--searchLimit;
					currentChar += ReturnSizeOfCharT();
				}
				
				return npos;
			}

			constexpr Size FirstInstanceOf(Const_Ptr str, const Size offset = 0)
			{
				CheckOffsetWithinLength(offset, 
					"Offset is greater than the length of string");

				auto pos = FirstInstanceOf(str[0], offset);
				auto strLength = GetStrLength(str);
				
				if (pos == npos)
					return npos;
				
				auto viewStr = string + pos;
				Size idx(0);
				
				while (*viewStr != CharType('\0'))
				{
					if (*viewStr == str[idx])
					{
						++viewStr;
						++pos;
						++idx;
						continue;
					}

					if (str[idx] == CharType('\0'))
						return (pos - strLength);

					idx = 0;
					pos = FirstInstanceOf(str[0], pos);
					viewStr = string + pos;
				}
				
				return npos;
			}
			
			constexpr Size FirstInstanceOf(const Template_String_View& other, const Size offset = 0)
			{
				return FirstInstanceOf(other.string, offset);
			}

			constexpr Size FirstInstanceOfNot(const Type item, const Size offset = 0)
			{
				CheckOffsetWithinLength(offset,
					"Offset is greater than the length of string");

				auto currentChar = string + offset;
				Size count(0);
				while (*currentChar != CharType('\0'))
				{
					if (*currentChar != item)
						return count;

					++count;
					currentChar += ReturnSizeOfCharT();
				}

				return npos;
			}

			constexpr Size LastInstanceOf(const CharType item, const Size offset = 0)
			{
				CheckOffsetWithinLength(offset, 
					"Offset greater than length of this string");

				auto currentChar = string + length - offset;
				Size pos(length - 1);

				while (currentChar != string)
				{
					if (*currentChar == item)
						return pos;

					--pos;
					currentChar -= ReturnSizeOfCharT();
				}
				return npos;
			}

			constexpr Size LastInstanceOfNot(const CharType item, const Size offset = 0) noexcept
			{
				auto currentChar = string + length - offset;
				Size pos(length - 1);

				while (currentChar != string)
				{
					if (*currentChar != item)
						return pos;

					--pos;
					currentChar -= ReturnSizeOfCharT();
				}
				return npos;
			}

			constexpr Size LastInstanceOf(Const_Ptr str, const Size offset = 0) noexcept
			{
				CheckOffsetWithinLength(offset, "Offset greater than length of this string");

				auto pos = LastInstanceOf(str[0], offset);
				auto strLength = GetStrLength(str);

				if (pos == npos)
					return npos;

				auto viewStr = string + length - pos;
				Size strIdx(0);

				while (viewStr != string)
				{
					if (*viewStr == str[strIdx])
					{
						++viewStr;
						++pos;
						++strIdx;
						continue;
					}

					if (str[strIdx] == CharType('\0'))
						return (pos - strLength);

					strIdx = 0;
					pos = LastInstanceOf(str[0], pos);
					viewStr = string + length - pos;
				}

				return npos;
			}

			void RemovePrefix(const Size offset)
			{
				CheckOffsetWithinLength(offset, 
					"Offset provided is outside the range of the string");
				
				string += offset;
				length -= offset;
			}

			Const_Ptr RemoveSuffix(const Size offset)
			{
				CheckOffsetWithinLength(offset, 
					"Offset provided is outside the range of the string");
				
				length -= offset;
				return string + offset;
			}

			USE_RESULT constexpr bool IsEqual(const Template_String_View& other) const noexcept
			{
				if (length != other.length)
					return false;
				
				auto thisCurrentChar = string;
				auto otherCurrentChar = other.string;
								
				while (*thisCurrentChar == *otherCurrentChar )
				{
					++thisCurrentChar;
					++otherCurrentChar;
					if (*thisCurrentChar == CharType('\0')
						&& *otherCurrentChar == CharType('\0'))
						return true;
				}
				
				return false;
			}

			constexpr Size Find(const CharType item, Size offset) const
			{
				return FirstInstanceOf(item, offset);
			}

			constexpr Size Find(Const_Ptr item, Size offset) const
			{
				return FirstInstanceOf(item, offset);
			}

			constexpr Size rFind(const CharType item, Size offset) const
			{
				return LastInstanceOf(item, offset);
			}

			constexpr Size rFind(Const_Ptr item, Size offset) const
			{
				return LastInstanceOf(item, offset);
			}


			
		private:
			constexpr void CheckOffsetWithinLength(const Size offset, const char* msg) const
			{
				if (offset > length)
					std::_Xout_of_range(msg);
			}

			USE_RESULT constexpr Size ReturnSizeOfCharT() const
			{
				return sizeof(char);
			}

			Size GetStrLength(Const_Ptr str)
			{
				Size size = 0;
				auto currentChar = str;
				
				while (*currentChar != CharType('\0'))
				{
					++size;
					currentChar += ReturnSizeOfCharT();
				}

				return size;
			}
			
		private:
			Const_Ptr string;
			size_t length;
		};

		// Type aliasing for standard forms of Template_String_View
		
#ifdef __cpp_lib_char8_t
		using u8StringView = Template_String_View<char8_t, std::char_traits<char8_t>>;
		
		USE_RESULT inline u8StringView operator"" _sv(u8StringView::Const_Ptr str, size_t length) noexcept
		{
			return u8StringView(str, length);
		}
#endif // __cpp_lib_char8_t
		
		using StringView = Template_String_View<char, std::char_traits<char>>;
		using u16StringView = Template_String_View<char16_t, std::char_traits<char16_t>>;
		using u32StringView = Template_String_View<char32_t, std::char_traits<char32_t>>;
		using wStringView = Template_String_View<wchar_t, std::char_traits<wchar_t>>;


		USE_RESULT constexpr StringView operator"" _sv(StringView::Const_Ptr str, size_t length) noexcept
		{
			return StringView(str, length);
		}
		
		USE_RESULT constexpr u16StringView operator"" _sv(u16StringView::Const_Ptr str, size_t length) noexcept
		{
			return u16StringView(str, length);
		}
		
		USE_RESULT constexpr u32StringView operator"" _sv(u32StringView::Const_Ptr str, size_t length) noexcept
		{
			return u32StringView(str, length);
		}
		
		USE_RESULT constexpr wStringView operator"" _sv(wStringView::Const_Ptr str, size_t length) noexcept
		{
			return wStringView(str, length);
		}
		
		
	}
}
