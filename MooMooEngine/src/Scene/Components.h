#pragma once

#include "glm/glm.hpp"
#include "PxPhysicsAPI.h"
#include "IO/Models/model.h"
#include "Renderer/Textures/Texture2D.h"
#include "Physics/PhysicsLayer.h"
#include "Physics/RigidBodyComponent.h"
#include "Physics/PhysXBoxCollider.h"
#include "Physics/PhysXSphereCollider.h"

namespace MooMooEngine 
{
	enum ShapeType { Tringle, Square, Circle, Cube, Cone, Sphere, Mesh };

	struct TagComponent 
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag_) : Tag(tag_) {}

	};

	struct TransformComponent
	{		
		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale = glm::vec3(1);

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		//TransformComponent(const glm::mat4 transform) : Transform(transform) {}

		//operator glm::mat4() { return Transform; }
		//operator const glm::mat4& () const { return Transform; }

		void Translate(glm::vec3 v) { position = v; }
		void Scale(glm::vec3 s)		{ scale = s; }
		void Rotation(glm::vec3 r)  { rotation = r; }


		glm::mat4 GetTransform() {
			glm::mat4 Transform{ 1.0f };

			Transform = glm::translate(Transform, position);
			Transform = glm::scale(Transform, scale);
			Transform = glm::rotate(Transform, rotation.x, glm::vec3(1,0,0));
			Transform = glm::rotate(Transform, rotation.y, glm::vec3(0,1,0));
			Transform = glm::rotate(Transform, rotation.z, glm::vec3(0,0,1));

			return Transform;
		};
		
	};

	struct ModelComponent {
		Model model;
		
		ModelComponent() = default;
		ModelComponent(const Model& model_) : model(model_) {};
	};

	struct ShapeComponent {
		int shape;

		ShapeComponent() = default;
		ShapeComponent( int shape_) : shape(shape_) {};
	};

	struct MaterialComponent {
		//bool hasMatComp = false;
		bool hasTexture = false;

		Texture2D texture2D;

		std::string textPath;

		unsigned int texID;

		Ref<Shader> shader;
		glm::vec4 colour;

		void Colour(glm::vec4 c) { colour = c; }
		void TextureID(unsigned int t) { texID = t; }

		MaterialComponent() = default;
		MaterialComponent(Ref<Shader> shader_) : shader(shader_) {};
	};

	struct PhysicsRBDComponent
	{
		DynamicRigidBody* rbD;
		bool hasPXComp = false;
		bool gravityEnabled = false;
		bool isKinematic = false;

		PhysicsRBDComponent() = default;
		PhysicsRBDComponent(DynamicRigidBody* physicsRBDComponent_) : rbD(physicsRBDComponent_) {};
	};

	struct BoxColliderComponent
	{		
		PhysXBoxCollider* physXBoxCollider;
		bool hasBoxComp = false;
		bool hasBoxCollComp = false;
		bool isTrigger = false;


		glm::vec3 position = glm::vec3(0);
		glm::vec3 halfExtents = glm::vec3(1);

		BoxColliderComponent() = default;
		BoxColliderComponent(PhysXBoxCollider* physXBoxCollider_) : physXBoxCollider(physXBoxCollider_) {};		
	};

	struct SphereColliderComponent
	{
		PhysXSphereCollider* physXSphereCollider;
		bool hasBoxComp = false;
		bool hasBoxCollComp = false;
		bool isTrigger = false;

		glm::vec3 position = glm::vec3(0);
		float radius = 1.0f; // Added missing data type for radius and initialized it

		SphereColliderComponent() = default;
		SphereColliderComponent(PhysXSphereCollider* physXSphereCollider_) : physXSphereCollider(physXSphereCollider_) {}; // Fixed constructor parameter name
	};

	struct ScriptComponent
	{
		bool hasScriptComp = false;
		bool isVSNeeded = false;
		std::string name;

		ScriptComponent() = default;
		ScriptComponent(const std::string& scriptName)
			: name(scriptName), hasScriptComp(true) {};

	};

	//struct PlayerController {
	//	PlayerControllerScript playerController;
	//};
}
