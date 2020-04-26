#pragma once

#include <Utility/Profiler/kProfiler.hpp>
#include <Utility/File System/kFileSystem.hpp>

#include <algorithm>
#include <chrono>
#include <fstream>
#include <mutex>
#include <string>
#include <thread>

namespace krakoa
{
	using namespace klib::kProfiler;

	class Instrumentor
	{
	public:

		static Instrumentor& Instance()
		{
			static Instrumentor instance;
			return instance;
		}

		~Instrumentor()
		{
			EndSession();
		}

		void BeginSession(const std::string& name, const std::string& filepath = "results")
		{
			const auto path = klib::kFileSystem::AppendFileExtension(filepath, "json");
			if (activeSession) { EndSession(); }
			activeSession = true;
			outputStream.open(path);
			WriteHeader();
			sessionName = name;
		}

		void EndSession()
		{
			if (!activeSession) { return; }
			activeSession = false;
			WriteFooter();
			outputStream.close();
			profileCount = 0;
		}

		void WriteProfile(const ProfilerResult& result)
		{
			std::lock_guard<std::mutex> lock(mutexLock);

			if (profileCount++ > 0) { outputStream << ","; }

			std::string name = result.name;
			std::replace(name.begin(), name.end(), '"', '\'');

			outputStream << "{";
			outputStream << "\"cat\":\"function\",";
			outputStream << "\"dur\":" << (result.end - result.start) << ',';
			outputStream << "\"name\":\"" << name << "\",";
			outputStream << "\"ph\":\"X\",";
			outputStream << "\"pid\":0,";
			outputStream << "\"tid\":" << result.threadID << ",";
			outputStream << "\"ts\":" << result.start;
			outputStream << "}";
		}

		void WriteHeader()
		{
			outputStream << "{\"otherData\": {},\"traceEvents\":[";
		}

		void WriteFooter()
		{
			outputStream << "]}";
		}

	private:
		Instrumentor()
			: sessionName("None"),
			profileCount(0),
			activeSession(false)
		{ }

	private:
		std::string     sessionName;
		int             profileCount;
		bool            activeSession;

		std::ofstream   outputStream;
		std::mutex      mutexLock;
	};

	class InstrumentationTimer
	{
	public:

		InstrumentationTimer(const std::string& name)
			: m_result({ name, 0, 0, 0 })
			, m_stopped(false)
		{
			m_startTimepoint = std::chrono::high_resolution_clock::now();
		}

		~InstrumentationTimer()
		{
			if (!m_stopped)
			{
				Stop();
			}
		}

		void Stop()
		{
			auto endTimepoint = std::chrono::high_resolution_clock::now();

			m_result.start = std::chrono::time_point_cast<std::chrono::microseconds>(m_startTimepoint).time_since_epoch().count();
			m_result.end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();
			m_result.threadID = (uint32_t)std::hash<std::thread::id>{}(std::this_thread::get_id());
			Instrumentor::Instance().WriteProfile(m_result);

			m_stopped = true;
		}

	private:
		ProfilerResult m_result;

		std::chrono::time_point<std::chrono::high_resolution_clock> m_startTimepoint;
		bool m_stopped;
	};
}

#ifdef KRAKOA_PROFILE
#define KRK_PROFILE_SESSION_BEGIN(name, filepath) krakoa::Instrumentor::Instance().BeginSession(name, filepath)
#define KRK_PROFILE_SESSION_END() krakoa::Instrumentor::Instance().EndSession()
#define KRK_PROFILE_SCOPE(name) krakoa::InstrumentationTimer timer##__LINE__(name)
#define KRK_PROFILE_FUNCTION()  KRK_PROFILE_SCOPE(__FUNCSIG__)
#else
#define KRK_PROFILE_SESSION_BEGIN(name, filepath) 
#define KRK_PROFILE_SESSION_END() 
#define KRK_PROFILE_SCOPE(name) 
#define KRK_PROFILE_FUNCTION()  
#endif

