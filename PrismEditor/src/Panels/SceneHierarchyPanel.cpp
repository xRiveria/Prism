#include "SceneHierarchyPanel.h"
#include "imgui/imgui.h"
#include "Prism/Scene/Components.h"
#include "glm/gtc/type_ptr.hpp"

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

		if (ImGui::IsWindowHovered() && ImGui::IsMouseDown(0)) //If we click elsewhere anywhere in the window, we deselect the current object. 
		{
			m_HierarchySelectedEntity = {};
		}

		ImGui::End();

		ImGui::Begin("Properties");
		if (m_HierarchySelectedEntity)
		{
			DrawComponents(m_HierarchySelectedEntity);
		}
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

		if (isExpanded) //If any child entities exist within the selected node, we continue recursively going down their trees.
		{
			ImGui::TreePop();
		}
	} 

	void SceneHierarchyPanel::DrawComponents(Entity entity)
	{
		if (entity.HasComponent<TagComponent>())
		{
			std::string& entityTagComponent = entity.GetComponent<TagComponent>().m_Tag;
			
			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), entityTagComponent.c_str()); //Copy entity name into the buffer.

			if (ImGui::InputText("Name", buffer, sizeof(buffer))) //The name is essentially what we will be editing in the UI control.
			{
				entityTagComponent = std::string(buffer);
			}
		}

		if (entity.HasComponent<TransformComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
			{
				auto& transform = entity.GetComponent<TransformComponent>().m_Transform;
				ImGui::DragFloat3("Position", glm::value_ptr(transform[3]), 0.10f);
				
				ImGui::TreePop();
			}		
		}

		if (entity.HasComponent<SpriteRendererComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(SpriteRendererComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Sprite Renderer"))
			{
				auto& entitySpriteComponent = entity.GetComponent<SpriteRendererComponent>();
				ImGui::ColorEdit4("Color", glm::value_ptr(entitySpriteComponent.m_Color));
				
				ImGui::TreePop();
			}
		}

		if (entity.HasComponent<CameraComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Camera"))
			{
				CameraComponent& cameraComponent = entity.GetComponent<CameraComponent>();
				SceneCamera& sceneCamera = cameraComponent.m_Camera;

				ImGui::Checkbox("Primary", &cameraComponent.m_IsPrimaryCamera);

				const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
				const char* currentProjectionTypeString = projectionTypeStrings[(int)sceneCamera.GetProjectionType()];
				if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
				{
					for (int i = 0; i < 2; i++)
					{
						bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
						if (ImGui::Selectable(projectionTypeStrings[i], isSelected))
						{
							currentProjectionTypeString = projectionTypeStrings[i];
							sceneCamera.SetProjectionType((SceneCamera::ProjectionType)i);
						}

						if (isSelected)
						{
							ImGui::SetItemDefaultFocus();
						}
					}

					ImGui::EndCombo();
				}
				//Remember that we want to keep the values of both projections even when switching between them. Thus, we will have seperate stack containers for them.
				if (sceneCamera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
				{
					float perspectiveVerticalFieldOfView = glm::degrees(sceneCamera.GetPerspectiveVerticalFieldOfView());
					if (ImGui::DragFloat("Vertical FOV", &perspectiveVerticalFieldOfView))
					{
						sceneCamera.SetPerspectiveVerticalFieldOfView(glm::radians(perspectiveVerticalFieldOfView));
					}

					float perspectiveNear = sceneCamera.GetPerspectiveNearClip();
					if (ImGui::DragFloat("Near", &perspectiveNear))
					{
						sceneCamera.SetPerspectiveNearClip(perspectiveNear);
					}

					float perspectiveFar = sceneCamera.GetPerspectiveFarClip();
					if (ImGui::DragFloat("Far", &perspectiveFar))
					{
						sceneCamera.SetPerspectiveFarClip(perspectiveFar);
					}
				}
				
				if (sceneCamera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
				{
					float orthographicSize = sceneCamera.GetOrthographicSize();
					if (ImGui::DragFloat("Size", &orthographicSize))
					{
						sceneCamera.SetOrthographicSize(orthographicSize);
					}

					float orthographicNearClip = sceneCamera.GetOrthographicNearClip();
					if (ImGui::DragFloat("Near", &orthographicNearClip))
					{
						sceneCamera.SetOrthographicNearClip(orthographicNearClip);
					}

					float orthographicFarClip = sceneCamera.GetOrthographicFarClip();
					if (ImGui::DragFloat("Far", &orthographicFarClip))
					{
						sceneCamera.SetOrthographicFarClip(orthographicFarClip);
					}

					ImGui::Checkbox("Fixed Aspect Ration", &cameraComponent.m_IsAspectRatioFixed);
				}

				ImGui::TreePop();
			}
		}
	}
}