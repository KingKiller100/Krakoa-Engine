#include "Precompile.hpp"
#include "WindowsInputManager.hpp"

#include "../../Debug/Instrumentor.hpp"
#include "../../Core/Application.hpp"

#include <bitset>
#include <GLFW/glfw3.h>

namespace krakoa::input
{	
	WindowsInputManager::WindowsInputManager(const Token& t) noexcept
		: InputManager(t)
	{	}

	WindowsInputManager::~WindowsInputManager()
		= default;

	void InputManager::CreateImpl() noexcept
	{
		KRK_PROFILE_FUNCTION();
		Create<WindowsInputManager>();
	}

	bool WindowsInputManager::IsKeyPressedImpl(KeyCode keycode) const noexcept
	{
		KRK_PROFILE_FUNCTION();
		const auto window = GetWindow().GetNativeWindow<GLFWwindow>();
		const auto state = glfwGetKey(window, static_cast<int>(keycode));
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool WindowsInputManager::IsKeyReleasedImpl(KeyCode keycode) const noexcept
	{
		static std::bitset<KEY_LAST> bitset;

		bool released = false;
		
		if (IsKeyPressedImpl(keycode))
			bitset.set(keycode);
		else if (bitset[keycode])
		{
			bitset.reset(keycode);
			released = true;
		}

		return released;
	}

	bool WindowsInputManager::IsMouseButtonPressedImpl(MouseCode button) const noexcept
	{
		KRK_PROFILE_FUNCTION();
		const auto window = GetWindow().GetNativeWindow<GLFWwindow>();
		const auto state = glfwGetMouseButton(window, static_cast<int>(button));
		return state == GLFW_PRESS;
	}

	kmaths::Vector2f WindowsInputManager::GetMousePositionImpl() const noexcept
	{
		KRK_PROFILE_FUNCTION();
		const auto window = GetWindow().GetNativeWindow<GLFWwindow>();
		double xPos, yPos;
		glfwGetCursorPos(window, &xPos, &yPos);
		return { static_cast<float>(xPos), static_cast<float>(yPos) };
	}

	float WindowsInputManager::GetMousePosXImpl() const noexcept
	{
		KRK_PROFILE_FUNCTION();
		const auto [x, y] = GetMousePosition();
		return x;
	}

	float WindowsInputManager::GetMousePosYImpl() const noexcept
	{
		KRK_PROFILE_FUNCTION();
		const auto [x, y] = GetMousePosition();
		return y;
	}
}

