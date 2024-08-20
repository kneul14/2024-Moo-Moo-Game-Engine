#include "MooMooEngine_PCH.h"
#include "PxPhysicsAPI.h"
#include "Scene/Components.h"
//#include "PhysXSphereCollider.h"

namespace MooMooEngine
{
	PhysXSphereCollider::PhysXSphereCollider(PxPhysics* physics, PxScene* scene, PxRigidActor* rb, PxMaterial* baseMaterial, float radius)
	{
		this->radius = radius;
		rigidBody = rb;
		sphereGeo = PxSphereGeometry(radius);
		shape = physics->createShape(sphereGeo, *baseMaterial);
		rigidBody->attachShape(*shape);
	}

	void PhysXSphereCollider::ChangeShape(float radius)
	{
		this->radius = radius;
		sphereGeo = PxSphereGeometry(radius);
		shape->setGeometry(sphereGeo);
	}

	PhysXSphereCollider::~PhysXSphereCollider()
	{
		rigidBody->detachShape(*shape);
	}
}