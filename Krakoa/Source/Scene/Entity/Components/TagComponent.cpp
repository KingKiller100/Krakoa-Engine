#include "Precompile.hpp"
#include "TagComponent.hpp"

namespace krakoa::scene::ecs::components
{
	TagComponent::TagComponent(const std::string_view& tag)
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

	const char* TagComponent::GetData() const
	{
		return tagName.data();
	}

	char* TagComponent::GetData()
	{
		return tagName.data();
	}

	void TagComponent::SetTag(const std::string_view& tag)
	{
		tagName = tag;
	}
}
