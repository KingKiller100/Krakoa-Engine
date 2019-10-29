#include "kFileSystem.h"

#include <cstdio>
#include <direct.h>
#include <fstream>
#include <Windows.h>

namespace krakoa::kFileSystem
{
	void OutputToFile(const char* fullDirectory, const char* content)
	{
		std::ofstream outFile(fullDirectory, std::ios::out | std::ios::app);

		if (outFile.is_open())
		{
			outFile << content;
			outFile.close();
		}
	}

	bool CreateNewDirectories(const std::string& directory)
	{
		bool isDirCreated = false;
		auto pos = directory.find_first_of('\\') + 1;

		while (true)
		{
			const auto nextForwardSlash = directory.find_first_of('\\', pos) + 1;
			const auto nextDirectory = directory.substr(0, nextForwardSlash);
			pos = nextForwardSlash;

			if (pos == 0)
			{
				return isDirCreated;
			}

			isDirCreated = CreateNewDirectory(nextDirectory.c_str());
		}
	}

	bool CreateNewDirectory(const char* directory)
	{
		return _mkdir(directory) == 0;
	}

	bool DeleteDirectory(const char* directory)
	{
		return _rmdir(directory) == 0;
	}

	bool CheckFileExists(const char* fileName)
	{
		const std::unique_ptr<FILE> f;
		auto file = f.get();
		fopen_s(&file, fileName, "r");
		if (file)
		{
			fclose(file);
			return true;
		}
		return false;
	}

	std::vector<std::string> ParseFileData(const char* fullFilePath)
	{
		std::vector<std::string> fileData;

		if (CheckFileExists(fullFilePath))
		{
			std::ifstream inFile(fullFilePath);

			if (inFile.is_open())
			{
				std::string data;
				while (!(inFile.eof()))
				{
					std::getline(inFile, data);
					fileData.push_back(data);
				}
				inFile.close();
			}
		}

		return fileData;
	}

	std::string GetCurrentWorkingDirectory()
	{
		static std::string fullFolderPathOfCurrentWorkingDirectory;

		if (fullFolderPathOfCurrentWorkingDirectory.empty())
		{
			char cwdBuffer[1024];
			GetModuleFileNameA(nullptr, cwdBuffer, sizeof(cwdBuffer));

			fullFolderPathOfCurrentWorkingDirectory = cwdBuffer;

			// Remove the filename, but keep the end slash
			fullFolderPathOfCurrentWorkingDirectory.erase(fullFolderPathOfCurrentWorkingDirectory.find_last_of('\\') + 1, fullFolderPathOfCurrentWorkingDirectory.back());
		}

		return fullFolderPathOfCurrentWorkingDirectory;
	}

}

