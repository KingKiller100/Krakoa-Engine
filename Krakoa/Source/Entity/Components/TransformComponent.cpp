#include  "Precompile.hpp"
#include "TransformComponent.hpp"

namespace krakoa::components
{
	TransformComponent::~TransformComponent() noexcept
		= default;

	void TransformComponent::SetScale(const kmaths::Vector2f& value)
	{
		KRK_PROFILE_FUNCTION();
		SetScale(kmaths::Vector3f{ value.x, value.y, 1.f });
	}

	const char* TransformComponent::GetType() const noexcept
	{
		KRK_PROFILE_FUNCTION();
		
		return GetStaticType();
	}
}
