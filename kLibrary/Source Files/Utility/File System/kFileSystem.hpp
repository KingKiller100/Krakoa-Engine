#pragma once

#include <HelperMacros.hpp>

#include <Utility/Format/StringManipulation.h>
#include <direct.h>
#include <corecrt_wdirect.h>
#include <cstdio>
#include <fstream>
#include <string>
#include <vector>

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#include <Windows.h>

namespace klib
{
	namespace kFileSystem
	{
		//Type aliases for STL containers --------------------------------------------------------
		
		// STL basic_string
		template<class Char>
		using StringWriter = std::basic_string<Char, std::char_traits<Char>, std::allocator<Char>>;

		template<class Char>
		using StringReader = std::basic_string_view<Char>;

		// STL basic_ifstream
		template<class Char>
		using FileReader = std::basic_ifstream<Char, std::char_traits<Char>>;

		// STL basic_ofstream
		template<class Char>
		using FileWriter = std::basic_ofstream<Char, std::char_traits<Char>>;

		// STL vector of StringTypes
		template<typename Char>
		using FileLinesData = std::vector<StringWriter<Char>>;
		// --------------------------------------------------------------------------------------

		
		/**
		 * \brief
		 *		Outputs a file to the specified directory and fills it with the given data
		 * \param fullFilePath
		 *		The full file directory with the final filename and file extension
		 * \param content
		 *		The data to fill the file with.
		 */
		template<class CharType = char>
		void OutputToFile(const CharType* fullFilePath, const CharType* content)
		{
			FileWriter<CharType> outFile(fullFilePath, std::ios::out | std::ios::app);

			if (outFile.is_open())
			{
				outFile << content;
				outFile.close();
			}
#ifdef _DEBUG
			else
			{
				auto dirStr = StringReader<CharType>(fullFilePath);
				OutputDebugStringA((StringWriter<CharType>("Cannot open file ") + dirStr.substr(dirStr.find_last_of('\\')).data()).c_str());
			}
#endif // DEBUG
		}

		/**
		 * \brief
		 *		Creates a new folder to the directory, if it does not exist already
		 * \param directory
		 *		Full folder path of the new folder to be created
		 * \return
		 *		Boolean representing whether the directory has been created (TRUE) or not (FALSE)
		 */
		template<class CharType = char>
		bool CreateNewDirectory(const CharType* directory)
		{
			if _CONSTEXPR_IF(std::is_same_v<CharType, char>)
			{
				return _mkdir(directory) == 0;
			}
			else if _CONSTEXPR_IF(std::is_same_v<CharType, wchar_t>)
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
		template<class CharType = char>
		bool CreateNewDirectories(const CharType* directory)
		{
			StringWriter<CharType> dir(directory);
			
			if (dir.back() != '\\')
				return false; // Final suffix of directory char type must end with '\\'

			bool isDirCreated = false;
			auto pos = dir.find_first_of('\\') + 1;

			StringWriter<CharType> nextDirectory;

			while (pos != 0)
			{
				isDirCreated = CreateNewDirectory<CharType>(nextDirectory.c_str());

				const auto nextForwardSlash = dir.find_first_of('\\', pos) + 1;
				nextDirectory = dir.substr(0, nextForwardSlash);
				pos = nextForwardSlash;
			}

			return isDirCreated;
		}

	
		/**
	 * \brief
	 *		Deletes file from system
	 * \tparam CharType
	 *		Char type i.e. must be char/wchar_t/u32char_t/etc...
	 * \param[in] fullFilePath
	 *		Full file path of the file to delete
	 * \return
	 *		TRUE if file is found and deleted, else FALSE if file cannot be found or deleted
	 */
		template<typename CharType = char>
		bool RemoveFile(const CharType* fullFilePath)
		{

			if constexpr (std::is_same_v<CharType, char>)
			if _CONSTEXPR_IF(std::is_same_v<CharType, char>)
			{
				return DeleteFileA(fullFilePath) == IS_TRUE; // 1 == TRUE
			}
			else if _CONSTEXPR_IF(std::is_same_v<CharType, wchar_t>)
			{
				return DeleteFileW(fullFilePath) == IS_TRUE; // 1 == TRUE
			}

			return false;
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
		template<class CharType = char>
		bool DeleteDirectory(const CharType* directory)
		{
			if _CONSTEXPR_IF(std::is_same_v<CharType, char>)
			{
				return _rmdir(directory) == 0;
			}
			else if _CONSTEXPR_IF(std::is_same_v<CharType, wchar_t>)
			{
				return _wrmkdir(directory) == 0;
			}

			return false;
		}

		/**
		 * \brief
		 *		Checks (from folder holding the executable file in current directory) if a file exists
		 * \param fullFilePath
		 *		filename (or full directory to the file)
		 * \return
		 *		TRUE if file exist or FALSE if file does not exist
		 */

		template<class CharType = char>
		bool CheckFileExists(const CharType* fullFilePath)
		{
			const std::unique_ptr<FILE> f;
			auto file = f.get();
			auto result = -1;

			if _CONSTEXPR_IF(std::is_same_v<CharType, char>)
			{
				result = fopen_s(&file, fullFilePath, "r");
			}
			else if _CONSTEXPR_IF(std::is_same_v<CharType, wchar_t>)
			{
				result = _wfopen_s(&file, fullFilePath, L"r");
			}

			if (file)			
				fclose(file);
			
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
		template<class CharType>
		USE_RESULT inline auto ParseFileData(const CharType* fullFilePath)
		{
			FileLinesData<CharType> fileData;

			if (CheckFileExists(fullFilePath))
			{
				FileReader<CharType> inFile;
				
				inFile.open(fullFilePath);

				if (inFile.is_open())
				{
					StringWriter<CharType> data;
					while (std::getline(inFile, data))
					{
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
		template<class Char = char>
		USE_RESULT StringWriter<Char> GetCurrentWorkingDirectory()
		{
			static StringWriter<Char> fullFolderPathOfCurrentWorkingDirectory;

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

		template<class Char = char>
		USE_RESULT StringWriter<Char> GetFileName(const StringWriter<Char>& path) noexcept
		{
			const std::string text = String::Replace(path, '/', '\\');
			const auto filename = text.substr(text.find_last_of('\\'));
			return filename;
		}

		template<class Char = char>
		USE_RESULT StringWriter<Char> GetFileNameWithoutExtension(const StringWriter<Char>& path) noexcept
		{
			StringWriter<Char> filename = GetFileName<Char>(path);
			filename = filename.substr(0, filename.find_first_of('.'));
			return filename;
		}

		template<class Char = char>
		USE_RESULT StringWriter<Char> GetPath(const StringWriter<Char>& path)
		{
			StringWriter<Char> parentPath = String::Replace(path, '/', '\\');
			parentPath = parentPath.substr(0, parentPath.find_last_of('\\'));
			return parentPath;
		}

		template<class Char = char>
		USE_RESULT StringWriter<Char> AppendFileExtension(const Char* fname, const Char* extension) noexcept
		{
			StringReader<Char> filename = fname;
			const auto isDotAtStartOFExtension = extension[0] == '.';

			const auto dotPos = filename.find_first_of('.');
			if (dotPos != StringReader<Char>::npos) // Dot is in the filename
			{
				const auto extPos = filename.find(extension);

				if (extPos != StringReader<Char>::npos) // Extension is present
				{
					if (dotPos <= extPos) // Extension is present after the dot
					{
						return filename.data(); // Assume it's all good
					}
				}

				StringWriter<Char> appendedFilename = filename.data();
				if (dotPos == filename.length() - 1)
				{
					appendedFilename += extension;
				}
				else
				{
					if (!isDotAtStartOFExtension)
						appendedFilename += '.';
					appendedFilename += extension;
				}

				return appendedFilename;
			}

			StringWriter<Char> newFilename = filename.data();
			if (!isDotAtStartOFExtension)
				newFilename += '.';
			newFilename += extension;

			return newFilename;
		}
	}
}