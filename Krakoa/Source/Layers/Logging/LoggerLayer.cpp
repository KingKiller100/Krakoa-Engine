#include "Precompile.hpp"
#include "LoggerLayer.hpp"

#include "../../Debug/Debug.hpp"
#include "../../Debug/Instrumentor.hpp"
#include "../../Logging/EngineLogger.hpp"
#include "../../UI/ImGui/UI.hpp"

#include <Template/kToImpl.hpp>
#include <Utility/String/kToString.hpp>

#include <sstream>

namespace krakoa
{
	namespace
	{
		constexpr auto g_MaxlogsToDisplay = 30;
	}

	struct PanelLogger : public klib::iLoggerDestination
	{
		~PanelLogger() override = default;
		void SetName(std::string* newName) override {}
		void AddEntry(const klib::LogEntry& entry) override
		{
			const auto& msg = entry.GetMsg();
			const auto& dsc = entry.GetDescriptor();
			const auto log = klib::ToString("[{0}]: {1}\n", dsc.lvl, msg.text);
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
		auto& panelLogger = klib::ToImpl<PanelLogger>(engineLogger.GetExtraDestination(0));

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
					ui::DrawRawText("{0}", line.data());
			});
	}

	void LoggerLayer::AddDestinaitonToEngineLogger()
	{
		KRK_PROFILE_FUNCTION();

		EngineLogger::GetLogger().AddDestination<PanelLogger>();
	}
}
