#include "MooMooEngine_PCH.h"
#include "PxPhysicsAPI.h"
#include "Scene/Components.h"

//#include "Physics/PhysXBoxCollider.h"


namespace MooMooEngine
{
	//PhysXBoxCollider::PhysXBoxCollider(glm::vec3 s, glm::vec3 pos, glm::vec3 off)
	//{
	//	extents  = s;
	//	position = pos + off;
	//	offset = off;
	//}

	PhysXBoxCollider::PhysXBoxCollider(PxPhysics* physics, PxScene* scene, PxRigidActor* rb, PxMaterial* baseMaterial, glm::vec3 halfExtents)
	{
		extents = halfExtents;
		rigidBody = rb;
		cubeGeo = (PxVec3(extents.x, extents.y, extents.z));
		//We create a Shape based off some Geometry and a Material (Plane Geometry is infinite remember)
		shape = physics->createShape(cubeGeo, *baseMaterial);
		//We attach the Shape to the RigidBody so now the Plane has a Transform, Type and Shape
		rigidBody->attachShape(*shape);
	}

	// make a way to updATE THE SIZE IN RUNTIME LOL
	// maybe make a change shape :thumbs_up:
	void PhysXBoxCollider::ChangeShape(glm::vec3 halfExtents)
	{
		extents = halfExtents;
		cubeGeo = (PxVec3(extents.x, extents.y, extents.z));
		shape->setGeometry(cubeGeo);
	}

	PhysXBoxCollider::~PhysXBoxCollider()
	{
		rigidBody->detachShape(*shape);
	}
}
