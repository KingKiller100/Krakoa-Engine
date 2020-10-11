#pragma once

#include <vector>
#include <string>
#include <unordered_map>

namespace util
{

	class iStringifier
	{
	public:
		virtual ~iStringifier() = default;
	};
	
	template<typename  T>
	class StringEntry : public iStringifier
	{
	public:
		StringEntry() = default;
		~StringEntry() override = default;
		
		void Add(const T& e, const std::string& str)
		{
			if (Exists(e))
				return;
			
			KRK_PROFILE_FUNCTION();
			table.insert(std::make_pair(e, str));
		}

		bool Exists(const T& e)
		{
			return table.find(e) != table.end();
		}

		
		std::string ToString(const T& e)
		{
			KRK_PROFILE_FUNCTION();
			if (const auto it = table.find(e); it != table.end())
			{
				return it->second;
			}

			throw std::exception("Unknown value for this type");
		}
		
	private:
		std::unordered_map<T, std::string> table;
	};

	class Stringifier
	{
	public:
		template<typename E>
		static void Add(const E& e, const std::string& str)
		{
			KRK_PROFILE_FUNCTION();
			
			if (!HasEnum<E>())
				Create<E>();

			Add_Impl(e, str);
		}
		
		template<typename E>
		static std::string ToString(const E& e)
		{
			KRK_PROFILE_FUNCTION();

			if (!HasEnum<E>())
			{
				Create<E>();
			}
			
			Add_Impl(e, typeid(e).name());
			StringEntry<E>* ets = Convert<E>();
			return ets->ToString(e);
		}
		
	private:
		template<typename E>
		static void Create()
		{
			KRK_PROFILE_FUNCTION();
			types.emplace_back(new StringEntry<E>());
		}
		
		template<typename E>
		static void Add_Impl(const E& e, const std::string& str)
		{
			KRK_PROFILE_FUNCTION();
			StringEntry<E>* ets = Convert<E>();
			ets->Add(e, str);
		}

		template<typename E>
		static bool HasEnum()
		{
			KRK_PROFILE_FUNCTION();
			return (types.size() > GetUniqueID<E>());
		}
		
		static size_t MakeUniqueID()
		{
			KRK_PROFILE_FUNCTION();
			static size_t id = 0;
			return id++;
		}

		template<typename E>
		static size_t GetUniqueID()
		{
			KRK_PROFILE_FUNCTION();
			static auto id = MakeUniqueID();
			return id;
		}

		template<typename E>
		static StringEntry<E>* Convert()
		{
			KRK_PROFILE_FUNCTION();
			return dynamic_cast<StringEntry<E>*>(types[GetUniqueID<E>()]);
		}
		
	private:
		inline static std::vector<iStringifier*> types{};
	};
}

	

#define STRINGIFIER(type) util::Stringifier::ToString(type)