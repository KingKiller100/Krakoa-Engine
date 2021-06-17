#include "Precompile.hpp"
#include "ExceptionHandler.hpp"

#include "../Util/Fmt.hpp"

#include <stdexcept>

namespace krakoa::errors
{
	namespace 
	{
		template<typename T>
		std::exception_ptr GetNested(T&& e)
		{
			try
			{
				const auto& nest = dynamic_cast<const std::nested_exception&>(e);
				return nest.nested_ptr();
			}
			catch (const std::bad_cast&)
			{
				return nullptr;
			}
		}

		std::string GetFullDescription(std::exception_ptr ePtr)
		{
			// Unresolvable exception
			if (ePtr == nullptr)
				throw std::bad_exception{};

			std::string desc;
			size_t nestIndex = 0;

			do
			{
				desc += klib::ToString("\n[{0}] ", nestIndex);
				++nestIndex;
				
				try
				{
					std::exception_ptr currentPtr;
					std::swap(ePtr, currentPtr);
					std::rethrow_exception(currentPtr);
				}
				catch (const std::exception& e)
				{
					desc += util::Fmt("{0}", e.what());
					ePtr = GetNested(e);
				}
				catch (const std::string& e) { desc += util::Fmt("{0}", e); }
				catch (const char* e) { desc += util::Fmt("{0}", e); }
				catch (...) { desc += "Unknown exception"; }
				
			} while (ePtr != nullptr);

			return desc;
		}
	}
	
	std::string UnwrapNestedExceptions(std::exception_ptr ePtr)
	{
		return GetFullDescription(ePtr);
	}
}
