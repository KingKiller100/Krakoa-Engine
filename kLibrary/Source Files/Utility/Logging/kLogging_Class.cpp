#include "pch.hpp"
#include "kLogging_Class.hpp"


#include "kLogEntry.hpp"
#include "kFileLogger.hpp"
#include "kConsoleLogger.hpp"
#include "kLogDescriptor.hpp"

#include "../../Type Traits/ToImpl.hpp"
#include "../Format/kFormatToString.hpp"
#include "../File System/kFileSystem.hpp"


namespace klib::kLogs
{
	using namespace kFormat;
	using namespace kCalendar;
	using namespace type_trait;

	const char* Logging::kLogs_Empty = "NO ENTRIES! CACHE IS EMPTY";

	Logging::Logging(const std::string_view& directory, const std::string_view& filename
		, const std::string_view& name)
		: minimumLoggingLevel(LogLevel::NORM),
		name(name),
		isEnabled(false),
		subSystemLoggingEnabled(false),
		cacheMode(false),
		constantFlushing(false)
	{
		Initialize(directory, filename);
	}

	Logging::~Logging()
	{
		if (!entriesQ.empty())
			FinalOutput();
	}

	void Logging::Initialize(const std::string_view& directory, const std::string_view& filename)
	{
		destinations[LoggerType::FILE].reset(new FileLogger(name, directory, filename));
		destinations[LoggerType::SYSTEM].reset(new ConsoleLogger(name));
		
		SetMinimumLoggingLevel(LogLevel::NORM);
		ToggleLoggingEnabled();
	}

	void Logging::Open()
	{
		for (auto& logger : destinations)
		{
			if (!logger.second->Open())
			{
				const auto errMsg = ToString("Unable to open log: {0}", name);
				throw std::runtime_error(errMsg);
			}
		}
	}

	void Logging::OutputInitialized(const std::string_view& openingMsg)
	{
		if (!isEnabled) { return; }
		
		for (auto& logger : destinations)
		{
			logger.second->OutputInitialized(openingMsg);
		}
	}

	constexpr void Logging::ToggleLoggingEnabled() noexcept
	{
		isEnabled = !isEnabled;
	}

	void Logging::SetName(const std::string_view& newName)
	{
		name = newName;
		for (const auto& logger : destinations)
		{
			logger.second->SetName(newName);
		}
	}

	void Logging::SetMinimumLoggingLevel(const LogLevel newMin) noexcept
	{
		minimumLoggingLevel = newMin;
	}

	void Logging::ToggleSubSystemEnabled() noexcept
	{
		subSystemLoggingEnabled = !subSystemLoggingEnabled;
	}

	constexpr void Logging::SetCacheMode(const bool enable) noexcept
	{
		cacheMode = enable;
	}

	void Logging::ChangeOutputPath(const std::string_view & dir, const std::string_view & fname)
	{
		ChangeOutputDirectory(dir);
		ChangeFilename(fname);
	}

	void Logging::ChangeOutputDirectory(const std::string_view dir)
	{
		auto& fLogger = type_trait::ToImpl<FileLogger>(destinations.at(LoggerType::FILE));
		fLogger.SetDirectory(dir);
	}

	void Logging::ChangeFilename(const std::string_view newFileName)
	{
		auto& fLogger = type_trait::ToImpl<FileLogger>(destinations.at(LoggerType::FILE));
		const auto filename = kFileSystem::AppendFileExtension(newFileName.data(), ".log");
		fLogger.SetFileName(filename);
	}
	
	std::string Logging::GetOutputPath() const
	{
		auto& fLogger = type_trait::ToImpl<FileLogger>(destinations.at(LoggerType::FILE));
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
		AddEntry(LogLevel::FATL,entry);
		FinalOutput();
	}

	void Logging::AddEntry(const LogLevel lvl, const LogEntry& entry)
	{
		if (!isEnabled || !IsLoggable(lvl)) 
			return;

		entriesQ.push_back(std::make_pair(entry, LogDescriptor(lvl)));
	}

	void Logging::AddBanner(LogEntry& entry, const std::string& desc
	                        , const std::string& frontPadding, const std::string& backPadding, const std::uint16_t paddingCount)
	{
		if (!isEnabled)
			return;

		static constexpr  auto format = "%s %s %s";
		
		std::string front, back;

		for (auto i = 0; i < paddingCount; ++i)
		{
			front.append(frontPadding);
			back.append(backPadding);
		}
		
		entry.msg = ToString(format
			, frontPadding.c_str()
			, entry.msg.c_str()
			, backPadding.c_str()
		);
		
		entriesQ.emplace_back(entry, LogDescriptor(desc));
	}

	void Logging::FinalOutput()
	{
		Close();
		isEnabled = false;
	}

	void Logging::Flush()
	{
		while (!entriesQ.empty())
		{
			for (auto& logger : destinations)
			{
				const auto& details = entriesQ.front();
				const auto& entry = details.first;
				const auto& desc = details.second;

				if (desc.lvl == LogLevel::BANR)
					logger.second->AddBanner(entry, desc);
				else
					logger.second->AddEntry(entry, desc);

			}
			entriesQ.pop_front();
		}
	}

	void Logging::Close()
	{
		Flush();
		
		for (auto& logger : destinations)
		{
			logger.second->Close();
		}
	}

	bool Logging::IsLoggable(const LogLevel lvl) const
	{
		return (lvl > minimumLoggingLevel);
	}

	std::string_view Logging::GetLastCachedEntry()
	{
		if (entriesQ.empty())
			return kLogs_Empty;

		if (!cacheMode)
			return "CHECK LOGGING FILE: " + GetOutputPath();

		const auto& lastlog = entriesQ.front();
		
		return lastlog.first.msg;
	}

	void Logging::ClearCache()
	{
		if (!entriesQ.empty())
			entriesQ.clear();
	}

	void Logging::EnableConstantFlush(bool enable)
	{
		constantFlushing = enable;
	}

	bool Logging::ErasePrevious(size_t count)
	{
		if (cacheMode)
			return false;

		if (entriesQ.empty())
			return false;

		do 
		{
			entriesQ.pop_front();
		} while (count-- != 0);

		return true;
	}
}
