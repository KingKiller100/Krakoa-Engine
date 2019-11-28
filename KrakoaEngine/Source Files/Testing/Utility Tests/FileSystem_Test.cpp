#include <Precompile.h>
#include <Testing/Utility Tests/FileSystem_Test.hpp>

#include <Utility/File System/kFileSystem.hpp>

namespace kTest::utility
{

	FileSystemTester::FileSystemTester()
		: Tester("File System Test")
	{	}

	FileSystemTester::~FileSystemTester()
		= default;

	void FileSystemTester::Test()
	{
		using namespace util::kFileSystem;
		
		const auto cwd = GetCurrentWorkingDirectory<char>();

		auto content = std::string();
		OutputToFile((cwd + "FS_File_Test").c_str(), content.c_str());

		const auto good = CheckFileExists((cwd + "FS_File_Test").c_str());
		VERIFY(good == true);

		const auto bad = CheckFileExists((cwd + "NULL").c_str());
		VERIFY(bad == false);

		const auto singleCreated = CreateNewDirectory((cwd + "Create Directory Test\\").c_str());
		VERIFY(singleCreated == true);

		const auto multipleCreated = CreateNewDirectories((cwd + "Create Directories Test\\Success1\\Success2\\").c_str());
		VERIFY(multipleCreated == true)

		const auto isDirDeleted = DeleteDirectory((cwd + "Create Directories Test\\Success1\\Success2\\").c_str());
		VERIFY(isDirDeleted == true);

		OutputToFile((cwd + "Create Directory Test\\Test.txt").c_str(), "Success\n");
		VERIFY(CheckFileExists((cwd + "Create Directory Test\\Test.txt").c_str()) == true);

		const auto fileInfo = util::kFileSystem::ParseFileData((cwd + "Create Directory Test\\Test.txt").c_str());
		VERIFY(fileInfo.front() == "Success\n");

		const auto isFileDeleted = RemoveFile((cwd + "Create Directory Test\\Test.txt").c_str());
		VERIFY(isFileDeleted == true);

		VERIFY(RemoveFile((cwd + "FS_File_Test").c_str()) == true);
		VERIFY(DeleteDirectory((cwd + "Change Dir\\").c_str()) == true);
		VERIFY(DeleteDirectory((cwd + "Create Directory Test\\").c_str()) == true);
		VERIFY(DeleteDirectory((cwd + "Create Directories Test\\Success1\\").c_str()) == true);
		VERIFY(DeleteDirectory((cwd + "Create Directories Test\\").c_str()) == true);
	}
}
