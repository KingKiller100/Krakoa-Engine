#include "pch.hpp"
#include "FileSystem_Test.hpp"

#include "../../Utility/File System/kFileSystem.hpp"

#ifdef TESTING_ENABLED
namespace kTest::utility
{

	FileSystemTester::FileSystemTester()
		: Tester("File System Test")
	{	}

	FileSystemTester::~FileSystemTester()
		= default;

	void FileSystemTester::Test()
	{
		using namespace klib::kFileSystem;
		
		const auto exeDir = GetExeDirectory<char>();

		const auto cwd = GetCurrentWorkingDirectory<char>();
		const auto w_cwd = GetCurrentWorkingDirectory<wchar_t>();
		const auto cwd16 = GetCurrentWorkingDirectory<char16_t>();
		const auto cwd32 = GetCurrentWorkingDirectory<char32_t>();

		auto content = std::string();
		OutputToFile((exeDir + "FS_File_Test").c_str(), content.c_str());

		const auto good = CheckFileExists((exeDir + "FS_File_Test").c_str());
		VERIFY(good == true);

		const auto bad = CheckFileExists((exeDir + "NULL").c_str());
		VERIFY(bad == false);

		const auto singleCreated = CreateNewDirectory((exeDir + "Create Directory Test\\").c_str());
		VERIFY(singleCreated == true);

		const auto multipleCreated = CreateNewDirectories((exeDir + "Create Directories Test\\Success1\\Success2\\").c_str());
		VERIFY(multipleCreated == true)

		const auto isDirDeleted = DeleteDirectory((exeDir + "Create Directories Test\\Success1\\Success2\\").c_str());
		VERIFY(isDirDeleted == true);

		OutputToFile((exeDir + "Create Directory Test\\Test.txt").c_str(), "Success\n");
		VERIFY(CheckFileExists((exeDir + "Create Directory Test\\Test.txt").c_str()) == true);

		const auto fileData = klib::kFileSystem::ParseFileData((exeDir + "Create Directory Test\\Test.txt").c_str());
		VERIFY(fileData.front().compare("Success") == 0);

		const auto isFileDeleted = RemoveFile((exeDir + "Create Directory Test\\Test.txt").c_str());
		VERIFY(isFileDeleted == true);

		VERIFY(RemoveFile((exeDir + "FS_File_Test").c_str()) == true);
		VERIFY(DeleteDirectory((exeDir + "Create Directory Test\\").c_str()) == true);
		VERIFY(DeleteDirectory((exeDir + "Create Directories Test\\Success1\\").c_str()) == true);
		VERIFY(DeleteDirectory((exeDir + "Create Directories Test\\").c_str()) == true);

		const auto path = exeDir + "Arbitrary Folder Name\\File.txt";
		const auto folders = GetPath(path);
		const auto file = GetFileName(path);
		const auto fileNoExt = GetFileNameWithoutExtension(path);

		VERIFY(folders.compare(exeDir + "Arbitrary Folder Name") == 0);
		VERIFY(file.compare("File.txt") == 0);
		VERIFY(fileNoExt.compare("File") == 0);

		const auto fileTestExt = AppendFileExtension(fileNoExt, ".test");
		VERIFY(fileTestExt.compare("File.test") == 0);
		const auto fileTestExt2 = AppendFileExtension(fileNoExt, "test");
		VERIFY(fileTestExt2.compare("File.test") == 0)
	}
}
#endif
