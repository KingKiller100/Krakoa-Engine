#pragma once

namespace krakoa::graphics
{
	class iContext
	{
	public:
		virtual ~iContext() noexcept = default;
		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;
	};

}
