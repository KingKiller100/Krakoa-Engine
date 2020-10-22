#include <pch.hpp>
#include "kFileLogger.hpp"

#include "kLogDescriptor.hpp"


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
			, filename(AppendFileExtension(fName, ".log"))
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
			filename = AppendFileExtension(newFilename, ".log");
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
			const std::string padding = "************************************************************************\n";
			std::string startLog = padding + "      File logger activated: ";
			startLog += openingMsg;
			startLog += "    " + GetDateInTextFormat(Date::DateTextLength::SHORT) + "    " + GetTimeText();
			startLog += "\n" + padding +"\n";
			Flush(startLog);
		}

		void FileLogger::AddEntry(const LogEntry& entry, const LogDescriptor& desc)
		{
			const auto timeStr = entry.time.ToString();
			auto logLine = ToString("[%s] [%s] [%s]:  %s",
				timeStr.data(),
				name.data(),
				desc.info.data(),
				entry.msg.data());

			if (desc.lvl >= LogLevel::ERRR)
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

		void FileLogger::AddBanner(const LogEntry& entry, const LogDescriptor& desc)
		{
			constexpr char format[] = "[%s] [%s] [%s]: %s";

			const auto bannerLine = ToString(format
			                                 , entry.time.ToString().data()
			                                 , name.data()
			                                 , desc.info.data()
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
				const std::string padding(72, '*');
				static constexpr char msg[]
					= "                     File Logging Concluded                            ";

				Flush(padding);
				Flush(msg);
				Flush(padding);

				logFileStream.close();
			}
		}

		void FileLogger::Flush(const std::string_view& msg)
		{
			logFileStream << msg;
			logFileStream.flush();
		}
	}
}
