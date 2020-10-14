#pragma once

#include "kLogEntry.hpp"

#include <string>

namespace klib
{
	namespace kLogs
	{

		constexpr char kLogs_Empty[] = "*EMPTY*";


		class iLogger
		{
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
			 *		Formats log message and level to the appropriate log message and then caches it
			 * \param[in] entry
			 *		Log entry containing message, time, date, log level, file and file line
			 */
			virtual void AddEntry(const LogEntry& entry) = 0;

			/**
			 * \brief
			 *		Formats the log banner to become the appropriate log banner message then caches it
			 * \param[in] entry
			 *		Log banner title
			 * \param type
			 *		The category/subject of the log banner
			 * \param frontPadding
			 *		Padding before banner message
			 * \param rearPadding
			 *		Padding after banner message
			 * \param paddingCount
			 *		number of padding elements
			 */
			virtual void AddBanner(const BannerEntry& entry) = 0;


			/**
			 * \brief
			 *		Opens up the log destination
			 */
			virtual bool Open() = 0;
			
			/**
			 * \brief
			 *		Closes the log destination
			 */
			virtual void Close() = 0;

			/**
			 * \brief
			 *		Returns the name of the logger
			 * \return
			 *		Returns name as a string_view
			 */
			USE_RESULT virtual std::string_view GetName() const = 0;

			/**
			 * \brief
			 *		Change name of the logger
			 * \param[in] newName
			 *		STL string representing a name
			 */
			virtual void SetName(const std::string_view& newName) = 0;
		};

	}
#ifdef KLIB_SHORTHAND_NAMESPACE
	using namespace kLogs;
#endif
}
