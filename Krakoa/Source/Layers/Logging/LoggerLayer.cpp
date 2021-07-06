#include "Precompile.hpp"
#include "LoggerLayer.hpp"

#include "../../UI/ImGui/UI.hpp"
#include "../../Debug/Debug.hpp"
#include "../../Debug/Instrumentor.hpp"
#include "../../Logging/EngineLogger.hpp"

#include <Utility/String/kToString.hpp>

#include <sstream>

namespace krakoa
{
	struct PanelLogger : public klib::iLoggerDestination
	{
	public:
		~PanelLogger() override = default;
		void SetName(std::string* newName) override {}
		void AddEntry(const klib::LogEntry& entry) override
		{
			const auto& msg = entry.GetMsg();
			const auto& dsc = entry.GetDescriptor();
			const auto log = 
				klib::ToString("[{0}]: {1}\n", 
					msg.time.ToString("hh:mm:ss:ccc"), 
					msg.text);
			logs << log;
		}
		void AddEntry(const klib::LogMessage& message, const klib::LogDescriptor& descriptor)
		{
			AddEntry({ message, descriptor });
		}
		bool IsOpen() const override
		{
			return true;
		}
		bool Open() override
		{
			const auto msg = klib::LogMessage("Initialized panel");
			const auto descriptor = klib::LogDescriptor(KRK_LOG_LVL_INF);
			AddEntry(msg, descriptor);
			return true;
		}
		void Close(const bool outputClosingMsg) override
		{}

	public:
		std::stringstream logs;
	};

	namespace
	{
		PanelLogger& GetLogger()
		{
			auto& engineLogger = EngineLogger::GetLogger();
			auto& panelLogger = engineLogger.GetExtraDestination<PanelLogger>(0);
			return panelLogger;
		}
	}


	LoggerLayer::LoggerLayer()
		: LayerBase("Logger Layer")
	{
		AddDestinaitonToEngineLogger();
	}

	LoggerLayer::~LoggerLayer()
		= default;

	void LoggerLayer::OnAttach()
	{
		GetLogger().Open();
	}

	void LoggerLayer::OnRender()
	{
		KRK_PROFILE_FUNCTION();
		static constexpr auto maxLogsToDisplay = 70;

		ui::DrawPanel("Console", []()
		{
			const auto logs = GetLogger().logs.str();
			auto lines = klib::Split(logs, "\n");
			const auto size = lines.size();

			if (size > maxLogsToDisplay)
			{
				const auto remaining = size - maxLogsToDisplay;
				lines.erase(lines.begin(), lines.begin() + remaining);
			}

			for (auto rIter = lines.rbegin(); rIter != lines.rend(); ++rIter)
			{
				ui::DrawRawText(*rIter);
			}
		});
	}

	void LoggerLayer::AddDestinaitonToEngineLogger()
	{
		KRK_PROFILE_FUNCTION();

		EngineLogger::GetLogger().AddDestination<PanelLogger>();
	}
}
