#pragma once

namespace krakoa::gfx
{
	class iContext
	{
	public:
		virtual ~iContext() noexcept = default;
		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;
	};

}
