#pragma once

#include <Core/PointerTypes.hpp>
#include <UI/NamedCommand.hpp>
#include <vector>

namespace krakoa::panels
{
	class MenuBar
	{
		using MenuBarList = std::unordered_map<std::string,
			std::vector<Multi_Ptr<ui::NamedCommand>>>;
	public:
		MenuBar() noexcept = default;
		~MenuBar();

		void Draw();

		void AddTitle(const MenuBarList::key_type& title);
		void AddOption(const MenuBarList::key_type& title, const ui::NamedCommand& command);
		void AddOption(const MenuBarList::key_type& title, const std::string_view& shortcut, const ui::NamedCommand& command);

	private:
		void DrawNodes();

	private:
		MenuBarList menuBarList;
		std::unordered_map<Multi_Ptr<ui::NamedCommand>, std::string> commandShortCuts;
	};
}
