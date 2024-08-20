#pragma once
#include <glm/glm.hpp>
#include "PxPhysicsAPI.h"

namespace MooMooEngine
{
	using namespace physx;

	class BaseCollider
	{
	public:
		PxShape* shape;
		glm::vec3 offset;
		glm::vec3 position;
		glm::vec3 rotation;

		void SetColliderPosition(glm::vec3, glm::vec3);
	};
}

