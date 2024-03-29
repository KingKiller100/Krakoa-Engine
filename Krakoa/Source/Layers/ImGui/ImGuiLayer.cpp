﻿#include "Precompile.hpp"
#include "ImGuiLayer.hpp"

#include "../../Config/GlobalConfig.hpp"
#include "../../Debug/Instrumentor.hpp"
#include "../../Core/Application.hpp"

#include <imgui.h> // ImGui include must be above any other openGL includes

#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>

// Temp for keymap
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <ImGuizmo.h>

namespace krakoa
{
	ImGuiLayer::ImGuiLayer()
		: LayerBase("ImGuiLayer")
		, isShowing(false)
		, isBlockingEvents(true)
		, demoWindowState(false)
	{
		SetDemoWindowStateFromConfig();
	}

	ImGuiLayer::~ImGuiLayer()
		= default;

	void ImGuiLayer::OnAttach()
	{
		KRK_PROFILE_FUNCTION();

		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		//io.ConfigViewportsNoAutoMerge = true;
		//io.ConfigViewportsNoTaskBarIcon = true;

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		const auto window = GetApp().GetWindow().GetNativeWindow<GLFWwindow>();

		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 430");
	}

	void ImGuiLayer::OnDetach()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::OnUpdate(float deltaTime)
	{}

	void ImGuiLayer::OnRender()
	{
		KRK_PROFILE_FUNCTION();

		if (demoWindowState)
			ImGui::ShowDemoWindow();
	}

	void ImGuiLayer::OnEvent(events::Event& e)
	{
		if (!isBlockingEvents)
			return;

		ImGuiIO& io = ImGui::GetIO();
		auto handled = e.isHandled();
		handled |= e.IsInCategory(events::E_C_MOUSE) & io.WantCaptureMouse;
		handled |= e.IsInCategory(events::E_C_KEYBOARD) & io.WantCaptureKeyboard;

		e.SetHandled(handled);
	}

	void ImGuiLayer::BeginDraw()
	{
		KRK_PROFILE_FUNCTION();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();
	}

	void ImGuiLayer::EndDraw() const
	{
		KRK_PROFILE_FUNCTION();

		// Get Display Dimensions
		auto& io = ImGui::GetIO();
		const auto& app = GetApp();
		const auto& dimensions = app.GetWindow().GetDimensions();
		io.DisplaySize = ImVec2(static_cast<float>(dimensions.width), 
			static_cast<float>(dimensions.height));

		// Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Create a back up context for a window to render outside of main window space
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			const auto backupCurrentContext = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backupCurrentContext);
		}
	}

	void ImGuiLayer::ToggleVisibility() noexcept
	{
		KRK_PROFILE_FUNCTION();

		isShowing = !isShowing;
	}

	bool ImGuiLayer::GetBlockEventsStatus() const noexcept
	{
		return isBlockingEvents;
	}

	void ImGuiLayer::BlockEvents()
	{
		isBlockingEvents = true;
	}

	void ImGuiLayer::UnblockEvents()
	{
		isBlockingEvents = false;
	}

	void ImGuiLayer::SetDemoWindowStateFromConfig()
	{
		demoWindowState = configurations::GetConfiguration<bool>("ImGui", "DemoWindow");
	}
}

