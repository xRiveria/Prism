#pragma once
#include "Prism/Core/Core.h"
#include "Prism/Core/Log.h"
#include "Prism/Scene/Entity.h"
#include "Prism/Scene/Scene.h"

namespace Prism
{
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Reference<Scene>& sceneContext);
		void SetHierachyContext(const Reference<Scene>& sceneContext);

		void OnImGuiRender();

		Entity GetSelectedEntity() const { return m_HierarchySelectedEntity; }

	private:
		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);

	private:
		Reference<Scene> m_HierarchyContext; 
		Entity m_HierarchySelectedEntity;
	};
}