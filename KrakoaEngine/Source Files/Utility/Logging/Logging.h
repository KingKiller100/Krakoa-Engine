#pragma once

#include <string>
#include <deque>

namespace krakoa
{
	namespace logs
	{
		enum class LogType : unsigned short
		{
			NORM, // Normal
			INFO, // Informative
			WARN, // Warning
			ERRR, // Error
			FATL  // Fatal
		};
		
		using LogQueue = std::deque<std::string>;
		
		class kLogger
		{
		public:
			kLogger();
			~kLogger();

			void AddEntry(const LogQueue::value_type& msg, const LogType type = LogType::NORM);
			void AddEntryBanner(const LogQueue::value_type& msg, const std::string& category);

			void OutputToErrorFile() const;
			void OutputToFile() const;
			
			const LogQueue::value_type LastEntry();
			void DeleteLastEntries(const size_t amount);
			void Clear() const;

		private:
			void AddToBuffer(LogQueue::value_type& logline);
			
			std::string ResolveLogType(const LogType type);
			std::string ResolveCategoryType(const LogType type);
			
		protected:
			LogQueue logBufferQueue;
			std::string logDirectory;
		};
	}
}
