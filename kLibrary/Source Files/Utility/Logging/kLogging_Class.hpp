#pragma once

#include <HelperMacros.hpp>
#include <string>
#include <deque>
#include <unordered_map>

namespace klib::kLogs
{
	enum LoggingConsoleColour : unsigned short
	{
		NAVY_BLUE = 1,
		DARK_GREEN,
		AQUA_BLUE,
		DARK_RED,
		PURPLE,
		MUSTARD_YELLOW,
		LIGHT_GREY,
		GREY,
		DARK_BLUE,
		LIGHT_GREEN,
		LIGHT_BLUE,
		SCARLET_RED,
		VIOLET_PURPLE,
		YELLOW,
		WHITE,
		RED_BG_WHITE_TEXT = 79
	};

	enum class LogLevel : unsigned short
	{
		BANR, // Log Banner
		NORM, // Normal
		INFO, // Informative
		WARN, // Warning
		ERRR, // Error
		FATL  // Fatal
	};

	using LogQueue = std::deque<std::string>;

	class Logging
	{
	public:
		Logging();
		Logging(std::string& filename, std::string& directory);

		~Logging();

		/**
		 * \brief
		 *		Initializes logging system
		 * \param[in] initialMinLevel
		 *		Initial minimum log level
		 * \note
		 *		No logging calls will function properly until this is called.
		 */
		void InitializeLogging(const LogLevel& initialMinLevel);

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
		void ToggleLoggingEnabled() noexcept;

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
		 *		Appends the current log file with the current logging cached in the buffer
		 */
		void AppendLogFile();

		/**
		 * \brief
		 *		Outputs cached kLogs to file
		 */
		void FinalOutput();

		/**
		 * \brief
		 *		Outputs all cached kLogs up to file with the logged error message at the end
		 * \param msg
		 *		Error message
		 * \param file
		 * \param line
		 */
		void OutputToFatalFile(const std::string_view& msg, const char* file, const unsigned line);

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
		void AddEntry(const std::string_view msg, const LogLevel lvl = LogLevel::NORM, const char* file = "", const unsigned line = 0) noexcept;

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
		LogQueue::value_type GetLastLoggedEntry();

		/**
		 * \brief
		 *		Deletes a given number of logs entries last entered
		 * \param numOfEntries
		 *		Amount of entries to delete
		*/
		void ErasePreviousEntries(const size_t numOfEntries);

		/**
		 * \brief
		 *		Deletes all log entries
		*/
		void Clear();

	private:
		/**
		 * \brief
		 *		Creates the log file with the cached log entries
		 */
		void OutputLogToFile();

		/**
		 * \brief
		 *		Places the cached kLogs from the queue to a single string object
		 * \return
		 *		The cached string of all currently cached kLogs
		 */
		std::string GetFullLogText();

		/**
		 * \brief
		 *		Adds log message to the cache
		 * \param logLine
		 *		Full log message
		 */
		void AddToLogBuffer(const std::string_view& logLine, const LogLevel lvl);

		/**
		 * \brief
		 *		Stores all log levels into a map with the string equivalent
		 */
		void InitializeLogLevelMap();

		void OutputToConsole(const std::string_view& logLine, const LogLevel lvl) noexcept;

		void InitializeOutputToConsoleColourMap();

	public:
		static const char* kLogs_Empty;

	protected:
		LogQueue logEntryQueue; // Queue buffer to cache the logged messages

		LogLevel minimumLoggingLevel;

		std::string directory;
		std::string filename;

		std::unordered_map<LogLevel, const char*> kLogs_LogLevelMap;
		std::unordered_map<LogLevel, LoggingConsoleColour> kLogs_ConsoleColourMap;

		bool enable_kLogging;
	};

}

