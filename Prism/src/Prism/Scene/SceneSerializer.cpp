#include "PrismPrecompiledHeader.h"
#include "Entity.h"
#include "Components.h"
#include "SceneSerializer.h"
#include "yaml-cpp/yaml.h"
#include <fstream>

namespace YAML
{
	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs) //Encodes our Vec3 into 3 seperate float values.
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs) //Decodes the Vec3 we saved as 3 seperate float values and retrieves it.
		{
			if (!node.IsSequence() || node.size() != 3)
			{
				return false;
			}

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
			{
				return false;
			}

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};
}

namespace Prism
{
	YAML::Emitter& operator<<(YAML::Emitter& outputFile, const glm::vec3& vector)
	{
		outputFile << YAML::Flow; //Flow outputs the below as [1, 2, 3]
		outputFile << YAML::BeginSeq << vector.x << vector.y << vector.z << YAML::EndSeq;
		return outputFile;
	}

	YAML::Emitter& operator<<(YAML::Emitter& outputFile, const glm::vec4& vector)
	{
		outputFile << YAML::Flow;
		outputFile << YAML::BeginSeq << vector.x << vector.y << vector.z << vector.w << YAML::EndSeq;
		return outputFile;
	}

	SceneSerializer::SceneSerializer(const Reference<Scene>& sceneContext) : m_Scene(sceneContext)
	{

	}

	static void SerializeEntity(YAML::Emitter& outputFile, Entity entity)
	{
		outputFile << YAML::BeginMap; //Entity
		outputFile << YAML::Key << "Entity" << YAML::Value << "12345566778"; //To Do: Entity ID (UUID)

		if (entity.HasComponent<TagComponent>())
		{
			outputFile << YAML::Key << "TagComponent";
			outputFile << YAML::BeginMap; //Tag Component

			std::string& entityName = entity.GetComponent<TagComponent>().m_Tag;
			outputFile << YAML::Key << "Tag" << YAML::Value << entityName;

			outputFile << YAML::EndMap; //Tag Component
		}

		if (entity.HasComponent<TransformComponent>())
		{
			outputFile << YAML::Key << "TransformComponent";
			outputFile << YAML::BeginMap; //Transform Component

			auto& transformComponent = entity.GetComponent<TransformComponent>();
			outputFile << YAML::Key << "Translation" << YAML::Value << transformComponent.m_Translation;
			outputFile << YAML::Key << "Rotation" << YAML::Value << transformComponent.m_Rotation;
			outputFile << YAML::Key << "Scale" << YAML::Value << transformComponent.m_Scale;

			outputFile << YAML::EndMap; //Transform Component
		}

		if (entity.HasComponent<CameraComponent>())
		{
			outputFile << YAML::Key << "CameraComponent";
			outputFile << YAML::BeginMap; //Camera Component

			CameraComponent& cameraComponent = entity.GetComponent<CameraComponent>();
			SceneCamera& sceneCamera = cameraComponent.m_Camera;

			outputFile << YAML::Key << "Camera" << YAML::Value;
			outputFile << YAML::BeginMap; //Camera
			outputFile << YAML::Key << "ProjectionType" << YAML::Value << (int)sceneCamera.GetProjectionType();
			outputFile << YAML::Key << "PerspectiveFOV" << YAML::Value << sceneCamera.GetPerspectiveVerticalFieldOfView();
			outputFile << YAML::Key << "PerspectiveNearClip" << YAML::Value << sceneCamera.GetPerspectiveNearClip();
			outputFile << YAML::Key << "PerspectiveFarClip" << YAML::Value << sceneCamera.GetPerspectiveFarClip();
			outputFile << YAML::Key << "OrthographicSize" << YAML::Value << sceneCamera.GetOrthographicSize();
			outputFile << YAML::Key << "OrthographicNearClip" << YAML::Value << sceneCamera.GetOrthographicNearClip();
			outputFile << YAML::Key << "OrthographicFarClip" << YAML::Value << sceneCamera.GetOrthographicFarClip();
			outputFile << YAML::EndMap; //Camera

			outputFile << YAML::Key << "Primary" << YAML::Value << cameraComponent.m_IsPrimaryCamera;
			outputFile << YAML::Key << "FixedAspectRatio" << YAML::Value << cameraComponent.m_IsAspectRatioFixed;

			outputFile << YAML::EndMap; //Camera Component
		}

		if (entity.HasComponent<SpriteRendererComponent>())
		{
			outputFile << YAML::Key << "SpriteRendererComponent";
			outputFile << YAML::BeginMap; //Sprite Renderer Component

			SpriteRendererComponent& spriteRendererComponent = entity.GetComponent<SpriteRendererComponent>();
			outputFile << YAML::Key << "Color" << YAML::Value << spriteRendererComponent.m_Color;

			outputFile << YAML::EndMap; //Sprite Renderer Component
		}

		outputFile << YAML::EndMap; //Entity
	}

	void SceneSerializer::SerializeToYAML(const std::string& filePath)
	{
		YAML::Emitter outputFile;
		outputFile << YAML::BeginMap; 
		outputFile << YAML::Key << "Scene" << YAML::Value << "Untitled"; //Scene Information

		outputFile << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq; //Entities
		m_Scene->m_Registry.each([&](auto entityID)
		{
			Entity entity = { entityID, m_Scene.get() };
			if (!entity) { return; }

			SerializeEntity(outputFile, entity);
		});
		outputFile << YAML::EndSeq; //Entities

		outputFile << YAML::EndMap;

		std::ofstream fout(filePath);
		fout << outputFile.c_str();
	}

	void SceneSerializer::SerializeToBinary(const std::string& filePath)
	{
		//Not Implemented
		PRISM_ENGINE_ASSERT(false, "Not Implemented!");
	}

	bool SceneSerializer::DeserializeFromYAML(const std::string& filePath)
	{
		YAML::Node data = YAML::LoadFile(filePath); 
		if (!data["Scene"]) { return false; } //If the scene node isn't present, something's wrong. Nodes are essentially the Keys we created for each information. We have 2 root nodes: Scene & Entities.

		std::string sceneName = data["Scene"].as<std::string>(); //We then get the scene name.
		PRISM_ENGINE_TRACE("Deserializing Scene '{0}'", sceneName);

		YAML::Node entities = data["Entities"];
		if (entities)
		{
			for (auto entity : entities)
			{
				uint64_t uuid = entity["Entity"].as<uint64_t>();

				std::string entityName;
				auto tagComponent = entity["TagComponent"];
				if (tagComponent)
				{
					entityName = tagComponent["Tag"].as<std::string>();
				}

				PRISM_ENGINE_TRACE("Deserialized Entity with ID = {0}, Name = {1}", uuid, entityName);
			
				Entity deserializedEntity = m_Scene->CreateEntity(entityName);
			
				auto transformComponent = entity["TransformComponent"];
				if (transformComponent)
				{
					//Entities will always have transforms. We thus do not need to add the component, just add it.
					TransformComponent& deserializedTransformComponent = deserializedEntity.GetComponent<TransformComponent>();
					deserializedTransformComponent.m_Translation = transformComponent["Translation"].as<glm::vec3>();
					deserializedTransformComponent.m_Rotation = transformComponent["Rotation"].as<glm::vec3>();
					deserializedTransformComponent.m_Scale = transformComponent["Scale"].as<glm::vec3>();
				}

				auto cameraComponent = entity["CameraComponent"];
				if (cameraComponent)
				{
					CameraComponent& addedCameraComponent = deserializedEntity.AddComponent<CameraComponent>();
					
					auto& cameraProperties = cameraComponent["Camera"];
					addedCameraComponent.m_Camera.SetProjectionType((SceneCamera::ProjectionType)cameraProperties["ProjectionType"].as<int>());

					addedCameraComponent.m_Camera.SetPerspectiveVerticalFieldOfView(cameraProperties["PerspectiveFOV"].as<float>());
					addedCameraComponent.m_Camera.SetPerspectiveNearClip(cameraProperties["PerspectiveNearClip"].as<float>());
					addedCameraComponent.m_Camera.SetPerspectiveFarClip(cameraProperties["PerspectiveFarClip"].as<float>());
					
					addedCameraComponent.m_Camera.SetOrthographicSize(cameraProperties["OrthographicSize"].as<float>());
					addedCameraComponent.m_Camera.SetOrthographicNearClip(cameraProperties["OrthographicNearClip"].as<float>());
					addedCameraComponent.m_Camera.SetOrthographicFarClip(cameraProperties["OrthographicFarClip"].as<float>());

					addedCameraComponent.m_IsPrimaryCamera = cameraComponent["Primary"].as<bool>();
					addedCameraComponent.m_IsAspectRatioFixed = cameraComponent["FixedAspectRatio"].as<bool>();
				}

				auto spriteRendererComponent = entity["SpriteRendererComponent"];
				if (spriteRendererComponent)
				{
					SpriteRendererComponent& addedSpriteRendererComponent = deserializedEntity.AddComponent<SpriteRendererComponent>();
					addedSpriteRendererComponent.m_Color = spriteRendererComponent["Color"].as<glm::vec4>();
				}
			} 
		}

		return true;
	}

	bool SceneSerializer::DeserializeFromBinary(const std::string& filePath)
	{

		//Not Implemented
		PRISM_ENGINE_ASSERT(false, "Not Implemented!");
		return false;
	}
}