#pragma once


namespace klib::kCalendar
{
	class TimeComponentBase
	{
	protected:
		USE_RESULT virtual bool Verify() const = 0;
		virtual void Limit() = 0;
	};
}