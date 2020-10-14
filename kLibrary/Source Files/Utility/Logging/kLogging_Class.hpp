#pragma once

#include "kLogEntry.hpp"

#include "../../HelperMacros.hpp"

#include <unordered_map>
#include <map>
#include <string>


namespace klib
{
	namespace kLogs
	{
		class iLogger;
		
		class Logging
		{
			enum class LoggerType
			{
				FILE,
				SYSTEM,
			};
			
		public:
			using LogEntries = std::map<std::uint64_t, LogEntry>;
			using BannerEntries = std::map<std::uint64_t, BannerEntry>;

		public:
			Logging(const std::string& directory, const std::string& filename);

			~Logging();

			/**
			 * \brief
			 *		Initializes logging system
			 * \note
			 *		No logging calls will function properly until this is called.
			 */
			void OutputInitialized(const std::string_view& startLog);

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
			void SetMinimumLoggingLevel(const LogEntry::LogLevel newMinLevel) noexcept;

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
			 * \param entry
			 *		Log level type
			 * \param file
			 * \param line
			 */
			void AddEntry(const LogEntry& entry);

			/**
			 * \brief
			 *		Formats the log banner to become the appropriate log banner message then caches it
			 * \param[in] entry
			 *		Log banner title
			 * \param type
			 *		The category/subject of the log banner
			 */
			void AddBanner(const BannerEntry& entry);

			/**
			 * \brief
			 *		Returns previous entry text
			 *
			 * \return
			 *		String of the final log entry
			*/
			std::string_view GetLastCachedEntry();

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
			void Initialize(const std::string& directory, const std::string& filename);

			/**
			 * \brief
			 *		Open up logs
			 */
			void Open();
			
			/**
			 * \brief
			 *		Outputs logs to file then closes it
			 */
			void Close();

			/**
			 * \brief
			 *		Checks log level is above the minimum
			 * \param lvl
			 *		Log level of upcoming entry
			 * \return
			 *		TRUE if equal/above minimum log level
			 */
			bool Loggable(const LogEntry::LogLevel lvl) const;
			
		public:
			static const char* kLogs_Empty;

		protected:
			LogEntries logEntries; // Queue buffer to cache the logged messages
			BannerEntries bannerEntries; // Queue buffer to cache the logged banner messages

			LogEntry::LogLevel minimumLoggingLevel;
			std::unordered_map<LoggerType, std::unique_ptr<iLogger>> loggers;
			std::string name;
			bool isEnabled;
			bool subSystemLoggingEnabled;
			bool inCacheMode;
			bool constantFlushing;


			std::uint64_t logIndex;
		};
	}

#ifdef KLIB_SHORTHAND_NAMESPACE
	using namespace kLogs;
#endif
}
