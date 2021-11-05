#pragma once

namespace krakoa::os::library
{
	class iOSLibrary
	{
	public:
		template <typename T>
		using ImportPtr_t = std::remove_pointer_t<T>*;

	public:
		virtual ~iOSLibrary() = default;

		[[nodiscard]] virtual std::string_view GetName() const = 0;
		[[nodiscard]] virtual bool IsLoaded() const = 0;

		template <typename T>
		auto Import( const std::string_view& id )
		{
			return static_cast<ImportPtr_t<T>>( ImportImpl( id.data() ) );
		}

	protected:
		virtual void* ImportImpl( const char* id ) = 0;
	};
}
