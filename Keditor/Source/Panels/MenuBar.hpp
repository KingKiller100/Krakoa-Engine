#pragma once

namespace krakoa::panels
{
	class MenuBar
	{
	public:
		MenuBar() noexcept = default;
		~MenuBar() = default;

		void Draw();

	private:
		void DrawNodes();
		void DrawFileNode();
	};
}
