#include "MooMooEngine_PCH.h"
//#include "DynamicRBComponent.h"
#include "Scene/Components.h"

namespace MooMooEngine
{
	DynamicRigidBody::DynamicRigidBody(PxPhysics* physics, PxScene* scene, glm::vec3 position)
    {
		pos = position;

		//Materials represent surface properties
		//(static friction, dynamic friction, restitution)
		baseMaterial = physics->createMaterial(0.5f, 0.5f, 0.5f);

		//Actors will need: 
		//	A Transform (position and orientation)
		//	A Type (RigidStatic / RigidDynmaic)
		//	A Shape (Type of Shape, Size of Shape, Material of Shape)

		//We create a Plane object that is a static rigidbody (so it will not move due to gravity, collisions etc.)

		//We have to rotate it so it acts like a floor
		transform = PxTransform(PxVec3(pos.x, pos.y, pos.z));
		//We create the Actor via a object type given a Transform
		rbD = physics->createRigidDynamic(transform);

		// Defaulted to having no gravity:)
		rbD->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
		
		/*rbD->addForce(PxVec3(0.f, 1000.f, 0.f));*/

		//We add the Actor to the Scene so it is simulated
		scene->addActor(*rbD);

		//PxBoxGeometry cubeGeo = (PxVec3(0.5f, 0.5f, 0.5f));
		////We create a Shape based off some Geometry and a Material (Plane Geometry is infinite remember)
		//PxShape* Pxshape = physics->createShape(cubeGeo, *baseMaterial);
		////We attach the Shape to the RigidBody so now the Plane has a Transform, Type and Shape
		//rbD->attachShape(*Pxshape);
    }

	DynamicRigidBody::~DynamicRigidBody()
    {
		if (rbD)
		{
			rbD = nullptr;
		}
		if (baseMaterial)
		{
			baseMaterial = nullptr;
		}
    }
}