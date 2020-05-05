#include "pch.hpp"
#include "PerformanceTestManager.hpp"

#include "PerformanceTestBase.hpp"

#include "Maths/Algorithms_SpeedTest.hpp"

// File System to output test results
#include "../../Utility/File System/kFileSystem.hpp"

#include <filesystem>
#include <iostream>


#ifdef TESTING_ENABLED
namespace kTest::performance
{
	PerformanceTestManager::PerformanceTestManager(const Token&)
		: Tester("Performance Tests")
	{}

	PerformanceTestManager::~PerformanceTestManager()
		= default;

	void PerformanceTestManager::Initialize()
	{
		using namespace klib;
		directory = kFileSystem::GetExeDirectory<char>() + "Speed Results\\";
		const auto isMade = kFileSystem::CreateNewDirectory(directory.c_str());

		if (!kFileSystem::CheckDirectoryExists(directory))
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

		InitializeMaths();
	}

	void PerformanceTestManager::ShutDown()
	{
		ClearAll();
	}

	void PerformanceTestManager::RunAll()
	{
		for (auto& test : tests)
		{
			RunTest(test);
			OutputResult(test->GetName());
		}
	}

	void PerformanceTestManager::RunTest(PerformanceTestBase * test)
	{
		std::cout << "\tNow Testing: " << test->GetName() << "\n";
		test->Run();
	}

	void PerformanceTestManager::Add(PerformanceTestBase* test)
	{
		tests.insert(test);
	}

	void PerformanceTestManager::ClearAll()
	{
		for (auto& test : tests)
		{
			delete test;
		}
		tests.clear();
	}

	void PerformanceTestManager::CollectResult(const std::string_view& result)
	{
		results.append(result);
	}

	void PerformanceTestManager::InitializeMaths()
	{
		Add(new maths::AlgorithmsSpeedTest());
	}

	void PerformanceTestManager::OutputResult(const std::string& name)
	{
		using namespace klib;
		const auto filename = kFileSystem::AppendFileExtension(name, "txt");
		const auto fullPath = directory + filename;
		kFileSystem::OutputToFile(fullPath, results);
		results.clear();
	}

	void PerformanceTestManager::Test()
	{
		Initialize();
		RunAll();
		ShutDown();
	}

	PerformanceTestManager& PerformanceTestManager::Get()
	{
		static Token t;
		static PerformanceTestManager instance(t);

		return instance;
	}
}
#endif
