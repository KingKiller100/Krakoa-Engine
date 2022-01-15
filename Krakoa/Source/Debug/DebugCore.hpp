#pragma once

#include "../Util/Fmt.hpp"
#include <Utility/Debug/Source/kSourceInfo.hpp>


namespace krakoa::debug
{
	void RaiseNotice( const std::string_view& msg, const klib::SourceInfo& sourceInfo );

	template <typename Logger>
	void RaiseException( const std::string_view& msg, const klib::SourceInfo& sourceInfo )
	{
		Logger::Error( msg, sourceInfo );

		throw std::runtime_error( util::Fmt( "{0}\n{1}", msg.data(), sourceInfo ) );
	}
}
