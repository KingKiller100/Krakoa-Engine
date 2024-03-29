﻿#pragma once

#include <Utility/Enum/kEnum.hpp>

namespace krakoa::scene
{
	class SceneConstants
	{
	protected:
		ENUM_STRUCT(SceneRuntimeState, int,
			RUNNING
			, PLAY = RUNNING
			, PAUSE
			, STOP
		);
	};
}