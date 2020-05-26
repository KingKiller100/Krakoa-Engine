#pragma once

// kLibrary
#include <kLib.hpp>

// For client game applications

// Core
#include "Instrumentor.hpp"
#include "Core/EngineConfig.hpp"
#include "Core/Application.hpp"
#include "Core/Logging/Logger.hpp"

// Input
#include "Input/KeyCode.hpp"
#include "Input/InputManager.hpp"
#include "Input/MouseButtonCode.hpp"

// Camera
#include "Camera/OrthographicCamera.hpp"
#include "Camera/OrthographicCameraController.hpp"

// Colours
#include "Graphics/Colour.hpp"

// Rendering
#include "Graphics/Renderer.hpp"
#include "Graphics/Renderer2D.hpp"
#include "Graphics/RenderCommand.hpp"

// Textures
#include "Graphics/Textures/iTexture.hpp"
#include "Graphics/Textures/iTexture2D.hpp"
#include "Graphics/Textures/SubTexture2d.hpp"

// Rendering Resources
#include "Graphics/ShaderLibrary.hpp"
#include "Graphics/Rendering Resources/iShader.hpp"
#include "Graphics/Rendering Resources/iBuffers.hpp"
#include "Graphics/Rendering Resources/iVertexArray.hpp"
#include "Graphics/Rendering Resources/BufferLayout.hpp"

// Layers
#include "Layers/LayerBase.hpp"
#include "Layers/ImGui/ImGuiLayer.hpp"

// Entities & Components
#include "Entity/EntitiyManager.hpp"
#include "Entity/Components/Transform.hpp"
