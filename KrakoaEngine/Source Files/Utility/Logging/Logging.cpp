#include "Logging.h"

namespace krakoa::logs
{
	const LogQueue::value_type startkLoggingTxt = "*****************************\nLogging Initialized\n********************************\n\n";
	const LogQueue::value_type endkLoggingTxt = "*****************************\nLogging Concluded\n********************************\n\n";
	bool kLoggingInitialized = false;
	
	kLogger::kLogger()
	{
		if (kLoggingInitialized)
			return;

		auto startTxt = startkLoggingTxt;
		AddToBuffer(startTxt);
		kLoggingInitialized = true;
	}

	kLogger::~kLogger()
	= default;

	void kLogger::AddToBuffer(LogQueue::value_type& logline)
	{
		logBufferQueue.push_back(std::move(logline));
	}

	void kLogger::AddEntry(const LogQueue::value_type& msg, const LogType type)
	{
		
	}

}
