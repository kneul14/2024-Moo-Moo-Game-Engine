#include "MooMooEngine_PCH.h"
#include "Entity.h"

namespace MooMooEngine {

	Entity::Entity(entt::entity handle_, Scene* scene_) : m_EntityHandle(handle_), m_Scene(scene_)
	{
	}

	Entity::~Entity()
	{
		//m_Scene->m_Registry.destroy(m_EntityHandle);
	}
}