#include "Precompile.hpp"
#include "Instrumentor.hpp"

#include <Utility/FileSystem/kFileSystem.hpp>

#include <algorithm>

namespace krakoa
{
	Instrumentor& Instrumentor::Get()
	{
		static Instrumentor instance;
		return instance;
	}

	Instrumentor::~Instrumentor()
	{
		EndSession();
	}

	void Instrumentor::BeginSession(const std::string& name, const std::string& filepath)
	{
		const auto locker = std::unique_lock(mutexLock);

		const auto path = klib::kFileSystem::AppendFileExtension(filepath, "json");
		if (activeSession)
		{
			EndSession();
		}
		activeSession = true;
		outputStream.open(path);
		WriteHeader();
		sessionName = name;
	}

	void Instrumentor::EndSession()
	{
		const auto locker = std::unique_lock(mutexLock);

		if (!activeSession) { return; }
		activeSession = false;
		WriteFooter();
		outputStream.close();
		profileCount = 0;
	}

	void Instrumentor::WriteProfile(const ProfilerResult<std::time_t>& result)
	{
		const auto locker = std::unique_lock(mutexLock);

		if (profileCount++ > 0) { outputStream << ","; }

		std::string name = result.name;
		std::replace(name.begin(), name.end(), '\"', '\'');

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

	void Instrumentor::WriteHeader()
	{
		outputStream << R"({"otherData": {},"traceEvents":[)";
	}

	void Instrumentor::WriteFooter()
	{
		outputStream << "]}";
	}

	Instrumentor::Instrumentor()
		: sessionName("None")
		, profileCount(0)
		, activeSession(false)
	{
	}
}
