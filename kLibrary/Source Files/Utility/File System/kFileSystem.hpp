#pragma once

#include "../../HelperMacros.hpp"

#include "../String/kStringManipulation.hpp"
#include "../String/kUTFStringConverter.hpp"

#include <direct.h>
#include <corecrt_wdirect.h>
#include <cstdio>
#include <fstream>
#include <string>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#	include <Windows.h>
#undef WIN32_LEAN_AND_MEAN

namespace klib::kFileSystem
{
	//Type aliases for STL containers --------------------------------------------------------

	// STL basic_ifstream
	template<class Char>
	using FileReader = std::basic_ifstream<ONLY_TYPE(Char), std::char_traits<ONLY_TYPE(Char)>>;

	// STL basic_ofstream
	template<class Char>
	using FileWriter = std::basic_ofstream<ONLY_TYPE(Char), std::char_traits<ONLY_TYPE(Char)>>;

	// STL vector of StringWriters
	template<typename Char>
	using FileLines = std::vector<kString::StringWriter<Char>>;

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
	constexpr void OutputToFile(const kString::StringWriter<CharType>& fullFilePath, const kString::StringWriter<CharType>& content, std::ios::openmode mode = std::ios::out | std::ios::app)
	{
		FileWriter<CharType> outFile(fullFilePath.data(), mode);

		if (outFile.is_open())
		{
			outFile << content.data();
			outFile.close();
		}
#ifdef _DEBUG
		else
		{
			if _CONSTEXPR_IF(std::is_same_v<CharType, char>)
			{
				const auto failMsg = kString::StringWriter<CharType>("Cannot create/open file ") + fullFilePath.data();
				OutputDebugStringA(failMsg.c_str());
			}
			else if _CONSTEXPR_IF(std::is_same_v<CharType, wchar_t>)
			{
				const auto failMsg = kString::StringWriter<CharType>(L"Cannot create/open file ") + fullFilePath.data();
				OutputDebugStringW(failMsg.c_str());
			}
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
	constexpr bool CreateNewDirectory(const kString::StringReader<CharType>& directory) noexcept
	{
		if _CONSTEXPR_IF(std::is_same_v<CharType, char>)
		{
			return _mkdir(directory.data()) == 0; // 0 == SUCCESS
		}
		else if _CONSTEXPR_IF(std::is_same_v<CharType, wchar_t>)
		{
			return _wmkdir(directory.data()) == 0; // 0 == SUCCESS
		}
		else
		{
			kString::StringWriter<wchar_t> temp;
			for (auto& c : directory)
				temp += CAST(wchar_t, c);
			return CreateNewDirectory<wchar_t>(temp);
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
	constexpr bool CreateNewDirectories(const kString::StringReader<CharType>& directory)
	{
		using Char = ONLY_TYPE(CharType);

		kString::StringWriter<Char> dir(directory);

		if (dir.back() != Char('\\'))
			dir += Char('\\'); // Final suffix of directory char type must end with '\\'

		bool isDirCreated = false;
		auto pos = dir.find_first_of(Char('\\')) + 1;

		kString::StringWriter<Char> nextDirectory;

		while (pos != 0)
		{
			isDirCreated = CreateNewDirectory<Char>(nextDirectory.c_str());

			const auto nextForwardSlash = dir.find_first_of(Char('\\'), pos) + 1;
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
	constexpr bool RemoveFile(const kString::StringReader<CharType>& fullFilePath)
	{
		if _CONSTEXPR_IF(std::is_same_v<CharType, char>)
		{
			return remove(fullFilePath.data()) == 0; // 0 == SUCCESS
		}
		else if _CONSTEXPR_IF(std::is_same_v<CharType, wchar_t>)
		{
			return _wremove(fullFilePath.data()) == 0; // 0 == SUCCESS
		}
		else
		{
			kString::StringWriter<wchar_t> temp;
			for (auto& c : fullFilePath)
				temp += CAST(char, c);
			return RemoveFile<char>(temp);
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
	bool DeleteDirectory(const kString::StringReader<CharType>& directory)
	{
		if _CONSTEXPR_IF(std::is_same_v<CharType, char>)
		{
			return _rmdir(directory.data()) == 0; // 0 == SUCCESS
		}
		else if _CONSTEXPR_IF(std::is_same_v<CharType, wchar_t>)
		{
			return _wrmkdir(directory.data()) == 0; // 0 == SUCCESS
		}
		else
		{
			kString::StringWriter<char> temp;
			for (auto& c : directory)
				temp += CAST(char, c);
			return DeleteDirectory<char>(temp);
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
	constexpr bool CheckFileExists(const kString::StringReader<CharType>& fullFilePath) noexcept
	{
		FILE* file;
		auto result = -1;

		if _CONSTEXPR_IF(std::is_same_v<CharType, char>)
		{
			result = fopen_s(&file, fullFilePath.data(), "r");
		}
		else if _CONSTEXPR_IF(std::is_same_v<CharType, wchar_t>)
		{
			result = _wfopen_s(&file, fullFilePath.data(), L"r");
		}
		else
		{
			kString::StringWriter<char> temp;
			for (auto& c : fullFilePath)
				temp += CAST(char, c);
			result = CheckFileExists<char>(temp);
		}

		if (file)
			fclose(file);

		return result == 0;
	}

	/**
	 * \brief
	 *		Checks (from folder holding the executable file in current directory) if a directory exists
	 * \param directoryPath
	 *		folder (or full directory)
	 * \return
	 *		TRUE if it exists or FALSE if it does not exist
	 */

	template<class CharType = char>
	constexpr bool CheckDirectoryExists(const kString::StringReader<CharType>& directoryPath) noexcept
	{
		if _CONSTEXPR_IF(std::is_same_v<CharType, char>)
		{
			struct stat info;
			const auto statResult = stat(directoryPath.data(), &info);

			if (statResult != 0)
				return false;

			const auto result = (info.st_mode & S_IFDIR);
			return result != 0;
		}
		else
		{
			const auto temp = kString::Convert<char>(directoryPath);
			return CheckDirectoryExists<char>(temp.data());
		}
	}

	/**
	 * \brief
	 *		Collects every line of text in the file and returns it
	 * \param fullFilePath
	 *		Full directory of the file you wish to collect data from
	 * \return
	 *		A vector of every line of data in the file, as a string
	 */
	template<class CharType = char>
	USE_RESULT constexpr auto ParseFileData(const kString::StringReader<CharType>& fullFilePath)
	{
		using Char = ONLY_TYPE(CharType);

		FileLines<Char> fileData;

		if (!CheckFileExists<Char>(fullFilePath))
			return fileData;

		FileReader<CharType> inFile(fullFilePath.data());

		if (!inFile.is_open())
			return fileData;

		kString::StringWriter<Char> data;
		while (std::getline(inFile, data))
		{
			fileData.push_back(data);
		}
		inFile.close();

		return fileData;
	}

	/**
	 * \brief
	 *		Returns the current working directory
	 * \return
	 *		Current working directory as a string
	 */
	template<class CharType = char>
	USE_RESULT constexpr kString::StringWriter<CharType>& GetCurrentWorkingDirectory()
	{
		using Char = ONLY_TYPE(CharType);

		static kString::StringWriter<Char> cwdFullPath;

		if (cwdFullPath.empty())
		{
			Char* cwdBuffer;
			DWORD length = 0;
			if _CONSTEXPR_IF(std::is_same_v<Char, char>)
			{
				length = ::GetCurrentDirectoryA(0, nullptr);
				cwdBuffer = new Char[length]{};
				::GetCurrentDirectoryA(length, cwdBuffer);
			}
			else if _CONSTEXPR_IF(std::is_same_v<Char, wchar_t>)
			{
				length = ::GetCurrentDirectoryW(0, nullptr);
				cwdBuffer = new Char[length]{};
				::GetCurrentDirectoryW(length, cwdBuffer);
			}
			else
			{
				const auto dummyBuffer = GetCurrentWorkingDirectory<char>();
				for (auto& c : dummyBuffer)
					cwdFullPath += c;
				return cwdFullPath;
			}

			cwdFullPath = kString::StringWriter<Char>(cwdBuffer, cwdBuffer + (length - 1));
			cwdFullPath += (Char('\\'));

			delete[] cwdBuffer;
		}

		return cwdFullPath;
	}

	/**
	 * \brief
	 *		Returns the current exe directory
	 * \return
	 *		Current working directory as a string
	 */
	template<class CharType = char>
	USE_RESULT constexpr kString::StringWriter<CharType>& GetExeDirectory()
	{
		using Char = ONLY_TYPE(CharType);

		static kString::StringWriter<Char> exeFullPath;

		if (exeFullPath.empty())
		{
			DWORD length = 0;
			const auto bufferSize = 1024;
			Char* exeBuffer = new Char[bufferSize]{};

			if _CONSTEXPR_IF(std::is_same_v<Char, char>)
			{
				length = ::GetModuleFileNameA(nullptr, exeBuffer, bufferSize);
			}
			else if _CONSTEXPR_IF(std::is_same_v<Char, wchar_t>)
			{
				length = ::GetModuleFileNameW(nullptr, exeBuffer, bufferSize);
			}
			else
			{
				const auto dummyBuffer = GetExeDirectory<char>();
				for (auto& c : dummyBuffer)
					exeFullPath += c;
				return exeFullPath;
			}

			exeFullPath = kString::StringWriter<Char>(exeBuffer, exeBuffer + length);
			exeFullPath.erase(exeFullPath.find_last_of(Char('\\')) + 1);

			delete[] exeBuffer;
		}

		return exeFullPath;
	}

	template<class CharType = char>
	USE_RESULT constexpr kString::StringWriter<CharType> GetFileName(const kString::StringWriter<CharType>& path) noexcept
	{
		using Char = std::decay_t<std::remove_pointer_t<CharType>>;
		const auto text = kString::Replace<Char>(path, Char('/'), Char('\\'));
		const auto filename = text.substr(text.find_last_of(Char('\\')) + 1);
		return filename;
	}

	template<class Char = char>
	USE_RESULT constexpr kString::StringWriter<Char> GetFileNameWithoutExtension(const kString::StringWriter<Char>& path) noexcept
	{
		kString::StringWriter<Char> filename = GetFileName<ONLY_TYPE(Char)>(path);
		filename = filename.erase(filename.find_first_of('.'));
		return filename;
	}

	template<class CharType = char>
	USE_RESULT constexpr kString::StringWriter<CharType> GetPath(const kString::StringWriter<CharType>& path)
	{
		using Char = ONLY_TYPE(CharType);
		auto parentPath = kString::Replace<ONLY_TYPE(Char)>(path, Char('/'), Char('\\'));
		parentPath = parentPath.substr(0, parentPath.find_last_of('\\'));
		return parentPath;
	}

	template<class Char = char>
	USE_RESULT constexpr kString::StringWriter<Char> AppendFileExtension(const kString::StringReader<Char>& fname, const kString::StringReader<Char>& extension) noexcept
	{
		using StrW = kString::StringWriter<Char>;
		using StrR = kString::StringReader<Char>;

		StrR filename = fname;
		const auto isDotAtStartOFExtension = extension[0] == '.';

		const auto dotPos = filename.find_first_of('.');
		if (dotPos != StrR::npos) // Dot is in the filename
		{
			const auto extPos = filename.find(extension);

			if (extPos != StrR::npos) // Extension is present
			{
				if (dotPos <= extPos) // Extension is present after the dot
				{
					return filename.data(); // Assume it's all good
				}
			}

			StrW appendedFilename = filename.data();
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

		StrW newFilename = filename.data();
		if (!isDotAtStartOFExtension)
			newFilename += '.';
		newFilename += extension;

		return newFilename;
	}
}


