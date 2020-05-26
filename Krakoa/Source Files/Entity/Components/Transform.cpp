#include  "Precompile.hpp"
#include "Transform.hpp"

namespace krakoa::components
{
	Transform::~Transform() noexcept
		= default;

	const char* Transform::GetType() const noexcept
	{
		KRK_PROFILE_FUNCTION();
		
		return GetStaticType();
	}
}
