#include "Precompile.hpp"
#include "ImGuiUI.hpp"

#include <imgui.h>

namespace krakoa::ui
{
	void Draw(const char* name, const DrawWindowFunc& context)
	{
		ImGui::Begin(name);
		context();
		ImGui::End();
	}

	void Draw(const char* name, WindowFlags flags, const DrawWindowFunc& context)
	{
		ImGui::Begin(name, nullptr, flags);
		context();
		ImGui::End();
	}

	void Draw(const char* name, bool* pOpen, WindowFlags flags, const DrawWindowFunc& context)
	{
		ImGui::Begin(name, pOpen, flags);
		context();
		ImGui::End();
	}

	bool DrawTree(const std::string_view& name, void* id, TreeNodeFlags flags,
	              const InteractAction& openedAction)
	{
		const bool opened = ImGui::TreeNodeEx(id, flags, name.data());
		if (opened)
		{
			openedAction();
			ImGui::TreePop();
		}
		return opened;
	}
}
