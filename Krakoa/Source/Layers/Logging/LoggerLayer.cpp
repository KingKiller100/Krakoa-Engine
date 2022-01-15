#include "Precompile.hpp"
#include "LoggerLayer.hpp"

#include "../../UI/ImGui/UI.hpp"
#include "../../Debug/Debug.hpp"
#include "../../Debug/Instrumentor.hpp"
#include "../../Logging/EngineLogger.hpp"

#include <Utility/Logging/kLogEntry.hpp>
#include <Utility/String/kToString.hpp>

#include <sstream>

namespace krakoa
{
	struct PanelLogger : public klib::iLogDestination
	{
	public:
		~PanelLogger() override = default;

		void AddEntry( const klib::LogEntry& entry ) override
		{
			const auto& msg = entry.GetMsg();
			const auto log =
				klib::ToString( "[{0}]: {1}\n",
					msg.time.ToString( "hh:mm:ss:ccc" ),
					msg.text );
			logs_ << log;
		}

		bool IsOpen() const override
		{
			return true;
		}

		std::string_view GetName() const override
		{
			return "PanelLogger";
		}
		void AddRaw( const klib::LogMessage& message ) override
		{
			logs_ << message.text;
		}

		void Close() override
		{}

		void Open() override
		{ }

		std::stringstream logs_;
	};

	namespace
	{
		LogDest<PanelLogger> g_PanelLogger;
	}


	LoggerLayer::LoggerLayer()
		: LayerBase( "Logger Layer" )
	{
		AddDestinaitonToEngineLogger();
	}

	LoggerLayer::~LoggerLayer()
	= default;

	void LoggerLayer::OnAttach()
	{
		g_PanelLogger->Open();
	}

	void LoggerLayer::OnRender()
	{
		KRK_PROFILE_FUNCTION();
		static constexpr auto maxLogsToDisplay = 70;

		ui::DrawPanel( "Console", []()
		{
			const auto logs = g_PanelLogger->logs_.str();
			auto lines = klib::Split( logs, "\n" );
			const auto size = lines.size();

			if ( size > maxLogsToDisplay )
			{
				const auto remaining = size - maxLogsToDisplay;
				lines.erase( lines.begin(), lines.begin() + remaining );
			}

			for ( auto rIter = lines.rbegin(); rIter != lines.rend(); ++rIter )
			{
				ui::DrawRawText( *rIter );
			}
		} );
	}

	void LoggerLayer::AddDestinaitonToEngineLogger()
	{
		KRK_PROFILE_FUNCTION();

		g_PanelLogger = EngineLogger::AddDestination<PanelLogger>();
	}
}
