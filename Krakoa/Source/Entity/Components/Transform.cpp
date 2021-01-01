#include  "Precompile.hpp"
#include "Transform.hpp"

namespace krakoa::components
{
	Transform::~Transform() noexcept
		= default;

	void Transform::SetScale(const kmaths::Vector2f& value)
	{
		SetScale(kmaths::Vector3f{ value.x, value.y, 1.f });
	}

	const char* Transform::GetType() const noexcept
	{
		KRK_PROFILE_FUNCTION();
		
		return GetStaticType();
	}
}
