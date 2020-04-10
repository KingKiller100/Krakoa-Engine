#include "Precompile.hpp"
#include "ImGuiLayer.hpp"

#include "../Core/EngineCore.hpp"
#include "../Core/Application.hpp"

#include <imgui.h> // ImGui include must be above any other openGL includes

#include <examples/imgui_impl_opengl3.h>
#include <examples/imgui_impl_glfw.h>

// Temp for keymap
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <functional>

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

		auto window = ANY_CAST(GLFWwindow*, Application::Reference().GetWindow().GetNativeWindow());

		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 410");
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
		static bool showWindow = true;
		ImGui::ShowDemoWindow(&showWindow);
	}

	void ImGuiLayer::OnEvent(events::Event& e)
	{}

	void ImGuiLayer::BeginDraw()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiLayer::EndDraw()
	{
		// Get Display Dimensions
		auto& io = ImGui::GetIO();
		const auto& app = Application::Reference();
		const auto& dimensions = app.GetWindow().GetDimensions();
		io.DisplaySize = ImVec2(CAST(float, dimensions.X()), CAST(float, dimensions.Y()));

		// Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Create a back up context for a window to render outside of main window space
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			auto backupCurrentContext = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backupCurrentContext);
		}
	}
}

