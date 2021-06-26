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
			for (auto&& node : menuBarList)
			{
				const auto& title = node.first;
				const auto& commands = node.second;
				ui::DrawMenu(title, [&commands] {
					for (auto&& command : commands)
					{
						ui::DrawMenuItem(command.label, [&]()
						{
							if (command.callback)
								command.callback();
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
		auto& node = menuBarList[title];
		const auto iter = std::find_if(node.begin(), node.end(),
			[&option](const ui::NamedCommand& command)
		{
			return command.label == option.label;
		});

		if (iter != node.end())
			return;

		node.emplace_back(option);
	}

}
