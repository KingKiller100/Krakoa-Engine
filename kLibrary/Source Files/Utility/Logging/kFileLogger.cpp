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
		FileLogger::FileLogger(const std::string_view& newName, const std::string_view& dir, const std::string_view& fName)
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
			Close();
			filename = newFilename;
			Open();
		}

		std::string_view FileLogger::GetDirectory() const
		{
			return directory;
		}

		void FileLogger::SetDirectory(const std::string_view& newDir)
		{
			Close();
			directory = newDir;
			Open();
		}

		std::string FileLogger::GetPath() const
		{
			return directory + filename;
		}

		void FileLogger::OutputInitialized(const std::string_view& openingMsg)
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

			if (entry.lvl >= LogEntry::LogLevel::ERRR)
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

		void FileLogger::AddBanner(const BannerEntry& entry)
		{
			std::string front, back;

			for (auto i = 0; i < entry.paddingCount; ++i)
			{
				front.append(entry.frontPadding);
				back.append(entry.backPadding);
			}

			const auto format = "[%s] [%s] [%s]: " + front + " %s" + back;
			const auto bannerLine = ToString(format
			                                 , entry.time.ToString().data()
			                                 , name.data()
			                                 , entry.type.data()
			                                 , entry.msg.data()
			);

			Flush(bannerLine);
		}

		bool FileLogger::Open()
		{
			if (!logFileStream.is_open())
			{
				const auto path = GetPath();
				CreateNewDirectories(directory.c_str());
				logFileStream.open(path, std::ios::out | std::ios::in | std::ios::app);
			}

			return logFileStream.good();
		}

		void FileLogger::Close()
		{
			if (logFileStream.is_open())
			{
				constexpr char endLogLine[]
					= R"(
************************************************************************
                     File Logging Concluded                            
************************************************************************
)";
				Flush(endLogLine);
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
