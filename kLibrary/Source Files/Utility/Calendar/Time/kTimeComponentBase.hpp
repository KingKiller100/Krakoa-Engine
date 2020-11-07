#pragma once


namespace klib::kCalendar
{
	class TimeComponentBase
	{
		TimeComponentBase(const size_t limit);
		virtual ~TimeComponentBase() = default;

		bool Verify();
	protected:
		size_t limit;
	};
}