#pragma once

#include "Scene.h"
#include "EnTT/include/entt.hpp"

namespace MooMooEngine 
{
	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity handle_, Scene* scene_);
		Entity(const Entity& other) = default;
		~Entity();

		template <typename T, typename ... Args>
		T& AddComponent(Args&&... args)
		{
			CORE_ASSERT(!HasComponent<T>(), "Entity already has this component");
			return m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
		}

		template <typename T>
		T& GetComponent()
		{
			CORE_ASSERT(HasComponent<T>(), "Entity does not have that component");
			return m_Scene->m_Registry.get<T>(m_EntityHandle);
		}

		template <typename T>
		bool HasComponent()
		{
			return m_Scene->m_Registry.all_of<T>(m_EntityHandle);
		}

		template <typename T>
		void RemoveComponent()
		{
			CORE_ASSERT(!HasComponent<T>(), "Entity does not have that component");
			return m_Scene->m_Registry.remove<T>(m_EntityHandle);
		}

		operator bool() { return m_EntityHandle != entt::null; }


	private:
		entt::entity m_EntityHandle = entt::null;
		Scene* m_Scene = nullptr;

	};
}