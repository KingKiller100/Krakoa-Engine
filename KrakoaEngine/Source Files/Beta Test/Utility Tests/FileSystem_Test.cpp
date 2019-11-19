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

	bool FileSystemTester::Test()
	{
		const auto cwd = kFileSystem::GetCurrentWorkingDirectory<char>();
		auto good = kFileSystem::CheckFileExists( (cwd + "waitforremotedebugger").c_str() );
		auto bad = kFileSystem::CheckFileExists( (cwd + "NULL").c_str() );
		auto multipleCreated = kFileSystem::CreateNewDirectories( (cwd + "Create Directories Test\\Success1\\Success2\\").c_str() );
		auto singleCreated = kFileSystem::CreateNewDirectory((cwd + "Create Directory Test\\").c_str());
		auto isDeleted = kFileSystem::DeleteDirectory((cwd + "Create Directories Test\\Success1\\Success2").c_str());
		kFileSystem::OutputToFile((cwd + "Create Directory Test\\Test.txt").c_str(), "Success\n");
		const auto fileInfo = util::kFileSystem::ParseFileData((cwd + "Create Directory Test\\Test.txt").c_str());

		return true;
	}
}
