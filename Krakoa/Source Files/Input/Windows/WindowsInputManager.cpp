#include "Precompile.hpp"
#include "WindowsInputManager.hpp"

#include "../../Instrumentor.hpp"
#include "../../Core/Application.hpp"

#include <GLFW/glfw3.h>

namespace krakoa::input
{
	WindowsInputManager::WindowsInputManager(Token&&) noexcept
	{	}

	WindowsInputManager::~WindowsInputManager()
		= default;

	void InputManager::CreateImpl() noexcept
	{
		KRK_PROFILE_FUNCTION();
		InputManager::Create<WindowsInputManager>();
	}

	bool WindowsInputManager::IsKeyPressedImpl(int keycode) const noexcept
	{
		KRK_PROFILE_FUNCTION();
		const auto window = std::any_cast<GLFWwindow*>(Application::Reference().GetWindow().GetNativeWindow());
		const auto state = glfwGetKey(window, keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool WindowsInputManager::IsMouseButtonPressedImpl(const MouseButtonType button) const noexcept
	{
		KRK_PROFILE_FUNCTION();
		const auto window = std::any_cast<GLFWwindow*>(Application::Reference().GetWindow().GetNativeWindow());
		const auto state = glfwGetMouseButton(window, static_cast<int>(button));
		return state == GLFW_PRESS;
	}

	std::pair<float, float> WindowsInputManager::GetMousePositionImpl() const noexcept
	{
		KRK_PROFILE_FUNCTION();
		const auto window = std::any_cast<GLFWwindow*>(Application::Reference().GetWindow().GetNativeWindow());
		double xPos, yPos;
		glfwGetCursorPos(window, &xPos, &yPos);
		return { static_cast<float>(xPos), static_cast<float>(yPos) };
	}

	float WindowsInputManager::GetMousePosXImpl() const noexcept
	{
		KRK_PROFILE_FUNCTION();
		const auto[x, y] = GetMousePosition();
		return x;
	}

	float WindowsInputManager::GetMousePosYImpl() const noexcept
	{
		KRK_PROFILE_FUNCTION();
		const auto[x, y] = GetMousePosition();
		return y;
	}
}

