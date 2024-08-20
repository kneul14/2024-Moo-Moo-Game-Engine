#pragma once

#include "Core/Timestep.h"
#include "EnTT/include/entt.hpp"
#include "Renderer/3D/Renderer3D.h"

#include "Scene/StateManager.h"

#include "vector"

namespace MooMooEngine {

	class Entity;
	class StateManager;

	class Scene {
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());
		void DeleteEntity(Entity* entity);

		void SetTag(std::string name, Entity& entity, int nameCount = 0);

		void OnUpdate(Timestep ts);
		entt::registry m_Registry; // do a getter 

		std::vector<Entity*> allEntitiesInScene;

	private:
		//Container for all the components and entities
		friend class Entity;
		Ref<Renderer3D> m_Renderer3D;
	};
}