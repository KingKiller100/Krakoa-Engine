#pragma once

#include "../../HelperMacros.hpp"

#include <deque>
#include <fstream>
#include <string>

namespace klib::kLogs
{
	enum class LLevel : unsigned short
	{
		BANR, // Log Banner
		DBUG, // Debug
		NORM, // Normal
		INFO, // Informative
		WARN, // Warning
		ERRR, // Error
		FATL  // Fatal
	};
	
	class Logging
	{
	public:
		using LogQueue = std::deque<std::string>;

		enum LConsoleColour : unsigned short
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

	public:
		Logging();
		Logging(const std::string& filename, const std::string& directory);
		Logging(std::string&& filename, std::string&& directory);

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
		constexpr void SetMinimumLoggingLevel(const LLevel newMinLevel) noexcept;

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
		void AddEntry(const std::string_view msg, const LLevel lvl = LLevel::NORM, const char* file = "", const unsigned line = 0);

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

	private:
		void SetUp();
		
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
		 *		Stores all log levels into a map with the string equivalent
		 */
		static void InitializeLogLevelMap() noexcept;

		/**
		 * \brief
		 *		Outputs log message to sub systems (i.e. console and output debug console)
		 * \param[in] logLine
		 *		STL string_view representing the log message
		 * \param[in] lvl
		 *		Log level
		 */
		void OutputToSubSystems(const std::string_view& logLine, const LLevel lvl) noexcept;

		/**
		 * \brief
		 *		Initialize map of log level to console colour map 
		 */
		static void InitializeOutputToConsoleColourMap();

		/**
		 * \brief
		 *		Outputs logs to file then closes it
		 */
		void CloseLogFile();

	public:
		static const char* kLogs_Empty;

	protected:
		LogQueue logEntryQueue; // Queue buffer to cache the logged messages

		LLevel minimumLoggingLevel;

		std::string directory;
		std::string filename;
		std::fstream logFileStream;

		std::string name;
		bool isEnabled;
		bool subSystemLoggingEnabled;
		bool inCacheMode;
	};
}

