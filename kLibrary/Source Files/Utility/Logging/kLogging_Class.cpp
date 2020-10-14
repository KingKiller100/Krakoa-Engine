#include "pch.hpp"
#include "kLogging_Class.hpp"

#include "../Format/kFormatToString.hpp"

#include "kFileLogger.hpp"
#include "kLogEntry.hpp"
#include "kSystemLogger.hpp"

#include "../../Maths/kAlgorithms.hpp"
#include "../../Type Traits/ToImpl.hpp"

#include "../File System/kFileSystem.hpp"


namespace klib::kLogs
{
	using namespace kFormat;
	using namespace kCalendar;
	using namespace type_trait;

	const char* Logging::kLogs_Empty = "NO ENTRIES! CACHE IS EMPTY";

	Logging::Logging(const std::string& directory, const std::string& filename)
		: minimumLoggingLevel(LogEntry::LogLevel::NORM),
		name("logger"),
		isEnabled(false),
		subSystemLoggingEnabled(false),
		inCacheMode(false),
		constantFlushing(false),
		logIndex(0)
	{
		Initialize(filename, directory);
	}

	Logging::~Logging()
	{
		if (!logEntries.empty() || !bannerEntries.empty())
			FinalOutput();
	}

	void Logging::Initialize(const std::string& directory, const std::string& filename)
	{
		loggers[LoggerType::FILE].reset(new FileLogger(name, directory, filename));
		loggers[LoggerType::SYSTEM].reset(new SystemLogger(name));
		
		SetMinimumLoggingLevel(LogEntry::LogLevel::NORM);
		ToggleLoggingEnabled();
	}

	void Logging::Open()
	{
		for (auto& logger : loggers)
		{
			if (!logger.second->Open())
			{
				const auto errMsg = ToString("Unable to open log: {0}", name);
				throw std::runtime_error(errMsg);
			}
		}
	}

	void Logging::OutputInitialized(const std::string_view& startLog)
	{
		if (!isEnabled) { return; }
		
		for (auto& logger : loggers)
		{
			logger.second->OutputInitialized(startLog);
		}
	}

	constexpr void Logging::ToggleLoggingEnabled() noexcept
	{
		isEnabled = !isEnabled;
	}

	void Logging::SetName(const std::string_view& newName)
	{
		name = newName;
		for (const auto& logger : loggers)
		{
			logger.second->SetName(newName);
		}
	}

	void Logging::SetMinimumLoggingLevel(const LogEntry::LogLevel newMin) noexcept
	{
		minimumLoggingLevel = newMin;
	}

	void Logging::ToggleSubSystemEnabled() noexcept
	{
		subSystemLoggingEnabled = !subSystemLoggingEnabled;
	}

	constexpr void Logging::SetCacheMode(const bool enable) noexcept
	{
		inCacheMode = enable;
	}

	void Logging::ChangeOutputPath(const std::string_view & dir, const std::string_view & fname)
	{
		ChangeOutputDirectory(dir);
		ChangeFilename(fname);
	}

	void Logging::ChangeOutputDirectory(const std::string_view dir)
	{
		auto& fLogger = type_trait::ToImpl<FileLogger>(loggers.at(LoggerType::FILE));
		fLogger.SetDirectory(dir);
	}

	void Logging::ChangeFilename(const std::string_view newFileName)
	{
		auto& fLogger = type_trait::ToImpl<FileLogger>(loggers.at(LoggerType::FILE));
		const auto filename = kFileSystem::AppendFileExtension(newFileName.data(), ".log");
		fLogger.SetFileName(filename);
	}
	
	std::string Logging::GetOutputPath() const
	{
		auto& fLogger = type_trait::ToImpl<FileLogger>(loggers.at(LoggerType::FILE));
		const auto path = fLogger.GetPath();
		return path;
	}

	void Logging::SuspendFileLogging()
	{
		constexpr const auto* const pauseLog = "\n************************************************************************\n";
		Close();
		SetCacheMode(true);
	}

	void Logging::ResumeFileLogging()
	{
		SetCacheMode(false);
	}


	void Logging::OutputToFatalFile(const LogEntry& entry)
	{
		AddEntry(entry);
		FinalOutput();
	}

	void Logging::AddEntry(const LogEntry& entry)
	{
		if (!isEnabled || !Loggable(entry.lvl)) 
			return;

		logEntries.insert(std::make_pair(logIndex++, entry));
	}

	void Logging::AddBanner(const BannerEntry& entry)
	{
		if (!isEnabled)
			return;

		bannerEntries.insert(std::make_pair(logIndex++, entry));
	}

	void Logging::FinalOutput()
	{
		Close();
		isEnabled = false;
	}

	void Logging::Flush()
	{
		const auto start = kmaths::Min(
			logEntries.begin()->first
			, bannerEntries.begin()->first
			);

		for (auto i = start; i < logIndex; ++i)
		{
			if (auto entry_iter = logEntries.find(i); entry_iter != logEntries.end())
			{
				for (auto& logger : loggers)
				{
					logger.second->AddEntry(entry_iter->second);
				}
			}
			else
			{
				const auto& entry = bannerEntries.at(i);
				
				for (auto& logger : loggers)
				{
					logger.second->AddBanner(entry);
				}
			}
		}
	}

	void Logging::Close()
	{
		for (auto& logger : loggers)
		{
			logger.second->Close();
		}
	}

	bool Logging::Loggable(const LogEntry::LogLevel lvl) const
	{
		return (lvl > minimumLoggingLevel);
	}

	std::string_view Logging::GetLastCachedEntry()
	{
		if (logEntries.empty() && bannerEntries.empty())
			return kLogs_Empty;

		if (!inCacheMode)
			return "CHECK LOGGING FILE: " + GetOutputPath();

		const auto lastlog_iter = logEntries.crbegin();
		const auto lastbanner_iter = bannerEntries.crbegin();
		
		const auto lastIndex = kmaths::Max(lastlog_iter->first,
			lastbanner_iter->first);

		if (lastIndex == lastlog_iter->first)
		{
			return lastlog_iter->second.msg;
		}
		else
		{
			return lastbanner_iter->second.msg;
		}
	}

	void Logging::ClearCache()
	{
		if (!logEntries.empty())
			logEntries.clear();

		if (bannerEntries.empty())
			bannerEntries.clear();
	}

	void Logging::EnableConstantFlush(bool enable)
	{
		constantFlushing = enable;
	}
}
