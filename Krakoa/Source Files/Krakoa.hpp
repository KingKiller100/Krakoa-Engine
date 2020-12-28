#pragma once

// kLibrary
#include <kLib.hpp>

// For client game applications

// System required files
#include "Instrumentor.hpp"
#include "EngineMacros.hpp"

// Memory
#include "PointerTypes.hpp"

// Core
#include "Core/Application.hpp"
#include "Logging/Logger.hpp"

// Input
#include "Input/KeyCode.hpp"
#include "Input/InputManager.hpp"
#include "Input/MouseButtonCode.hpp"

// Camera
#include "Camera/OrthographicCamera.hpp"
#include "Camera/OrthographicCameraController.hpp"

// Colours
#include "Graphics/Helpers/Colour.hpp"

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
#include "Entity/Entity.hpp"
#include "Entity/EntityManager.hpp"
#include "Entity/Components/Transform.hpp"
#include "Entity/Components/Appearance.hpp"
