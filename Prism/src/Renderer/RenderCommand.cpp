#include "PrismPrecompiledHeader.h"
#include "RenderCommand.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Prism
{
	//Will be set dynamically in the future. 
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;
}