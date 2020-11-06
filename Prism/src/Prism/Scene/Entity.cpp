#include "PrismPrecompiledHeader.h"
#include "Entity.h"

namespace Prism
{
	Entity::Entity(entt::entity entityHandle, Scene* scene) : m_EntityHandle(entityHandle), m_Scene(scene)
	{

	}
}