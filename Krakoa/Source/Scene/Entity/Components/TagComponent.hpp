#pragma once

#include <string>

namespace krakoa::scene::ecs::components
{
	class TagComponent
	{
	public:
		TagComponent(const std::string_view& tag);
		TagComponent(const char* tag);
		TagComponent(const std::string& tag);
		~TagComponent() noexcept;

		[[nodiscard]] std::string_view GetTag() const;
		[[nodiscard]] const char* GetData() const;
		[[nodiscard]] char* GetData();
		void SetTag(const std::string_view& tag);

		void Reserve(size_t size);
		void Resize(size_t size);

	private:
		std::string tagName;
	};

	
}
