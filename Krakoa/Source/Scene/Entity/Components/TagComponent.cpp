#include "Precompile.hpp"
#include "TagComponent.hpp"

#include <Template/kTypeName.hpp>

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

	const std::string& TagComponent::GetTag() const
	{
		return tagName;
	}

	void TagComponent::SetTag(const std::string_view& tag)
	{
		tagName = tag;
	}

	const char* TagComponent::GetType() const noexcept
	{
		return klib::GetTypeName<TagComponent>();
	}
}
