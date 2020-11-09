#include "SceneHierarchyPanel.h"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
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

	static void DrawVector3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
	{
		ImGui::PushID(label.c_str());
		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth); //First Column
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		if (ImGui::Button("X", buttonSize))
		{
			values.x = resetValue;
		}
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		if (ImGui::Button("Y", buttonSize))
		{
			values.y = resetValue;
		}
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		if (ImGui::Button("Z", buttonSize))
		{
			values.z = resetValue; 
		}
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();
		ImGui::Columns(1);
		ImGui::PopID();
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
				auto& transformComponent = entity.GetComponent<TransformComponent>();
				DrawVector3Control("Translation", transformComponent.m_Translation);
				
				//Converts our radians to degrees for display in the Inspector, and translates its new values back to radians.
				glm::vec3 rotation = glm::degrees(transformComponent.m_Rotation);
				DrawVector3Control("Rotation", rotation);
				transformComponent.m_Rotation = glm::radians(rotation);

				DrawVector3Control("Scale", transformComponent.m_Scale, 1.0f);
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

					ImGui::Checkbox("Fixed Aspect Ratio", &cameraComponent.m_IsAspectRatioFixed);
				}

				ImGui::TreePop();
			}
		}
	}
}