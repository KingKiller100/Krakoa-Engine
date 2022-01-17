#include "Precompile.hpp"
#include "LayerBase.hpp"
#include "../Debug/Debug.hpp"
#include "../Util/Fmt.hpp"

namespace krakoa
{
	LayerBase::LayerBase( const char* name )
		: debugName( name )
		, enabled( true )
	{}

	LayerBase::~LayerBase()
	= default;

	void LayerBase::SetApp( std::shared_ptr<Application> app )
	{
		app_ = app;
	}

	Application& LayerBase::GetApp() const
	{
		if ( app_.expired() )
		{
			KRK_FATAL( util::Fmt("{0} layer's Application ptr is empty", GetName()) );
		}
		return *app_.lock();
	}

	constexpr std::string_view LayerBase::GetName() const
	{
		return debugName;
	}
}
