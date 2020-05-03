#include "pch.hpp"
#include "SpeedTestManager.hpp"

#include "SpeedTestBase.hpp"

#include "../Utility/Format/kFormatToString.hpp"

// File System to output test results
#include "../Utility/File System/kFileSystem.hpp"

#include <filesystem>

SpeedTestManager::SpeedTestManager()
= default;

SpeedTestManager::~SpeedTestManager()
= default;

void SpeedTestManager::Initialize()
{
	using namespace klib;
	directory = kFileSystem::GetExeDirectory<char>() + "Speed Results\\";
	const auto isMade = kFileSystem::CreateNewDirectory(directory.c_str());

	if (!kFileSystem::CheckFileExists(directory))
	{
		throw std::runtime_error("Test Results directory could not be created/found. Please check why!");
	}

	std::filesystem::path startPoint(directory);
	for (auto iter = std::filesystem::directory_iterator(startPoint);
		iter != std::filesystem::directory_iterator();
		++iter)
	{
		const auto dir = *iter;
		const auto isFile = dir.is_regular_file();
		if (isFile)
		{
			const auto currentPath = dir.path().string();
			kFileSystem::RemoveFile(currentPath);
		}
	}
}

void SpeedTestManager::ShutDown()
{
	for (auto& test : tests)
	{
		delete test;
	}
}

void SpeedTestManager::Run()
{
	for (auto& test : tests)
	{
		test->Run();
		OutputResult(test->GetName());
	}
}

void SpeedTestManager::Add(SpeedTestBase * test)
{
	tests.insert(test);
}

void SpeedTestManager::CollectResult(const std::string_view & result)
{
	results.append(result);
}

void SpeedTestManager::InitializeMaths()
{
	//Add(new AlgoirthmsSpeedTest());
}

void SpeedTestManager::OutputResult(const std::string& name)
{
	using namespace klib;
	const auto filename = kFileSystem::AppendFileExtension(name + "_SpeedTest", "txt");
	const auto fullPath = directory + filename;
	kFileSystem::OutputToFile(fullPath, results);
	results.clear();
}

SpeedTestManager& SpeedTestManager::Get()
{
	static SpeedTestManager instance;
	return instance;
}
