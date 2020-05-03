#pragma once

#include "../Testing/Tester.hpp"

#include <string>
#include <unordered_set>

#ifdef TESTING_ENABLED

class SpeedTestBase;

class SpeedTestManager
{
public:
	SpeedTestManager();
	~SpeedTestManager();

	static void Initialize();
	static void ShutDown();
	static void Run();
	static void Add(SpeedTestBase* test);
	static void CollectResult(const std::string_view& result);

	static SpeedTestManager& Get();

	static void InitializeMaths();

private:
	static void OutputResult(const std::string& name);

private:
	static std::unordered_set<SpeedTestBase*> tests;
	static std::string results;
	static std::string directory;
};