#pragma once

namespace Prism
{
	class GraphicsContext
	{
	public:
		virtual ~GraphicsContext() = default;

		virtual void Initialize() = 0;
		virtual void SwapBuffers() = 0;
	};

}
