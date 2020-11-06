#pragma once
#include "Entity.h"

namespace Prism
{
	class ScriptableEntity
	{
	public:
		virtual ~ScriptableEntity() {}

		template<typename T>
		T& GetComponent()
		{
			return m_Entity.GetComponent<T>();
		}

	protected: //Not pure virtual as we may not want to have some of these in your scripts.
		virtual void OnCreate() {}
		virtual void OnDestroy() {}
		virtual void OnUpdate(Timestep deltaTime) {}

	private:
		Entity m_Entity; //This is an EntityHandle and Scene Pointer.
		friend class Scene;
	};

}
