#include <Precompile.hpp>
#include <Rendering/LayerBase.hpp>

namespace krakoa
{
	LayerBase::LayerBase(const char* name)
		: debugName(name),
		enabled(true)
	{}

	LayerBase::~LayerBase()
		= default;

	constexpr const char* LayerBase::GetName() const
	{
		return debugName;
	}
}


