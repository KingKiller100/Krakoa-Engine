#pragma once

#include <string>

namespace krakoa::components
{
	class TagComponent
	{
	public:
		TagComponent(const std::string_view& tag);
		TagComponent(const char* tag);
		TagComponent(const std::string& tag);
		~TagComponent() noexcept;

		[[nodiscard]] const std::string& GetTag() const;
		void SetTag(const std::string_view& tag);

		[[nodiscard]] const char* GetType() const noexcept;
		
	private:
		std::string tagName;
	};
}
