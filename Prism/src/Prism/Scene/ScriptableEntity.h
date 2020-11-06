#pragma once
#include "Entity.h"

namespace Prism
{
	class ScriptableEntity
	{
	public:
		template<typename T>
		T& GetComponent()
		{
			return m_Entity.GetComponent<T>();
		}

	private:
		Entity m_Entity; //This is an EntityHandle and Scene Pointer.
		friend class Scene;
	};

}
