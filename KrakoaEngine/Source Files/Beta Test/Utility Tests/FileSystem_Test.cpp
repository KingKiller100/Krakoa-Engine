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
		const auto cwd = kFileSystem::GetCurrentWorkingDirectory<char>();
		kFileSystem::CheckFileExists( (cwd + "waitforremotedebugger").c_str() );
		kFileSystem::CreateNewDirectories( (cwd + "Create Directories Test\\Success1\\Success2\\").c_str() );
		kFileSystem::CreateNewDirectory((cwd + "Create Directory Test\\").c_str());
		kFileSystem::DeleteDirectory((cwd + "Create Directories Test\\Success1\\Success2").c_str());
		kFileSystem::OutputToFile((cwd + "Create Directory Test\\Test.txt").c_str(), "Success\n");
		const auto fileInfo = util::kFileSystem::ParseFileData<char>((cwd + "Test.txt").c_str());
	}
}
