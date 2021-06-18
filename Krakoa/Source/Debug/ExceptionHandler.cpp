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

			std::string output;
			std::vector<std::string> descriptions;

			do
			{
				try
				{
					std::exception_ptr currentPtr;
					std::swap(ePtr, currentPtr);
					std::rethrow_exception(currentPtr);
				}
				catch (const std::exception& e)
				{
					descriptions.emplace_back(util::Fmt("{0}", e.what()));
					ePtr = GetNested(e);
				}
				catch (const std::string& e) { descriptions.emplace_back(util::Fmt("{0}", e)); }
				catch (const char* e) { descriptions.emplace_back(util::Fmt("{0}", e)); }
				catch (...) { descriptions.emplace_back("Unknown exception"); }

			} while (ePtr != nullptr);

			if (!descriptions.empty())
			{
				if (descriptions.size() > 1)
				{
					for (size_t i = 0; i < descriptions.size(); ++i)
					{
						output += util::Fmt("\n[{0}] {1}", i, descriptions[i]);
					}
				}
				else
				{
					output = descriptions.front();
				}
			}

			return output;
		}
	}

	std::string UnwrapNestedExceptions(std::exception_ptr ePtr)
	{
		return GetFullDescription(ePtr);
	}
}
