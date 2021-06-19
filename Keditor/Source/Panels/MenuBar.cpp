#include "MenuBar.hpp"

#include "Core/Application.hpp"
#include "UI/ImGui/MenuUI.hpp"

namespace krakoa::panels
{
	void MenuBar::Draw()
	{
		DrawNodes();
	}

	void MenuBar::DrawNodes()
	{
		ui::DrawMenuBar([this] {
			DrawFileNode();
		});
	}

	void MenuBar::DrawFileNode()
	{
		ui::DrawMenu("File", [] {
			ui::DrawMenuItem("Save Scene", [&]()
			{
				
			});

			ui::DrawMenuItem("Exit", [&]()
			{
				GetApp().Close();
			});
		});
	}
}
