#include  "Precompile.hpp"
#include "Transform.hpp"

namespace krakoa::component
{
	const char* Transform::GetType() const noexcept
	{
		return GetStaticType();
	}
}
