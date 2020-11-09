#pragma once

#include "Prism/Core/Core.h"
#include "Scene.h"

namespace Prism
{
	class SceneSerializer
	{
	public:
		SceneSerializer(const Reference<Scene>& sceneContext);

		void SerializeToYAML(const std::string& filePath);
		void SerializeToBinary(const std::string& filePath);

		bool DeserializeFromYAML(const std::string& filePath);
		bool DeserializeFromBinary(const std::string& filePath);

	private:
		Reference<Scene> m_Scene;
	};
}
