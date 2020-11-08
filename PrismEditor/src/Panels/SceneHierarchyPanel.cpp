#include "SceneHierarchyPanel.h"
#include "imgui/imgui.h"
#include "Prism/Scene/Components.h"

namespace Prism
{
	SceneHierarchyPanel::SceneHierarchyPanel(const Reference<Scene>& sceneContext)
	{
		SetHierachyContext(sceneContext);
	}

	void SceneHierarchyPanel::SetHierachyContext(const Reference<Scene>& sceneContext)
	{
		m_HierarchyContext = sceneContext;
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarcy");
		m_HierarchyContext->m_Registry.each([&](auto entityID)
		{
			Entity entity{ entityID, m_HierarchyContext.get() };
			DrawEntityNode(entity);		
		});

		ImGui::End();
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		std::string& entityTagComponent = entity.GetComponent<TagComponent>().m_Tag;
		
		ImGuiTreeNodeFlags flags = ((m_HierarchySelectedEntity == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;	
		bool isExpanded = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, entityTagComponent.c_str()); //We're using the Entity's ID to serve as the tree node's unique identifier.

		if (ImGui::IsItemClicked())
		{
			m_HierarchySelectedEntity = entity;
		}

		if (isExpanded)
		{
			ImGui::TreePop();
		}
	}
}