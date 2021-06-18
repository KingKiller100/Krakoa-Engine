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

			std::string fullOutput;
			std::vector<std::string> messages;

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
					messages.emplace_back(e.what());
					ePtr = GetNested(e);
				}
				catch (const std::string& e) { messages.emplace_back(e); }
				catch (const char* e) { messages.emplace_back(e); }
				catch (...) { messages.emplace_back("Unknown Exception"); }

			} while (ePtr != nullptr);

			if (!messages.empty())
			{
				if (messages.size() > 1)
				{
					for (size_t i = 0; i < messages.size(); ++i)
					{
						fullOutput += util::Fmt("\n[{0}] {1}", i, messages[i]);
					}
				}
				else
				{
					fullOutput = messages.front();
				}
			}

			return fullOutput;
		}
	}

	std::string UnwrapNestedExceptions(std::exception_ptr ePtr)
	{
		return GetFullDescription(ePtr);
	}
}
