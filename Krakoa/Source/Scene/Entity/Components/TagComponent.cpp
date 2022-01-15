#include "Precompile.hpp"
#include "TagComponent.hpp"

namespace krakoa::scene::ecs::components
{
	TagComponent::TagComponent(std::string_view tag)
		:tagName(tag)
	{}

	TagComponent::TagComponent(const char* tag)
		: tagName(tag)
	{
	}

	TagComponent::TagComponent(const std::string& tag)
		: tagName(tag)
	{
	}

	TagComponent::~TagComponent() noexcept
		= default;

	std::string_view TagComponent::GetTag() const
	{
		return tagName;
	}

	const char* TagComponent::GetRawTag() const
	{
		return tagName.data();
	}

	char* TagComponent::GetRawTag()
	{
		return tagName.data();
	}

	void TagComponent::Reserve(size_t size)
	{
		tagName.reserve(size);
	}

	void TagComponent::Resize(size_t size)
	{
		tagName.resize(size);
	}
}
