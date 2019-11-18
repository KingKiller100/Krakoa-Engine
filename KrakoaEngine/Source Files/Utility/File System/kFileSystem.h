#pragma once

#include <string>
#include <vector>

namespace util
{
	namespace kFileSystem
	{
		//Type aliases for STL containers --------------------------------------------------------
		
		// STL string
		template<class Char>
		using StringType = std::basic_string<Char, std::char_traits<Char>, std::allocator<Char>>;

		// STL string_view
		template<class Char>
		using StringReader = std::basic_string_view<Char>;

		// STL ifstream
		template<class Char>
		using FileReader = std::basic_ifstream<Char, std::char_traits<Char>>;

		// STL ofstream
		template<class Char>
		using FileWriter = std::basic_ofstream<Char, std::char_traits<Char>>;
		// --------------------------------------------------------------------------------------

		
		/**
		 * \brief
		 *		Outputs a file to the specified directory and fills it with the given data
		 * \param fullDirectory
		 *		The full file directory with the final filename and file extension
		 * \param content
		 *		The data to fill the file with.
		 */
		template<class CharT>
		void OutputToFile(const CharT* fullDirectory, const CharT* content)
		{
			FileWriter<CharT> outFile(fullDirectory, std::ios::out | std::ios::app);

			if (outFile.is_open())
			{
				outFile << content;
				outFile.close();
			}
		}

		/**
		 * \brief
		 *		Creates a new folder to the directory, if it does not exist already
		 * \param directory
		 *		Full folder path of the new folder to be created
		 * \return
		 *		Boolean representing whether the directory has been created (TRUE) or not (FALSE)
		 */
		template<class CharT>
		bool CreateNewDirectory(const CharT* directory)
		{
			if _CONSTEXPR_IF(std::is_same_v<CharT, char>)
			{
				return _mkdir(directory) == 0;
			}
			else if _CONSTEXPR_IF(std::is_same_v<CharT, wchar_t>)
			{
				return _wmkdir(directory) == 0;
			}

			return false;
		}

		/**
		 * \brief
		 *		Creates multiple new folder to the directory paths, if the directory does not exist already;
		 *		from the current working directory
		 * \param directory
		 *		Full folder path of the directories to be created
		 * \return
		 *		Boolean representing whether the directories has been created (TRUE) or not (FALSE)
		 *	\note
		 *		The path must be completely unique otherwise the path will not be created. If parts of the
		 *		path already exist, only
		 */
		template<class CharT>
		bool CreateNewDirectories(const StringReader<CharT>& directory)
		{
			if (directory.back() != '\\')
				return false; // Final segment of directory must end with '\\'

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

				isDirCreated = CreateNewDirectory<CharT>(nextDirectory.data());
			}
		}

		/**
		 * \brief
		 *		Deletes a directory
		 * \param directory
		 *		The full folder directory
		 * \return
		 *		Boolean representing whether the directory could be deleted or not
		 * \note
		 *		Please note that this may only work if these two requirements are met:
		 *		- The folder is completely empty (including empty of system and hidden folder files)
		 *		- This directory is not the current directory of this application.
		 */
		template<class CharT>
		bool DeleteDirectory(const CharT* directory)
		{
			if _CONSTEXPR_IF(std::is_same_v<CharT, char>)
			{
				return _rmdir(directory) == 0;
			}
			else if _CONSTEXPR_IF(std::is_same_v<CharT, wchar_t>)
			{
				return _wrmkdir(directory) == 0;
			}

			return false;
		}

		/**
		 * \brief
		 *		Checks (from folder holding the executable file in current directory) if a file exists
		 * \param fileName
		 *		filename (or full directory to the file)
		 * \return
		 *		TRUE if file exist or FALSE if file does not exist
		 */

		template<class CharT>
		bool CheckFileExists(const CharT* fileName)
		{
			FILE* file;
			auto result = -1;

			if _CONSTEXPR_IF(std::is_same_v<CharT, char>)
			{
				result = fopen_s(&file, fileName, "r");
			}
			else if _CONSTEXPR_IF(std::is_same_v<CharT, wchar_t>)
			{
				result = _wfopen_s(&file, fileName, L"r");
			}

			if (file)
			{
				fclose(file);
				delete file;
				file = nullptr;
			}

			return result == 0;
		}

		/**
		 * \brief
		 *		Collects every line of text in the file and returns it
		 * \param fullFilePath
		 *		Full directory of the file you wish to collect data from
		 * \return
		 *		A vector of every line of data in the file, as a string
		 */
		template<class CharT>
		std::vector<StringType<CharT>> ParseFileData(const CharT* fullFilePath)
		{
			std::vector<StringType<CharT>> fileData;

			if (CheckFileExists<CharT>(fullFilePath))
			{
				std::basic_ifstream<CharT, std::char_traits<CharT>> inFile(fullFilePath);

				if (inFile.is_open())
				{
					StringType<CharT> data;
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

		/**
		 * \brief
		 *		Returns the current directory
		 * \return
		 *		Current working directory as a string
		 */
		template<class Char>
		StringType<Char> GetCurrentWorkingDirectory()
		{
			StringType<Char> fullFolderPathOfCurrentWorkingDirectory;

			if (fullFolderPathOfCurrentWorkingDirectory.empty())
			{
				Char cwdBuffer[1024];

				if _CONSTEXPR_IF(std::is_same_v<Char, char>)
					GetModuleFileNameA(nullptr, cwdBuffer, sizeof(cwdBuffer));
				else if _CONSTEXPR_IF(std::is_same_v<Char, wchar_t>)
					GetModuleFileNameW(nullptr, cwdBuffer, sizeof(cwdBuffer));

				fullFolderPathOfCurrentWorkingDirectory = cwdBuffer;

				// Remove the filename, but keep the end slash
				fullFolderPathOfCurrentWorkingDirectory.erase(fullFolderPathOfCurrentWorkingDirectory.find_last_of('\\') + 1, fullFolderPathOfCurrentWorkingDirectory.back());
			}

			return fullFolderPathOfCurrentWorkingDirectory;
		}


	}
}