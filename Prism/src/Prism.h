#pragma once

//All includes here are for use by Prism applications.

//----- Core -----

#include "Prism/Core/Application.h"
#include "Prism/Core/Layer.h"
#include "Prism/Core/Log.h"
#include "Prism/Core/Timestep.h"
#include "Prism/Core/Input.h"
#include "Prism/Core/MouseButtonCodes.h"
#include "Prism/Core/KeyCodes.h"
#include "Prism/Utilities/Version.h"
#include "Prism/Utilities/ThirdPartyLibrary.h"
#include "Prism/Threading/Threading.h"

//----- Scenes & Entities -----

#include "Prism/Scene/Scene.h"
#include "Prism/Scene/Components.h"
#include "Prism/Scene/Entity.h"
#include "Prism/Scene/ScriptableEntity.h"

//----- Editor -----

#include "Prism/ImGui/ImGuiLayer.h"
#include "Prism/Core/ILogInterface.h"


//----- Renderer -----

#include "Prism/Renderer/Renderer.h"
#include "Prism/Renderer/Renderer2D.h"
#include "Prism/Renderer/RenderCommand.h"

#include "Prism/Renderer/Buffer.h"
#include "Prism/Renderer/Shader.h"
#include "Prism/Renderer/Texture.h"
#include "Prism/Renderer/VertexArray.h"
#include "Prism/Renderer/SubTexture2D.h"
#include "Prism/Renderer/Framebuffer.h"

#include "Prism/Renderer/OrthographicCamera.h"
#include "Prism/Renderer/OrthographicCameraController.h"

//----- To Be Abstracted -----

#include "Platform/Windows/WindowsFileSystem.h"


