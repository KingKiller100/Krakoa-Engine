#include "pch.hpp"
#include "kLogging.hpp"


#include "kLogEntry.hpp"
#include "kFileLogger.hpp"
#include "kConsoleLogger.hpp"
#include "kLogDescriptor.hpp"
#include "iLogDestination.hpp"

#include "../../Type Traits/ToImpl.hpp"
#include "../Format/kFormatToString.hpp"
#include "../File System/kFileSystem.hpp"


namespace klib::kLogs
{
	using namespace kFormat;
	using namespace kCalendar;
	using namespace type_trait;

	const char* Logging::kLogs_Empty = "NO ENTRIES! CACHE IS EMPTY";

	Logging::Logging(const std::string_view& directory, const std::string_view& filename, const std::string_view& name)
		: minimumLoggingLevel(LogLevel::NORM),
		name(name),
		isEnabled(false),
		consoleLoggingEnabled(false),
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
		destinations[DestionationType::FILE].reset(new FileLogger(name, directory, filename));
		destinations[DestionationType::CONSOLE].reset(new ConsoleLogger(name));
		
		SetMinimumLoggingLevel(LogLevel::NORM);
		ToggleLoggingEnabled();
	}

	void Logging::Open()
	{
		for (auto& dest : destinations)
		{
			if (!dest.second->Open())
			{
				const auto destType = DestionationType::PrettyType(dest.first);
				const auto errMsg = ToString("{0} - Unable to open log destination: {1}", name, destType);
				throw std::runtime_error(errMsg);
			}
		}
	}

	void Logging::OutputInitialized(const std::string_view& openingMsg)
	{
		if (!isEnabled) { return; }
		
		for (auto& dest : destinations)
		{
			dest.second->OutputInitialized(openingMsg);
		}
	}

	constexpr void Logging::ToggleLoggingEnabled() noexcept
	{
		isEnabled = !isEnabled;
	}

	void Logging::SetName(const std::string_view& newName)
	{
		name = newName;
		for (const auto& dest : destinations)
		{
			dest.second->SetName(newName);
		}
	}

	void Logging::SetMinimumLoggingLevel(const LogLevel newMin) noexcept
	{
		minimumLoggingLevel = newMin;
	}

	void Logging::ToggleConsoleEnabled() noexcept
	{
		consoleLoggingEnabled = !consoleLoggingEnabled;
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

	void Logging::ChangeOutputDirectory(const std::string_view& directory)
	{
		auto& fLogger = type_trait::ToImpl<FileLogger>(destinations.at(DestionationType::FILE));
		fLogger.SetDirectory(directory);
	}

	void Logging::ChangeFilename(const std::string_view& fname)
	{
		auto& fLogger = type_trait::ToImpl<FileLogger>(destinations.at(DestionationType::FILE));
		const auto filename = kFileSystem::AppendFileExtension(fname, ".log");
		fLogger.SetFileName(filename);
	}
	
	std::string Logging::GetOutputPath() const
	{
		auto& fLogger = type_trait::ToImpl<FileLogger>(destinations.at(DestionationType::FILE));
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

	void Logging::AddBanner(const LogEntry& entry, const std::string& desc
	                        , const std::string& frontPadding, const std::string& backPadding, const std::uint16_t paddingCount)
	{
		if (!isEnabled)
			return;

		constexpr  auto format = "%s %s %s";
		
		std::string front, back;

		for (auto i = 0; i < paddingCount; ++i)
		{
			front.append(frontPadding);
			back.append(backPadding);
		}
		
		const auto msg = ToString(format
			, frontPadding.c_str()
			, entry.msg.c_str()
			, backPadding.c_str()
		);
		
		entriesQ.emplace_back(LogEntry(msg.data(), entry.file.data(), entry.line), LogDescriptor(desc));
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
			for (auto& dest : destinations)
			{
				if (!consoleLoggingEnabled)
				{
					if (dest.first == DestionationType::CONSOLE)
						continue;
				}
				
				const auto& details = entriesQ.front();
				const auto& entry = details.first;
				const auto& desc = details.second;

				if (desc.lvl == LogLevel::BANR)
					dest.second->AddBanner(entry, desc);
				else
					dest.second->AddEntry(entry, desc);
			}
			entriesQ.pop_front();
		}
	}

	void Logging::Close()
	{
		Flush();
		
		for (auto& dest : destinations)
		{
			dest.second->Close();
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

		const auto& lastLog = entriesQ.front();
		
		return lastLog.first.msg;
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
