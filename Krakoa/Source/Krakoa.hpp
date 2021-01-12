#pragma once

// kLibrary
#include <kLib.hpp>

// For client game applications

// System required files
#include "Core/EngineMacros.hpp"

// Memory
#include "Core/PointerTypes.hpp"

// Core
#include "Core/Application.hpp"
#include "Logging/Logger.hpp"

// Debug & Profiling
#include "Debug/Debug.hpp"
#include "Debug/Instrumentor.hpp"

// Input
#include "Input/KeyCode.hpp"
#include "Input/InputManager.hpp"
#include "Input/MouseButtonCode.hpp"

// Camera
#include "Camera/OrthographicCamera.hpp"
#include "Camera/OrthographicCameraController.hpp"

// Colours
#include "Graphics/Helpers/Colour.hpp"

// Frame Buffer
#include "Graphics/Framebuffers/iFrameBuffer.hpp"

// Rendering
#include "Graphics/Renderer.hpp"
#include "Graphics/2D/Renderer2D.hpp"
#include "Graphics/RenderCommand.hpp"

// Textures
#include "Graphics/Textures/iTexture.hpp"
#include "Graphics/2D/Textures/iTexture2D.hpp"
#include "Graphics/2D/Textures/SubTexture2d.hpp"

// Rendering Resources
#include "Graphics/ShaderLibrary.hpp"
#include "Graphics/Resources/iShader.hpp"
#include "Graphics/Resources/iBuffers.hpp"
#include "Graphics/Resources/iVertexArray.hpp"
#include "Graphics/Resources/BufferLayout.hpp"

// Layers
#include "Layers/LayerBase.hpp"
#include "Layers/ImGui/ImGuiLayer.hpp"

// Entities & Components
#include "Entity/Entity.hpp"
#include "Entity/EntityComponentSystem.hpp"
#include "Entity/Components/TransformComponent.hpp"
#include "Entity/Components/AppearanceComponent.hpp"