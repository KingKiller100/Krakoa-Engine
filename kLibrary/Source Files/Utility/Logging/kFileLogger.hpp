#pragma once

#include "iLogger.hpp"
#include <filesystem>
#include <fstream>

namespace klib
{
	namespace kLogs
	{
		class FileLogger final : public iLogger
		{
		public:
			FileLogger(const std::string_view& newName, const std::string_view& dir, const std::string_view& fName);
			~FileLogger() noexcept;

			USE_RESULT std::string_view GetName() const override;
			void SetName(const std::string_view& newName) override;

			USE_RESULT std::string_view GetFileName() const;
			void SetFileName(const std::string_view& newFilename);

			USE_RESULT std::string_view GetDirectory() const;
			void SetDirectory(const std::string_view& newDir);

			USE_RESULT std::string GetPath() const;
			
			void OutputInitialized(const std::string_view& openingMsg) override;
			void AddEntry(const LogEntry& entry) override;
			void AddBanner(const BannerEntry& entry) override;
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