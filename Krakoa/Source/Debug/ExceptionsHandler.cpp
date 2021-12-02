#include "Precompile.hpp"

#include "ExceptionsHandler.hpp"
#include "../Util/Fmt.hpp"
#include <sstream>
#include <exception>

namespace krakoa::debug
{
	using namespace util;

	namespace
	{
		template <typename Exception>
		std::exception_ptr GetNestedException( const Exception& e )
		{
			try
			{
				auto& nested = dynamic_cast<const std::nested_exception&>( e );
				return nested.nested_ptr();
			}
			catch ( const std::bad_cast& )
			{
				return nullptr;
			}
		}

		std::string ResolveNestedExceptions( std::exception_ptr ptr )
		{
			if ( ptr == nullptr )
				throw std::bad_exception{};

			std::stringstream fullOutput;

			do
			{
				try
				{
					std::exception_ptr prev;
					std::swap( ptr, prev );
					std::rethrow_exception( prev );
				}
				catch ( const std::exception& e )
				{
					fullOutput << "Exception: " << e.what() << "\n";
					ptr = GetNestedException( e );
				}
				catch ( const std::string_view& e )
				{
					fullOutput << "Error Message: " << e << "\n";
				}
				catch ( const std::string& e )
				{
					fullOutput << "Error Message: " << e << "\n";
				}
				catch ( const char* e )
				{
					fullOutput << "Error Message: " << e << "\n";
				}
				catch ( ... )
				{
					fullOutput << "Unknown exception: " << "\n";
				}
			} while ( ptr != nullptr );


			return fullOutput.str();
		}
	}

	std::string UnwrapNestedExceptions( std::exception_ptr ePtr )
	{
		return ResolveNestedExceptions( ePtr );
	}
}
