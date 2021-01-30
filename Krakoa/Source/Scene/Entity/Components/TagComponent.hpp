#pragma once

#include <TypeTraits/StringTraits.hpp>

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

		template<typename Stringish, class = std::enable_if_t<
			klib::type_trait::Is_Stringish_V<Stringish>
			>>
		void SetTag(const Stringish& tag)
		{
			tagName = tag;
		}

		void Reserve(size_t size);
		void Resize(size_t size);

	private:
		std::string tagName;
	};

	
}
