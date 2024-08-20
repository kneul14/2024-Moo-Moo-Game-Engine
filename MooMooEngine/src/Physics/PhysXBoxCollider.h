#include "Core/Core.h"
#include "Physics/PhysicsLayer.h"
#include "BaseCollider.h"

namespace MooMooEngine 
{
	using namespace physx;

	class PhysXBoxCollider : BaseCollider
	{
	public:
		PhysXBoxCollider() = default;
		PhysXBoxCollider(PxPhysics* physics, PxScene* scene, PxRigidActor* rb, PxMaterial* baseMaterial, glm::vec3 halfExtents);
		void RenderCollider(const glm::mat4& view, const glm::mat4& projection);
		void ChangeShape(glm::vec3 halfExtents);
		~PhysXBoxCollider();

	private:
		
		glm::vec3     extents;
		PxBoxGeometry cubeGeo;
		PxRigidActor* rigidBody;
	};
}