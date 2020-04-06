#pragma once

#include "../../Core/EngineCore.hpp"

#include <any>

namespace krakoa::graphics
{
	class iContext
	{
	public:
		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;
	};

}
