#pragma once

#include "../Core/PointerTypes.hpp"

#include <Utility/Logging/kLogging.hpp>

namespace memory
{
	class MemoryLogger
	{
	public:
		static void Init(std::string_view openingMsg);
		static klib::kLogs::Logging& GetLogger();
		
	private:
		static krakoa::Solo_Ptr<klib::kLogs::Logging> pMemoryLogger;
	};
}


// MEMORY SIDE Log Macros
#define MEM_INIT_LOGS(opener)                                       ::memory::MemoryLogger::Init(opener);
#define MEM_SET_LOG_NAME(name)                                      ::memory::MemoryLogger::GetLogger().SetName(name);
#define MEM_SET_LOG_MIN(minLvl)                                     ::memory::MemoryLogger::GetLogger().SetMinimumLoggingLevel(minLvl);
#define MEM_GET_LOG_PATH()                                          ::memory::MemoryLogger::GetLogger().GetFile().GetPath();
#define MEM_TOGGLE_LOGGING()                                        ::memory::MemoryLogger::GetLogger().ToggleLoggingEnabled();
#define MEM_TOGGLE_SUBSYSTEM_LOGGING()                              ::memory::MemoryLogger::GetLogger().GetConsole().Close(false);
#define MEM_DISABLE_LOGFILE_OUTPUT()                                ::memory::MemoryLogger::GetLogger().SetCacheMode(true);
#define MEM_ENABLE_LOGFILE_OUTPUT()                                 ::memory::MemoryLogger::GetLogger().SetCacheMode(false);
#define MEM_CHANGE_LOGS_DESTINATION(destination)                    ::memory::MemoryLogger::GetLogger().GetFile().SetPath(destination);
#define MEM_CHANGE_LOGS_FILENAME(name)                              ::memory::MemoryLogger::GetLogger().GetFile().SetFileName(name);
#define MEM_VBAT(msg)                                               ::memory::MemoryLogger::GetLogger().AddRaw(msg);
#define MEM_DBG(msg)                                                ::memory::MemoryLogger::GetLogger().AddEntry(::klib::kLogs::LogLevel::DBG, ::klib::kLogs::LogMessage(msg));
#define MEM_NRM(msg)                                                ::memory::MemoryLogger::GetLogger().AddEntry(::klib::kLogs::LogLevel::NRM, ::klib::kLogs::LogMessage(msg));
#define MEM_INF(msg)                                                ::memory::MemoryLogger::GetLogger().AddEntry(::klib::kLogs::LogLevel::INF, ::klib::kLogs::LogMessage(msg));
#define MEM_WRN(msg)                                                ::memory::MemoryLogger::GetLogger().AddEntry(::klib::kLogs::LogLevel::WRN, ::klib::kLogs::LogMessage(msg));
#define MEM_ERR(msg)                                                ::memory::MemoryLogger::GetLogger().AddEntry(::klib::kLogs::LogLevel::ERR, ::klib::kLogs::LogMessage(msg, SOURCE_INFO()));
#define MEM_BANNER(msg, descriptor, frontPad, backPad, count)       ::memory::MemoryLogger::GetLogger().AddBanner(msg, descriptor, frontPad, backPad, count);
#define MEM_LOG_SUSPEND()                                           ::memory::MemoryLogger::GetLogger().GetFile().Close(false);
#define MEM_LOG_RESUME()                                            ::memory::MemoryLogger::GetLogger().GetFile().Open();
#define MEM_LOG_END()                                               ::memory::MemoryLogger::GetLogger().FinalOutput();
#define MEM_LOG_GET_LAST()                                          ::memory::MemoryLogger::GetLogger().GetLastCachedEntry();
#define MEM_LOG_ERASE_PREV(numOfPrevEntries)                        ::memory::MemoryLogger::GetLogger().ErasePrevious(numOfPrevEntries);
#define MEM_LOG_CLEAR()                                             ::memory::MemoryLogger::GetLogger().ClearCache();

