﻿#include <Precompile.hpp>
#include <ImGui/ImGuiLayer.hpp>

#include <Core/Application.hpp>

#include <imgui.h> // imgui include must be above any other openGL includes
#include <Platform/OpenGL/imgui_impl_opengl3.hpp>

// Temp for keymap
#include <GLFW/glfw3.h>
#include <glad/glad.h>


namespace krakoa
{
	ImGuiLayer::ImGuiLayer()
		: LayerBase("ImGuiLayer"),
		time(0.0f)
	{	}

	ImGuiLayer::~ImGuiLayer()
		= default;

	void ImGuiLayer::OnAttach()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		auto& io = ImGui::GetIO();
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

		// TODO: Temp keymap: Become Krakoa keymap
		io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
		io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
		io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
		io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
		io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
		io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
		io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
		io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
		io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
		io.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
		io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
		io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
		io.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
		io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
		io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
		io.KeyMap[ImGuiKey_KeyPadEnter] = GLFW_KEY_KP_ENTER;
		io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
		io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
		io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
		io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
		io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
		io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void ImGuiLayer::OnDetach()
	{

	}

	void ImGuiLayer::OnUpdate()
	{
		glClearColor(0.85f, 0.35f, 0.f, 0.25f); // Orange background
		glClear(GL_COLOR_BUFFER_BIT);

		auto& window = Application::Pointer()->GetWindow();
		
		auto& io = ImGui::GetIO(); (void)io;
		io.DisplaySize = ImVec2(static_cast<float>(window.GetWidth()), static_cast<float>(window.GetHeight()));

		const auto currentTime = static_cast<float>(glfwGetTime());
		const auto desiredDeltaTime = (1.f / 60.f);
		io.DeltaTime = time > 0.0f ? currentTime - time : desiredDeltaTime;
		time = currentTime;
		
		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();
		
		bool isShowing = true;
		ImGui::ShowDemoWindow(&isShowing);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void ImGuiLayer::OnEvent(events::Event& e)
	{

	}
}

