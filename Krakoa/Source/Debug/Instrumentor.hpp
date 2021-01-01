#pragma once

#include <Utility/Profiler/kProfiler.hpp>
#include <Utility/FileSystem/kFileSystem.hpp>

#include <algorithm>
#include <fstream>
#include <mutex>
#include <string>

namespace krakoa
{
	using namespace klib::kProfiler;

	class Instrumentor
	{
	public:

		static Instrumentor& Get();

		~Instrumentor();

		void BeginSession(const std::string& name, const std::string& filepath = "results");

		void EndSession();

		void WriteProfile(const ProfilerResult<std::time_t>& result);

	private:
		void WriteHeader();

		void WriteFooter();

	private:
		Instrumentor();

	private:
		std::string     sessionName;
		int             profileCount;
		bool            activeSession;

		std::ofstream   outputStream;
		std::mutex      mutexLock;
	};
}

#ifdef KRAKOA_PROFILE
#define KRK_PROFILE_SESSION_BEGIN(name, filepath) krakoa::Instrumentor::Get().BeginSession(name, filepath)
#define KRK_PROFILE_SESSION_END() krakoa::Instrumentor::Get().EndSession()
#define KRK_PROFILE_SCOPE(name) klib::kProfiler::Profiler<std::time_t> timer##__LINE__(name, [&](const klib::kProfiler::ProfilerResult& result) { krakoa::Instrumentor::Get().WriteProfile(result); })
#define KRK_PROFILE_FUNCTION()  KRK_PROFILE_SCOPE(__FUNCSIG__)
#else
#define KRK_PROFILE_SESSION_BEGIN(name, filepath) 
#define KRK_PROFILE_SESSION_END() 
#define KRK_PROFILE_SCOPE(name) 
#define KRK_PROFILE_FUNCTION()  
#endif

