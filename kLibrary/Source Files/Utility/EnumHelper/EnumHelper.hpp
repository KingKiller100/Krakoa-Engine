#pragma once

#include "../../Maths/kAlgorithms.hpp"
#include "../../Type Traits/String.hpp"

#include <array>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <string_view>

#define MAP(macro, ...) \
    IDENTITY( \
        APPLY(CHOOSE_MAP_START, COUNT(__VA_ARGS__)) \
            (macro, __VA_ARGS__))

#define CHOOSE_MAP_START(count) MAP ## count

#define APPLY(macro, ...) IDENTITY(macro(__VA_ARGS__))

#define IDENTITY(x) x

#define MAP1(m, x)      m(x)
#define MAP2(m, x, ...) m(x) IDENTITY(MAP1(m, __VA_ARGS__))
#define MAP3(m, x, ...) m(x) IDENTITY(MAP2(m, __VA_ARGS__))
#define MAP4(m, x, ...) m(x) IDENTITY(MAP3(m, __VA_ARGS__))
#define MAP5(m, x, ...) m(x) IDENTITY(MAP4(m, __VA_ARGS__))
#define MAP6(m, x, ...) m(x) IDENTITY(MAP5(m, __VA_ARGS__))
#define MAP7(m, x, ...) m(x) IDENTITY(MAP6(m, __VA_ARGS__))
#define MAP8(m, x, ...) m(x) IDENTITY(MAP7(m, __VA_ARGS__))
#define MAP9(m, x, ...) m(x) IDENTITY(MAP8(m, __VA_ARGS__))
#define MAP10(m, x, ...) m(x) IDENTITY(MAP9(m, __VA_ARGS__))
#define MAP11(m, x, ...) m(x) IDENTITY(MAP10(m, __VA_ARGS__))
#define MAP12(m, x, ...) m(x) IDENTITY(MAP11(m, __VA_ARGS__))
#define MAP13(m, x, ...) m(x) IDENTITY(MAP12(m, __VA_ARGS__))
#define MAP14(m, x, ...) m(x) IDENTITY(MAP13(m, __VA_ARGS__))

#define EVALUATE_COUNT(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, count, ...) \
    count

#define COUNT(...) \
    IDENTITY(EVALUATE_COUNT(__VA_ARGS__, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1))


// Stringizes each argument.
#define STRINGIZE_SINGLE(e) #e,
#define STRINGIZE(...) IDENTITY(MAP(STRINGIZE_SINGLE, __VA_ARGS__))

namespace secret::helper
{

	// The type "T" mentioned above that drops assignment operations.
	template <typename U>
	struct ignore_assign {
		constexpr explicit ignore_assign(U value) : _value(value) { }
		constexpr operator U() const { return _value; }

		constexpr const ignore_assign& operator =(int dummy) const
		{
			return *this;
		}

		U   _value;
	};

	// Some helpers needed for _from_string.
	constexpr char    terminators[] = " =\t\r\n";

	// The size of terminators includes the implicit '\0'.
	constexpr bool is_terminator(char c, size_t index = 0)
	{
		return
			index >= sizeof(terminators) ? false :
			c == terminators[index] ? true :
			is_terminator(c, index + 1);
	}

	constexpr bool matches_untrimmed(const std::string_view& untrimmed, const std::string_view& s,
		size_t index = 0)
	{
		return
			is_terminator(untrimmed[index]) ? s[index] == '\0' :
			s[index] != untrimmed[index] ? false :
			matches_untrimmed(untrimmed, s, index + 1);
	}
}

// Prepends "(ignore_assign<_underlying>)" to each argument.
#define IGNORE_ASSIGN_SINGLE(e) (secret::helper::ignore_assign<underlying_t>)e,
#define IGNORE_ASSIGN(...) \
    IDENTITY(MAP(IGNORE_ASSIGN_SINGLE, __VA_ARGS__))




//#define ENUM_CLASS(name, underlying, ...)
namespace kEnum
{
	using underlying_t = std::uint64_t;
	enum _enum : underlying_t { A = 5, B, C, D, E };

	namespace data
	{
		static constexpr auto size = IDENTITY(COUNT(A = 5, B, C, D, E));

		static constexpr std::array<underlying_t, size> values =
		{ IDENTITY(IGNORE_ASSIGN(A = 5, B, C, D, E)) };

		static constexpr std::array<std::string_view, size> raw_names =
		{ IDENTITY(STRINGIZE(A = 5, B, C, D, E)) };
	}

	void InitializeNames(char* the_names[data::size]);
	std::string_view TrimmedNames(const _enum input);

	constexpr underlying_t ToPrimitive(_enum input)
	{
		return static_cast<underlying_t>(input);
	}

	constexpr std::string_view ToString(_enum input)
	{
		const auto names = TrimmedNames(input);
		return names;
	}

	constexpr _enum FromString(const std::string_view& s, size_t index = 0)
	{
		using namespace secret::helper;

		if (!kmaths::InRange<size_t>(
			index,
			0,
			data::size)
			)
			std::_Xout_of_range("Invalid identifier");

		const auto matches = matches_untrimmed(data::raw_names[index], s);

		const auto ret = matches
			? static_cast<_enum>(data::values[index])
			: FromString(s, index + 1);

		return ret;
	}

	inline std::string_view TrimmedNames(const _enum input)
	{
		static char* the_names[data::size];
		static bool  initialized = false;

		if (!initialized)
		{
			InitializeNames(the_names);
			initialized = true;
		}

		size_t index = 0;
		while (index < data::size)
		{
			if (input == data::values[index])
				break;
			++index;
		}

		return the_names[index];
	}

	inline void InitializeNames(char* the_names[data::size])
	{
		using namespace secret::helper;
		for (auto i = 0; i < data::size; ++i)
		{
			const auto& raw_name = data::raw_names[i];
			auto& name = the_names[i];

			const auto length_til_terminator =
				std::strcspn(raw_name.data(),
					terminators);

			name = new char[length_til_terminator + 1];

			std::strncpy(name,
				raw_name.data(),
				length_til_terminator);

			name[length_til_terminator] = '\0';
		}
	}
}

