#include "Precompile.hpp"

#include "DebugCore.hpp"
#include "../Platform/OperatingSystem/MessageBox/MessageBox.hpp"

#include "../Logging/EngineLogger.hpp"
#include "../Util/Fmt.hpp"

#include <Utility/Logging/kLogging.hpp>
#include <Utility/String/kToString.hpp>
#include <Utility/Debug/kDebugger.hpp>


namespace krakoa::debug
{
	using namespace os;

	void RaiseNotice( const std::string_view& msg, const klib::SourceInfo& sourceInfo )
	{
		KRK_ERR(msg);

		const auto text = klib::ToString( "[Desc] {0}\n"
			"Click \"Abort\" to close application.\n"
			"Click \"Retry\" to break.\n"
			"Click \"Ignore\" to continue.\n"
			"[File]: {1}\n"
			"[Line]: {2}\n"
			"[Func]: {3}\n"
			, msg
			, sourceInfo.file
			, sourceInfo.line
			, sourceInfo.func
		);

		MessageBoxDisplay::Show( "Krakoa Debug Error", text,
			[]( MessageBoxResponse response )
			{
				if ( response == MessageBoxResponse::MSGBOX_CANCEL
					|| response == MessageBoxResponse::MSGBOX_ABORT )
				{
					std::terminate();
				}
				else if ( response == MessageBoxResponse::MSGBOX_TRY_AGAIN
					|| response == MessageBoxResponse::MSGBOX_RETRY )
				{
					klib::BreakPoint();
				}
			}
			, MessageBoxButtonSet::ABORT_RETRY_IGNORE
			, MessageBoxIcon::ERROR
			, MessageBoxDefaultButton::BTN_2
		);
	}

	void RaiseException( const std::string_view& msg, const klib::SourceInfo& sourceInfo, LogProfile& logger )
	{
		logger->AddEntry( klib::LogLevel::ERR,
			util::Fmt( "{0} {1}", msg, sourceInfo ) );

		throw std::runtime_error( msg.data() );
	}
}
