#include <iInputManager.hpp>

namespace krakoa
{
	class WindowsInputManager : public iInputManager
	{
	public:
		WindowsInputManager();
		~WindowsInputManager();

	protected:
		// Inherited via iInputManager
		bool IsKeyPressedImpl(int keycode) override;
	};
}