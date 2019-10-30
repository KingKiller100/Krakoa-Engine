#pragma once

#include <string>
#include <vector>

namespace util
{
	namespace kFileSystem
	{
		/**
		 * \brief
		 *		Outputs a file to the specified directory and fills it with the given data
		 * \param fullDirectory
		 *		The full file directory with the final filename and file extension
		 * \param content
		 *		The data to fill the file with.
		 */
		void OutputToFile(const char* fullDirectory, const char* content);

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
		bool CreateNewDirectories(const std::string& directory);

		/**
		 * \brief
		 *		Creates a new folder to the directory, if it does not exist already
		 * \param directory
		 *		Full folder path of the new folder to be created
		 * \return
		 *		Boolean representing whether the directory has been created (TRUE) or not (FALSE)
		 */
		bool CreateNewDirectory(const char* directory);

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
		bool DeleteDirectory(const char* directory);

		/**
		 * \brief
		 *		Checks (from folder holding the executable file in current directory) if a file exists
		 * \param fileName
		 *		filename (or full directory to the file)
		 * \return
		 *		TRUE if file exist or FALSE if file does not exist
		 */
		bool CheckFileExists(const char* fileName);

		/**
		 * \brief
		 *		Collects every line of text in the file and returns it
		 * \param fullFilePath
		 *		Full directory of the file you wish to collect data from
		 * \return
		 *		A vector of every line of data in the file, as a string
		 */
		std::vector<std::string> ParseFileData(const char* fullFilePath);

		/**
		 * \brief
		 *		Returns the current directory
		 * \return
		 *		Current working directory as a string
		 */
		std::string GetCurrentWorkingDirectory();

	}
}