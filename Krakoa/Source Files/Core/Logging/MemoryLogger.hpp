#pragma once

#include "../../PointerTypes.hpp"

#include <Utility/Logging/kLogging_Class.hpp>

namespace memory
{
	class MemoryLogger
	{
	public:
		static void Init();
		static klib::kLogs::Logging& GetLogger();
		
	private:
		static krakoa::Solo_Ptr<klib::kLogs::Logging> pMemoryLogger;
	};
}


// MEMORY SIDE Log Macros
#define MEM_INIT_LOGS()                                     ::memory::MemoryLogger::Init();
#define MEM_SET_FLUSHING(enable)                            ::memory::MemoryLogger::GetLogger().EnableConstantFlush(enable);
#define MEM_SET LOG_NAME(name)                              ::memory::MemoryLogger::GetLogger().SetName(name);
#define MEM_SET_LOG_MIN(minLvl)                             ::memory::MemoryLogger::GetLogger().SetMinimumLoggingLevel(minLvl);
#define MEM_GET_LOG_PATH()                                  ::memory::MemoryLogger::GetLogger().GetOutputPath();
#define MEM_TOGGLE_LOGGING()                                ::memory::MemoryLogger::GetLogger().ToggleLoggingEnabled();
#define MEM_TOGGLE_SUBSYSTEM_LOGGING()                      ::memory::MemoryLogger::GetLogger().ToggleSubSystemEnabled();
#define MEM_DISABLE_LOGFILE_OUTPUT()                        ::memory::MemoryLogger::GetLogger().SetCacheMode(true);
#define MEM_ENABLE_LOGFILE_OUTPUT()                         ::memory::MemoryLogger::GetLogger().SetCacheMode(false);
#define MEM_CHANGE_LOGS_DESTINATION(destination)            ::memory::MemoryLogger::GetLogger().ChangeOutputDirectory(destination);
#define MEM_CHANGE_LOGS_FILENAME(name)                      ::memory::MemoryLogger::GetLogger().ChangeFilename(name);
#define MEM_DBUG(msg)                                       ::memory::MemoryLogger::GetLogger().AddEntry(msg, ::klib::kLogs::LLevel::DBUG);
#define MEM_NORM(msg)                                       ::memory::MemoryLogger::GetLogger().AddEntry(msg, ::klib::kLogs::LLevel::NORM);
#define MEM_INFO(msg)                                       ::memory::MemoryLogger::GetLogger().AddEntry(msg, ::klib::kLogs::LLevel::INFO);
#define MEM_WARN(msg)                                       ::memory::MemoryLogger::GetLogger().AddEntry(msg, ::klib::kLogs::LLevel::WARN);
#define MEM_ERRR(msg)                                       ::memory::MemoryLogger::GetLogger().AddEntry(msg, ::klib::kLogs::LLevel::ERRR, __FILE__, __LINE__);
#define MEM_BANNER(banner, category)                        ::memory::MemoryLogger::GetLogger().AddEntryBanner(banner, category);
#define MEM_LOG_SUSPEND()                                   ::memory::MemoryLogger::GetLogger().SuspendFileLogging();
#define MEM_LOG_RESUME()                                    ::memory::MemoryLogger::GetLogger().ResumeFileLogging();
#define MEM_LOG_END()                                       ::memory::MemoryLogger::GetLogger().FinalOutput();
#define MEM_LOG_GET_LAST()                                  ::memory::MemoryLogger::GetLogger().GetLastCachedEntry();
#define MEM_LOG_ERASE_PREV(numOfPrevEntries)                ::memory::MemoryLogger::GetLogger().ErasePreviousCacheEntries(numOfPrevEntries);
#define MEM_LOG_CLEAR()                                     ::memory::MemoryLogger::GetLogger().ClearCache();


#ifdef KRAKOA_DEBUG
#	include <Utility/Debug Helper/kAssert.hpp>
#	include <cassert>

#	define MEM_FATAL(condition, msg)                        kAssert((condition), msg);
#	define MEM_ASSERT(condition)                            assert( (condition) );

#else
#	define MEM_FATAL(condition, msg)                        if ( !(condition) ) ::memory::MemoryLogger::GetLogger().OutputToFatalFile(msg, __FILE__, __LINE__);
#endif // !KRAKOA_RELEASE
