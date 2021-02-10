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
	namespace
	{
		constexpr auto g_MaxlogsToDisplay = 70;
	}

	struct PanelLogger : public klib::iLoggerDestination
	{
		~PanelLogger() override = default;
		void SetName(std::string* newName) override {}
		void AddEntry(const klib::LogEntry& entry) override
		{
			const auto& msg = entry.GetMsg();
			const auto& dsc = entry.GetDescriptor();
			const auto log = klib::ToString("[{0}]: {1}\n", dsc.lvl.ToUnderlying(), msg.text);
			logs << log;
		}
		bool IsOpen() const override
		{
			return true;
		}
		bool Open() override
		{
			return true;
		}
		void Close(const bool outputClosingMsg) override {}

		std::stringstream logs;
	};


	LoggerLayer::LoggerLayer()
		: LayerBase("Logger Layer")
	{
		AddDestinaitonToEngineLogger();
	}

	LoggerLayer::~LoggerLayer()
		= default;

	void LoggerLayer::OnRender()
	{
		KRK_PROFILE_FUNCTION();

		auto& engineLogger = EngineLogger::GetLogger();
		auto& panelLogger = engineLogger.GetExtraDestination<PanelLogger>(0);

		ui::DrawPanel("Console", [&]()
			{
				const auto logs = panelLogger.logs.str();
				auto lines = klib::Split(logs, "\n");
				const auto size = lines.size();

				if (size > g_MaxlogsToDisplay)
				{
					const auto remaining = size - g_MaxlogsToDisplay;
					lines.erase(lines.begin(), lines.begin() + remaining);
				}

				for (auto& line : lines)
				{
					ui::DrawRawText(line);
				}
			});
	}

	void LoggerLayer::AddDestinaitonToEngineLogger()
	{
		KRK_PROFILE_FUNCTION();

		EngineLogger::GetLogger().AddDestination<PanelLogger>();
	}
}
