#pragma once

#include <string>

namespace krakoa::os::library
{
	template <typename Import>
	struct ImportWrapper
	{
	public:
		using Import_t = Import;

		constexpr ImportWrapper( const std::string_view& name, void* ip )
			: id( name )
			, imp( static_cast<Import*>( ip ) )
		{ }

		USE_RESULT constexpr std::string_view GetID() const noexcept
		{
			return id;
		}

		constexpr Import* GetImport() const
		{
			return imp;
		}

		template <typename ...Args>
		constexpr decltype(auto) operator()( Args&& ...args ) const
		{
			return imp( std::forward<Args>( args )... );
		}

		USE_RESULT constexpr operator bool() const
		{
			return imp;
		}

		USE_RESULT constexpr bool operator==( std::nullptr_t ) const
		{
			return imp == nullptr;
		}

		USE_RESULT constexpr bool operator!=( std::nullptr_t ) const
		{
			return imp != nullptr;
		}

		USE_RESULT constexpr bool operator==( const ImportWrapper& other ) const
		{
			return imp == other.imp;
		}

		USE_RESULT constexpr bool operator!=( const ImportWrapper& other ) const
		{
			return imp != other.imp;
		}

	private:
		std::string id;
		Import* imp;
	};

	class iOSLibrary
	{
	public:
		template <typename T>
		using ImportPtr_t = std::remove_pointer_t<T>;

	public:
		virtual ~iOSLibrary() = default;

		[[nodiscard]] virtual std::string_view GetName() const = 0;
		[[nodiscard]] virtual bool IsLoaded() const = 0;
		[[nodiscard]] virtual std::vector<std::string> GetImportsID() const = 0;

		template <typename T>
		auto Import( const std::string_view& id )
		{
			return ImportWrapper<ImportPtr_t<T>>( id, ImportImpl( id.data() ) );
		}


	protected:
		virtual void* ImportImpl( const char* id ) = 0;
	};
}
