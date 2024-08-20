#include "MooMooEngine_PCH.h"
#include "PxPhysicsAPI.h"
#include "Scene/Components.h"
#include <glm/glm.hpp>

namespace MooMooEngine
{
	class PhysXSphereCollider : BaseCollider
	{
	public:
		PhysXSphereCollider(PxPhysics* physics, PxScene* scene, PxRigidActor* rb, PxMaterial* baseMaterial, float radius);
		~PhysXSphereCollider();

		void ChangeShape(float radius);

	private:
		PxRigidActor* rigidBody;
		PxShape* shape;
		PxSphereGeometry sphereGeo;
		float radius;
	};
}