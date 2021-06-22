#include "MenuBar.hpp"

#include "UI/ImGui/MenuUI.hpp"
#include <Logging/EngineLogger.hpp>

namespace krakoa::panels
{
	void MenuBar::Draw()
	{
		DrawNodes();
	}

	void MenuBar::DrawNodes()
	{
		ui::DrawMenuBar([this] {
			for (auto&& header : menuBarList)
			{
				const auto& title = header.first;
				const auto& commands = header.second;
				ui::DrawMenu(title, [&commands] {
					for (auto&& command : commands)
					{
						ui::DrawMenuItem(command.first, [&]()
						{
							if (command.second)
								command.second();
						});
					}
				});
			}
		});
	}

	void MenuBar::AddTitle(const MenuBarList::key_type& title)
	{
		menuBarList[title];
	}

	void MenuBar::AddOption(const std::string& title, const ui::NamedCommand& option)
	{
		KRK_TRC(util::Fmt("Adding menu command \"{0}\" to option \"{1}\"", option.label, title));
		menuBarList[title][option.label] = option.callback;
	}

}
