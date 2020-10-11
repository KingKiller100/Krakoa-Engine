#pragma once

#include <deque>
#include <string>

namespace klib
{
	namespace kLogs
	{
		enum class LogLevel : unsigned short
		{
			BANR, // Log Banner
			DBUG, // Debug
			NORM, // Normal
			INFO, // Informative
			WARN, // Warning
			ERRR, // Error
			FATL  // Fatal
		};

		constexpr char kLogs_Empty[] = "*EMPTY*";


		class iLogger
		{
		protected:
			using LogQueue = std::deque<std::string>;
			
		public:
			virtual ~iLogger() = default;

			/**
			 * \brief
			 *		Initializes logging system
			 * \param openingMsg
			 *		LogEntry
			 * \note
			 *		No logging calls will function properly until this is called.
			 */
			virtual void OutputInitialized(const std::string_view& openingMsg) = 0;

			/**
			 * \brief
			 *		Flush stored log stream to file
			 */
			virtual void Flush() = 0;

			/**
			 * \brief
			 *		Formats log message and level to the appropriate log message and then caches it
			 * \param[in] msg
			 *		Log message
			 * \param lvl
			 *		Log level type
			 * \param file
			 *		Source file
			 * \param line
			 *		Source file line number
			 */
			virtual void AddEntry(const std::string_view& msg, const LogLevel lvl,
				const char* file, const unsigned line) = 0;

			/**
			 * \brief
			 *		Formats the log banner to become the appropriate log banner message then caches it
			 * \param[in] msg
			 *		Log banner title
			 * \param type
			 *		The category/subject of the log banner
			 */
			virtual void AddEntryBanner(const std::string_view msg, const std::string_view type) = 0;


			/**
			 * \brief
			 *		Prepares logs to 
			 */
			virtual void Open() = 0;
			virtual void Close() = 0;

			/**
			 * \brief
			 *		Change name of the logger
			 * \param[in] newName
			 *		STL string representing a name
			 */
			virtual void SetName(const std::string_view& newName) = 0;
			virtual std::string_view SetName() = 0;
		};

	}
#ifdef KLIB_SHORTHAND_NAMESPACE
	using namespace kLogs;
#endif
}
