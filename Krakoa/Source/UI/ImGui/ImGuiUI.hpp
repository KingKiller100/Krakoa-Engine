#pragma once

#include "../Flags/TreeNodeFlags.hpp"
#include "../Flags/WindowFlags.hpp"

#include <functional>
#include <string>

namespace krakoa
{
	namespace ui
	{
		using DrawWindowFunc = std::function<void()>;
		using InteractAction = std::function<void()>;

		void Draw(const char* name, const DrawWindowFunc& context);

		void Draw(const char* name, WindowFlags flags, const DrawWindowFunc& context);

		void Draw(const char* name, bool* pOpen, WindowFlags flags, const DrawWindowFunc& context);

		bool DrawTree(void* id, TreeNodeFlags flags, const std::string_view& name
		              , const InteractAction& openedAction);
	}
}

