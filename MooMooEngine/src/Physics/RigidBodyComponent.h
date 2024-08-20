#pragma once
#include "PxPhysicsAPI.h"
#include "Core/Core.h"
#include "Physics/PhysicsLayer.h"
#include "glm/glm.hpp"
using namespace physx;

namespace MooMooEngine 
{
	class DynamicRigidBody
	{
	public:
		DynamicRigidBody() = default;
		DynamicRigidBody(PxPhysics* physics, PxScene* scene, glm::vec3 position);
		~DynamicRigidBody();

		PxMaterial*	    baseMaterial;
		PxTransform	    transform;
		glm::vec3		pos;
		PxRigidDynamic*   rbD;
		//PxRigidActor*   rb;

		float mass            = 1.0f;

		float sFriction       = 0.5f;
		float dFriction       = 0.5f;
		float restitution     = 0.5f;

		// So that the user can stipulate the values in the inspector window:)
		void Mass(float m)             { mass = m; }
		void StaticFriction(float sf)  { sFriction = sf; }
		void DynamicFriction(float df) { dFriction = df; }
		void Restitution(float r)      { restitution = r; }
	};
}

//bool hasPXComp = false;
//bool isKinematic = false;
////Ref<PhysicsLayer> physX;
//
//PxScene* scene = nullptr;
//glm::vec3 transform = glm::vec3(0.0f);
//PxRigidDynamic* actor = nullptr;
//PxMaterial* material = nullptr;
//
//float mass = 1.0f;
//float sFriction = 0.5f;
//float dFriction = 0.5f;
//float restitution = 0.5f;
//
//// So that the user can stipulate the values in the inspector window:)
//void StaticFriction(float sf) { sFriction = sf; }
//void DynamicFriction(float df) { dFriction = df; }
//void Restitution(float r) { restitution = r; }
//
//PhysicsRBDComponent() = default;
//
////PhysicsRBDComponent(Ref<PhysicsLayer> physX_) : physX(physX_), hasPXComp(true), isKinematic(false), scene(nullptr),
////											 transform(glm::vec3(0.0f)), actor(nullptr), material(nullptr), mass(1.0f)
////{
////
////}
//
//PhysicsRBDComponent(PxPhysics* physics, PxScene* scene, const glm::vec3& go_Pos)
//{
//	// Materials represent surface properties (static friction, dynamic friction, restitution)
//	material = physics->createMaterial(sFriction, dFriction, restitution);
//
//	// Initialize the transform
//	transform = go_Pos;
//
//	// Create the DynamicRBComponent
//	DynamicRBComponent dynamicRB(physics, scene, go_Pos);
//	this->scene = scene;
//	this->actor = dynamicRB.rbD;
//	this->material = dynamicRB.baseMaterial;
//}
//
//// Destructor
//~PhysicsRBDComponent()
//{
//	if (actor)
//	{
//		scene->removeActor(*actor);
//		actor->release();
//	}
//}
