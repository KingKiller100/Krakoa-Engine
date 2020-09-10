#pragma once

#include <array>
#include <string>
#include <memory>
#include <unordered_set>

namespace util
{

	class iEnumToString
	{
	public:
		virtual ~iEnumToString() = default;
	};
	
	template<typename  E>
	class EnumToString : public iEnumToString
	{
	public:
		EnumToString() = default;
		~EnumToString() override = default;
		
		void Add(const E& e, const std::string& str)
		{
			table.insert(std::make_pair(e, str));
		}
		
		std::string ToString(const E& e)
		{
			if (const auto it = table.find(e); it != table.end())
			{
				return it->second;
			}

			throw std::exception("Unknown value for this enum type");
		}
		
	private:
		std::unordered_map<E, std::string> table;
	};

	class EnumToStringManager
	{
	public:
		template<typename E>
		static void Create()
		{
			enums[GetUniqueID<E>()] = new EnumToString<E>();
		}

		template<typename E>
		static void AddTo(const E& e, const std::string& str)
		{
			EnumToString<E>* ets = Convert<E>();
			ets->Add(e, str);
		}
		
		template<typename E>
		static std::string ToString(const E& e)
		{
			EnumToString<E>* ets = Convert<E>();
			return ets->ToString(e);
		}
		
	protected:
		static size_t MakeUniqueID()
		{
			static size_t id = 0;
			return id++;
		}

		template<typename E>
		static size_t GetUniqueID()
		{
			static auto id = MakeUniqueID();
			return id;
		}

		template<typename E>
		static EnumToString<E>* Convert()
		{
			return dynamic_cast<EnumToString<E>*>(enums[GetUniqueID<E>()]);
		}
		
	private:
		inline static std::array<iEnumToString*, 1024> enums{};
	};

}
