#pragma once

#include "iLogDestination.hpp"
#include <filesystem>
#include <fstream>

namespace klib
{
	namespace kLogs
	{
		class LogLevel;

		class FileLogger final : public iLogDestination
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
			void AddEntry(const LogEntry& entry, const LogDescriptor& desc) override;
			void AddBanner(const LogEntry& entry, const LogDescriptor& desc) override;

			bool Open() override;

			bool IsOpen() override;

			void Close(const bool outputClosingMsg) override;

		private:
			void Flush(const std::string_view& msg);

		private:
			std::string name;

			std::string directory;
			std::string filename;
			std::fstream fileStream;
		};
		
	}
}