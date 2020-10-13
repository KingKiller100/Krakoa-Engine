#include <pch.hpp>
#include "kFileLogger.hpp"

#include "../Calendar/kCalendar.hpp"
#include "../Format/kFormatToString.hpp"
#include "../File System/kFileSystem.hpp"


namespace klib
{
	using namespace kFormat;
	using namespace kCalendar;
	using namespace kFileSystem;
	
	namespace kLogs
	{
		FileLogger::FileLogger(const std::string& newName, const std::string& dir, const std::string& fName)
			: name(newName)
			, directory(dir)
			, filename(fName)
		{}

		FileLogger::~FileLogger() noexcept
			= default;

		std::string_view FileLogger::GetName() const
		{
			return name;
		}

		void FileLogger::SetName(const std::string_view& newName)
		{
			name = newName;
		}

		std::string_view FileLogger::GetFileName() const
		{
			return filename;
		}

		void FileLogger::SetFileName(const std::string_view& newFilename)
		{
			filename = newFilename;
		}

		std::string_view FileLogger::GetDirectory() const
		{
			return directory;
		}

		void FileLogger::SetDirectory(const std::string_view& newDir)
		{
			directory = newDir;
		}

		void FileLogger::OutputInitialized(const std::string& openingMsg)
		{

			const auto startLog =
				"************************************************************************\n      File logger activated: "
				+ openingMsg + "    " + GetDateInTextFormat(Date::DateTextLength::SHORT) + "    " + GetTimeText()
				+ "\n************************************************************************\n\n";
			Flush(startLog);
		}

		void FileLogger::AddEntry(const LogEntry& entry)
		{
			auto logLine = ToString("[%s] [%s] [%s]:  %s",
				entry.time.ToString().data(),
				name.data(),
				//kLogs_LLevelMap.at(lvl),
				entry.msg.data());

			if (entry.lvl >= LogLevel::ERRR)
			{
				logLine.append(ToString(R"(
               [FILE]: %s
               [LINE]: %d)",
					entry.file,
					entry.line)
				);
			}

			logLine.append("\n");

			Flush(logLine);
		}

		void FileLogger::AddEntryBanner(const LogEntry& entry, const std::string_view type, 
			const std::string frontPadding, const std::string& backPadding, size_t paddingCount)
		{
			std::string front, back;

			for (auto i = 0; i < paddingCount; ++i)
			{
				front.append(frontPadding);
				back.append(backPadding);
			}

			auto format = "[%s] [%s] [%s]: " + front + "  %s" + back;
			const auto bannerLine = ToString(format
			                                 , entry.time.ToString().data()
			                                 , name.data()
			                                 , type.data()
			                                 , entry.msg.data()
			);

			Flush(bannerLine);
		}

		bool FileLogger::Open()
		{
			if (!logFileStream.is_open())
			{
				const auto path = directory + filename;
				CreateNewDirectories(directory.c_str());
				logFileStream.open(path, std::ios::out | std::ios::in | std::ios::app);
			}

			return logFileStream.good();
		}

		void FileLogger::Close()
		{
			if (logFileStream.is_open())
			{
				logFileStream.flush();
				logFileStream.close();
			}
		}

		void FileLogger::Flush(const std::string& msg)
		{
			logFileStream << msg;
			logFileStream.flush();
		}
	}
}
