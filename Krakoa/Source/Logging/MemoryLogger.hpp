#pragma once

#include "LoggerCore.hpp"
#include "../Core/EngineMacros.hpp"
#include <Utility/Logging/kLogging.hpp>
#include <Utility/Debug/Source/kSourceInfo.hpp>


namespace memory
{
	class KRAKOA_API MemoryLogger
	{
	public:
		static void Init(std::string_view openingMsg);
		static krakoa::LogProfile RegisterProfile(std::string_view name);
		
		static void Raw(std::string_view message);

		static void Trace(std::string_view message);
		static void Debug(std::string_view message);
		static void Info(std::string_view message);
		static void Warn(std::string_view message);
		static void Error(std::string_view message, klib::SourceInfo sourceInfo);

		static void Banner(
			std::string_view message
			, std::string_view frontPadding
			, std::string_view backPadding
			, std::uint16_t paddingCount
		);

	private:
		static klib::LogDispatcher dispatcher_;
	};
}

// MEMORY Log Macros

#ifdef KRAKOA_DEBUG
#	define MEM_INIT_LOGS(msg)                                          ::memory::MemoryLogger::Init(msg)
#	define MEM_LOG_RAW(msg)                                            ::memory::MemoryLogger::Raw(msg)
#	define MEM_TRC(msg)                                                ::memory::MemoryLogger::Trace(msg)
#	define MEM_DBG(msg)                                                ::memory::MemoryLogger::Debug(msg)
#	define MEM_INF(msg)                                                ::memory::MemoryLogger::Info(msg)
#	define MEM_WRN(msg)                                                ::memory::MemoryLogger::Warn(msg)
#	define MEM_ERR(msg)                                                ::memory::MemoryLogger::Error(msg, SOURCE_INFO())
#	define MEM_BANNER(msg, fPad, bPad, padCount)                       ::memory::MemoryLogger::Banner(msg, fPad, bPad, padCount)
#else
#	define MEM_INIT_LOGS(msg)                            
#	define MEM_LOG_RAW(msg)                              
#	define MEM_TRC(msg)                                  
#	define MEM_DBG(msg)                                  
#	define MEM_INF(msg)                                  
#	define MEM_WRN(msg)                                  
#	define MEM_ERR(msg)                                  
#	define MEM_BANNER(msg, fPad, bPad, padCount)         
#endif
