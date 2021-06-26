#include "MenuBar.hpp"

#include "UI/ImGui/MenuUI.hpp"
#include <Logging/EngineLogger.hpp>

namespace krakoa::panels
{
	MenuBar::~MenuBar()
	{
		commandShortCuts.clear();
		menuBarList.clear();
	}

	void MenuBar::Draw()
	{
		DrawNodes();
	}

	void MenuBar::DrawNodes()
	{
		ui::DrawMenuBar([&] {
			for (auto&& node : menuBarList)
			{
				const auto& title = node.first;
				const auto& commands = node.second;
				ui::DrawMenu(title, [&] {
					for (auto&& command : commands)
					{
						const auto shortCutIter = commandShortCuts.find(command);

						if (shortCutIter != commandShortCuts.end())
						{
							ui::DrawMenuItem(command->label, shortCutIter->second, [&]()
							{
								if (command->callback)
									command->callback();
							});
						}
						else
						{
							ui::DrawMenuItem(command->label, [&]()
							{
								if (command->callback)
									command->callback();
							});
						}
					}
				});
			}
		});
	}

	void MenuBar::AddTitle(const MenuBarList::key_type& title)
	{
		menuBarList[title];
	}

	void MenuBar::AddOption(const std::string& title, const ui::NamedCommand& command)
	{
		KRK_TRC(util::Fmt("Adding menu command \"{0}\" to option \"{1}\"", command.label, title));
		auto& node = menuBarList[title];
		const auto iter = std::find_if(node.begin(), node.end(),
			[&command](const Multi_Ptr<ui::NamedCommand>& cmd)
		{
			return cmd->label == command.label;
		});

		if (iter != node.end())
			return;

		node.emplace_back(Make_Multi<ui::NamedCommand>(command));
	}

	void MenuBar::AddOption(const MenuBarList::key_type& title, const std::string_view& shortcut,
		const ui::NamedCommand& command)
	{
		KRK_TRC(util::Fmt("Adding menu command \"{0}\" to option \"{1}\"", command.label, title));
		auto& node = menuBarList[title];
		const auto iter = std::find_if(node.begin(), node.end(),
			[&command](const Multi_Ptr<ui::NamedCommand>& cmd)
		{
			return cmd->label == command.label;
		});

		if (iter != node.end())
			return;

		const auto& item = node.emplace_back(Make_Multi<ui::NamedCommand>(command));
		commandShortCuts[item] = shortcut.data();
	}
}
