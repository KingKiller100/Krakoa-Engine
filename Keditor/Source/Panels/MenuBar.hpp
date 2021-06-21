#pragma once

#include <UI/NamedCommand.hpp>

namespace krakoa::panels
{
	class MenuBar
	{
		using MenuBarList = std::unordered_map<std::string,
		std::unordered_map<std::string, std::function<void()>>>;
	public:
		MenuBar() noexcept = default;
		~MenuBar() = default;

		void Draw();

		void AddTitle(const MenuBarList::key_type& title);
		void AddOption(const MenuBarList::key_type& title, const ui::NamedCommand& option);
	
	private:
		void DrawNodes();

	private:
		MenuBarList menuBarList;
	};
}
