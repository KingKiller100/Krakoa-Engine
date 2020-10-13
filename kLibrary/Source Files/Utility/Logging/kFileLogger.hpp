#pragma once

#include "iLogger.hpp"
#include <fstream>

namespace klib
{
	namespace kLogs
	{
		class FileLogger final : public iLogger
		{
		public:
			FileLogger(const std::string& newName, const std::string& dir, const std::string& fName);
			~FileLogger() noexcept;

			USE_RESULT std::string_view GetName() const override;
			void SetName(const std::string_view& newName) override;

			USE_RESULT std::string_view GetFileName() const;
			void SetFileName(const std::string_view& newFilename);

			USE_RESULT std::string_view GetDirectory() const;
			void SetDirectory(const std::string_view& newDir);
			
			void OutputInitialized(const std::string& openingMsg) override;
			void AddEntry(const LogEntry& entry) override;
			void AddEntryBanner(const LogEntry& entry, const std::string_view type, const std::string frontPadding, 
				const std::string& backPadding, size_t paddingCount) override;
			bool Open() override;
			void Close() override;

		private:
			void Flush(const std::string& msg);
			
		private:
			std::string name;

			std::string directory;
			std::string filename;
			std::fstream logFileStream;
		};
		
	}
}