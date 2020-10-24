﻿#include "pch.hpp"
#include "kFileLogger.hpp"

#include "kLogEntry.hpp"

#include "../Calendar/kCalendar.hpp"
#include "../Format/kFormatToString.hpp"
#include "../File System/kFileSystem.hpp"

#include <iostream>

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
			Close(true);
			filename = AppendFileExtension(newFilename, ".log");
			Open();
		}

		std::string_view FileLogger::GetDirectory() const
		{
			return directory;
		}

		void FileLogger::SetDirectory(const std::string_view& newDir)
		{
			Close(true);
			directory = newDir;
			Open();
		}

		std::string FileLogger::GetPath() const
		{
			return directory + filename;
		}

		void FileLogger::OutputInitialized(const std::string_view& openingMsg)
		{
			if (!IsOpen())
			{
				const auto msg = ToString("{0}: file logger not open", name);
				std::cout << msg << std::endl;
				throw std::runtime_error(msg);
			}
		
			const std::string spacing(5, ' ');
			const std::string padding(73, '*');
			constexpr auto nl = type_trait::s_NewLine<char>;
			
			std::string opener = padding + nl;
			opener += spacing + "File logger activated: ";
			opener += spacing + GetDateInTextFormat(Date::DateTextLength::SHORT) + spacing + GetTimeText() + nl;
			opener += openingMsg;
			opener += nl + padding + nl;
			
			Flush(opener);
		}

		bool FileLogger::Open()
		{
			if (!IsOpen())
			{
				const auto path = GetPath();
				CreateNewDirectories(directory.c_str());
				fileStream.open(path, std::ios::out | std::ios::in | std::ios::app);
			}

			return fileStream.good();
		}

		bool FileLogger::IsOpen()
		{
			return fileStream.is_open() && fileStream.good();
		}

		void FileLogger::AddEntry(const LogEntry& entry)
		{
			if (!IsOpen())
				return;

			const auto logLine = CreateLogText(entry);
			
			Flush(logLine);
		}

		std::string FileLogger::CreateLogText(const LogEntry& entry) const
		{
			std::string logLine;

			const auto& message = entry.GetMsg();
			const auto& desc = entry.GetDescriptor();

			if (entry.HasDescription(LogDescriptor(LogLevel::VBAT)))
			{
				logLine = message.text;
			}
			else
			{
				const auto timeStr = message.time.ToString();
				const auto dateStr = message.date.ToString(Date::SLASH);

				logLine = ToString("[{0}] [{1}] [{2}] [{3}]:  {4}",
					dateStr,
					timeStr,
					name,
					desc.info,
					message.text);
			}

			if (desc.lvl >= LogLevel::ERRR)
			{
				logLine.append(ToString(R"(
               [FILE]: {0}
               [LINE]: {1})",
					message.file,
					message.line)
				);
			}

			logLine.push_back(type_trait::s_NewLine<char>);

			return logLine;
		}

		void FileLogger::Close(const bool outputClosingMsg)
		{
			if (!IsOpen())
				return;

			if (outputClosingMsg)
			{
				const std::string padding(72, '*');
				static constexpr char msg[]
					= "                     File Logging Concluded                            ";

				Flush(padding);
				Flush(msg);
				Flush(padding);
			}

			fileStream.close();
		}

		void FileLogger::Flush(const std::string_view& msg)
		{
			if (!IsOpen())
				return;
			
			fileStream << msg;
			fileStream.flush();
		}
	}
}
