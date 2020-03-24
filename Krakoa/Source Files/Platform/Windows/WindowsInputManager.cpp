#include <Precompile.hpp>
#include "WindowsInputManager.hpp"

#include "Core/Application.hpp"

#include "Patterns/SimpleSingletonImpl.hpp"

#include <GLFW/glfw3.h>

namespace krakoa
{
	WindowsInputManager::WindowsInputManager()
	{
	}

	WindowsInputManager::~WindowsInputManager()
	{
	}

	bool WindowsInputManager::IsKeyPressedImpl(int keycode)
	{
		const auto window = std::any_cast<GLFWwindow*>(Application::Reference().GetWindow().GetNativeWindow());
		const auto res = glfwGetKey(window, keycode);
		return res;
	}
}