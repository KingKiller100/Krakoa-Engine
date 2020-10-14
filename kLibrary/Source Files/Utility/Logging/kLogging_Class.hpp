#pragma once

#include "../../HelperMacros.hpp"

#include <deque>
#include <fstream>
#include <map>
#include <string>

#include "kLogEntry.hpp"

namespace klib
{
	namespace kLogs
	{
		class iLogger;
		enum class LogLevel : unsigned short;
		
		class Logging
		{
			enum class LoggerType
			{
				FILE,
				SYSTEM,
			};
			
		public:
			using LogQueue = std::deque<std::string>;

		public:
			Logging(const std::string& directory, const std::string& filename);

			~Logging();

			/**
			 * \brief
			 *		Initializes logging system
			 * \note
			 *		No logging calls will function properly until this is called.
			 */
			void OutputInitialized();

			/**
			 * \brief
			 *		Change name of the logger
			 * \param[in] newName
			 *		STL string representing a name
			 */
			void SetName(const std::string_view& newName);

			/**
			 * \brief
			 *		Set minimum level of a log that can be stored
			 * \param[in] newMinLevel
			 *		New minimum log level
			 * \note
			 *		No logs less than this given level will be stored by the log system.
			 */
			void SetMinimumLoggingLevel(const LogLevel newMinLevel) noexcept;

			/**
			 * \brief
			 *		Toggles if logging system is enabled
			 */
			constexpr void ToggleLoggingEnabled() noexcept;

			/**
			 * \brief
			 *		Toggles if sub system outputting is enabled
			 */
			void ToggleSubSystemEnabled() noexcept;

			/**
			 * \brief
			 *		Toggles whether logs output to system to keep a local cache
			 */
			constexpr void SetCacheMode(const bool enable) noexcept;

			/**
			 * \brief
			 *		Change the directory the log file outputs to
			 * \param dir
			 *		STL string view representing the new directory
			 * \param fname
			 *		STL string view representing the new filename
			 */
			void ChangeOutputPath(const std::string_view& dir, const std::string_view& fname);

			/**
			 * \brief
			 *		Change the directory the log file outputs to
			 * \param dir
			 *		STL string view representing the new directory
			 */
			void ChangeOutputDirectory(const std::string_view dir);

			/**
			 * \brief
			 *		Change name of log filename
			 * \param newFileName
			 *		STL string view representing filename
			 */
			void ChangeFilename(const std::string_view newFileName);

			/**
			 * \brief
			 *		Gets the full output path of the logger
			 * \return
			 *		Gets the full output path of the logger
			 */
			std::string GetOutputPath() const;

			/**
			 * \brief
			 *		Appends the current log file with the current logging cached in the buffer
			 *		and pauses outputing to log file.
			 * \note
			 *		Allows user to keep logging but just keeps it in cache and outputs to subsystems
			 */
			void SuspendFileLogging();

			/**
			 * \brief
			 *		Continues logging and flushes cache to file
			 */
			void ResumeFileLogging();

			/**
			 * \brief
			 *		Outputs cached kLogs to file
			 */
			void FinalOutput();

			/**
			 * \brief
			 *		Flush stored log stream to file
			 */
			void Flush();

			/**
			 * \brief
			 *		Outputs all cached kLogs up to file with the logged error message at the end
			 * \param entry
			 *		Error message
			 * \param file
			 * \param line
			 */
			void OutputToFatalFile(const LogEntry& entry);

			/**
			 * \brief
			 *		Formats log message and level to the appropriate log message and then caches it
			 * \param[in] msg
			 *		Log message
			 * \param lvl
			 *		Log level type
			 * \param file
			 * \param line
			 */
			void AddEntry(const std::string_view& msg, const LogLevel lvl = LogLevel::NORM, const char* file = "", const unsigned line = 0);

			/**
			 * \brief
			 *		Formats the log banner to become the appropriate log banner message then caches it
			 * \param[in] msg
			 *		Log banner title
			 * \param type
			 *		The category/subject of the log banner
			 */
			void AddEntryBanner(const std::string_view msg, const std::string_view type);

			/**
			 * \brief
			 *		Returns previous entry text
			 *
			 * \return
			 *		String of the final log entry
			*/
			LogQueue::value_type GetLastCachedEntry();

			/**
			 * \brief
			 *		Deletes a given number of logs entries last entered
			 * \param numOfEntries
			 *		Amount of entries to delete
			*/
			void ErasePreviousCacheEntries(const size_t numOfEntries);

			/**
			 * \brief
			 *		Deletes all log entries
			*/
			void ClearCache();

			/**
			 * \brief
			 *		Enables constant flushing
			 * \param enable
			 *		Boolean representing whether to constantly flush or not
			 */
			void EnableConstantFlush(bool enable);

		private:
			void SetUp(const std::string& directory, const std::string& filename);

			/**
			 * \brief
			 *		Creates the log file with the cached log entries
			 */
			void OutputLogToFile(const std::string_view& line);

			/**
			 * \brief
			 *		Places the cached kLogs from the queue to a single string object
			 * \return
			 *		The cached string of all currently cached kLogs
			 */
			std::string GetFullCache();

			/**
			 * \brief
			 *		Adds log message to the cache
			 * \param logLine
			 *		Full log message
			 */
			void AddToLogBuffer(const std::string_view& logLine);

			/**
			 * \brief
			 *		Outputs log message to sub systems (i.e. console and output debug console)
			 * \param[in] logLine
			 *		STL string_view representing the log message
			 * \param[in] lvl
			 *		Log level
			 */
			void OutputToSubSystems(const std::string_view& logLine, const LogLevel lvl) const noexcept;

			/**
			 * \brief
			 *		Outputs logs to file then closes it
			 */
			void Close();

		public:
			static const char* kLogs_Empty;

		protected:
			LogQueue logEntryQueue; // Queue buffer to cache the logged messages

			LogLevel minimumLoggingLevel;
			std::map<LoggerType, std::unique_ptr<iLogger>> loggers;
			std::string name;
			bool isEnabled;
			bool subSystemLoggingEnabled;
			bool inCacheMode;
		};
	}

#ifdef KLIB_SHORTHAND_NAMESPACE
	using namespace kLogs;
#endif
}
