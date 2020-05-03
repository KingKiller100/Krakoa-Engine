#include "pch.hpp"
#include "SpeedTestBase.hpp"

#include "SpeedTestManager.hpp"

#include "../Utility/Format/kFormatToString.hpp"
#include <iostream>

#ifdef TESTING_ENABLED
SpeedTestBase::SpeedTestBase(const char* name) noexcept
	: name(name)
{
}

SpeedTestBase::~SpeedTestBase()
= default;

void SpeedTestBase::Run() noexcept
{
	try
	{
		Test();
	}
	catch (...)
	{
		const auto output = klib::kFormat::ToString("{0} seems to have crashed!\n", name);
		SpeedTestManager::Get().CollectResult(output);
	}
}

void SpeedTestBase::SendResult(const std::string_view & result) noexcept
{
	const auto output = klib::kFormat::ToString("{0} is the winner!\n", result);
	SpeedTestManager::Get().CollectResult(output);
}

const std::string& SpeedTestBase::GetName() const
{
	return name;
}
