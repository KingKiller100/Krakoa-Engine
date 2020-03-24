#include <Patterns/SimpleSingleton.hpp>


namespace krakoa
{
	class iInputManager : pattern::SimpleSingleton<iInputManager>
	{
	public:
		bool IsKeyPressed(int keycode) { return Reference().IsKeyPressedImpl(keycode); }

	protected:
		virtual bool IsKeyPressedImpl(int keycode) = 0;
	};
}