#pragma once

#include <Utility/Debug/Source/kSourceInfo.hpp>


namespace krakoa::debug
{
	void RaiseNotice( std::string_view msg, const klib::SourceInfo& sourceInfo );
	
	void RaiseException( std::string_view msg );
}
