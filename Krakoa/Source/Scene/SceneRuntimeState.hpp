#pragma once

#include <Utility/Enum/kEnum.hpp>

namespace krakoa::scene
{
	ENUM_STRUCT(SceneRuntimeState, int,
		RUNNING
		, PLAY = RUNNING
		, PAUSE
		, STOP
	);
}