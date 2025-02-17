#include "MooMooEngine_PCH.h"
#include "Entity.h"
#include "Components.h"

namespace MooMooEngine {

	EngineState StateManager::m_SceneState = EngineState::Edit;

	Scene::Scene() {
		entt::entity entity = m_Registry.create();
		m_Renderer3D = std::make_shared<Renderer3D>();

	}

	Scene::~Scene() {};

	Entity Scene::CreateEntity(const std::string& name) {
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<TransformComponent>();

		SetTag(name, entity);
	
		allEntitiesInScene.push_back(&entity);

		return entity;
	}

	void Scene::SetTag(std::string tag, Entity& entity, int nameCount) {
		std::string validTag;
		if (nameCount != 0) {
			validTag = tag + " " + to_string(nameCount);
		}
		else
		{
			validTag = tag;
		}

		auto view = m_Registry.view<TagComponent>();
		for (auto entityIt : view) {

			auto& tagComp = m_Registry.get<TagComponent>(entityIt);
			if (tagComp.Tag == validTag) {
				SetTag(tag, entity, nameCount + 1);
				return;
			}
		}
		auto& tagToAdd = entity.AddComponent<TagComponent>();
		tagToAdd.Tag = validTag.empty() ? "Entity" : validTag;		
	}

	void Scene::DeleteEntity(Entity* entity)
	{
		allEntitiesInScene.erase(std::remove(allEntitiesInScene.begin(), allEntitiesInScene.end(), entity), allEntitiesInScene.end());
		delete entity;
	}

	void Scene::OnUpdate(Timestep ts) {

		auto physicsView = m_Registry.view<TransformComponent, PhysicsRBDComponent>();
		for (auto entity : physicsView)
		{
			auto [transform, physX] = physicsView.get<TransformComponent, PhysicsRBDComponent>(entity);

			if (!physX.gravityEnabled) {
				physX.rbD->rbD->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
			}
			else {
				physX.rbD->rbD->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, false);
			}

			// This allows the user to control whether the entity should be moved with scripting (kinematic) or by the physics engine (dynamic).
			if (physX.isKinematic) {
				physX.rbD->rbD->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
			}
			else {
				// responds to physics and is set to dynamic mode.
				physX.rbD->rbD->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, false);
			}
			
			PxVec3 objectPos = physX.rbD->rbD->getGlobalPose().p;
			//transform.Translate(glm::vec3(physX.rbD.transform.p.x, physX.rbD.transform.p.y, physX.rbD.transform.p.z));
			transform.Translate(glm::vec3(objectPos.x, objectPos.y, objectPos.z));
			
		}

		m_Renderer3D->PreProcessing();

		// Gets all entities with a ModelComponent
		// Goes through each one
		// Passes the model and its transform to the renderer to actually render it:) (draw it)

		auto view = m_Registry.view<TransformComponent, ModelComponent, MaterialComponent>();
		for (auto entity : view) {
			auto[transform, entityModel, material] = view.get<TransformComponent, ModelComponent, MaterialComponent>(entity);

			material.shader->SetFloat4("u_ObjectColour", material.colour);

			if (material.hasTexture) {
				material.shader->SetInt("hasTexture", 1);
				material.shader->SetInt("colourID", material.texID);
				glBindTexture(GL_TEXTURE_2D, material.texID);
			}
			else { material.shader->SetInt("hasTexture", 0); }

			//entityModel.model.Draw(transform.Transform);
			m_Renderer3D->RenderModel(entityModel.model, transform.GetTransform());
		}

		// Updating the Objects that have a Shape component
		// Passes the shape and its transform to the renderer to actually render it:) (draw it)

		auto view2 = m_Registry.view<TransformComponent, ShapeComponent, MaterialComponent>();
		for (auto entity : view2) {
			auto [transform, entityModel, material] = view2.get<TransformComponent, ShapeComponent, MaterialComponent>(entity);

			material.shader->SetMat4("u_Transform", transform.GetTransform());

			material.shader->SetFloat4("u_ObjectColour", material.colour);

			if (material.hasTexture) {
				material.shader->SetInt("hasTexture", 1);
				material.shader->SetInt("colourID", material.texID);
				//glActiveTexture(GL_TEXTURE0 /*+ material.texID*/);
				glBindTexture(GL_TEXTURE_2D, material.texID);
			}
			else { material.shader->SetInt("hasTexture", 0); }

			material.shader->Bind();

			if (entityModel.shape == Tringle)     m_Renderer3D->drawTriangle();
			if (entityModel.shape == Square)      m_Renderer3D->drawSquare();
			if (entityModel.shape == Circle)      m_Renderer3D->drawCircle();
			if (entityModel.shape == Cube)		  m_Renderer3D->drawCube();
			if (entityModel.shape == Sphere)      m_Renderer3D->drawSphere();
		}

		auto scriptsView = m_Registry.view<ScriptComponent>();
		for (auto entity : scriptsView)
		{
			ScriptComponent& scripts = scriptsView.get<ScriptComponent>(entity);

			// console log that the GO has a script attached
			cout << "Script is attached" << endl;
		}

	}
}