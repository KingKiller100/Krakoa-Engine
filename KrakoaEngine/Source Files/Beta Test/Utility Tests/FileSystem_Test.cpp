#include "Precompile.h"
#include "FileSystem_Test.h"

#include "../../Utility/File System/kFileSystem.h"

namespace kTest::utility
{
	using namespace util;
	
	FileSystemTester::FileSystemTester()
		: Tester("File System Test")
	{	}

	FileSystemTester::~FileSystemTester()
		= default;

	void FileSystemTester::Test()
	{
		kFileSystem::CheckFileExists("C:\\Users\\kweku\\Documents\\Krakoa Engine\\bin\\x64\\Debug\\Hooper2\\waitforremotedebugger");
		const auto cwd = kFileSystem::GetCurrentWorkingDirectory<char>();
		kFileSystem::CreateNewDirectories<char>((cwd + "Create Directories Test\\Success1\\Success2\\"));
		kFileSystem::CreateNewDirectory((cwd + "Create Directory Test\\").c_str());
		kFileSystem::DeleteDirectory((cwd + "Create Directories Test\\Success1\\Success2").c_str());
		kFileSystem::OutputToFile((cwd + "Create Directory Test\\Test.txt").c_str(), "Success\n");
		const auto fileInfo = util::kFileSystem::ParseFileData((cwd + "Create Directory Test\\Test.txt").c_str());
	}
}
