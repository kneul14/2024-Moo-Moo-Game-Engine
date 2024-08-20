#include "MooMooEngine_PCH.h"
#include "BaseCollider.h"

namespace MooMooEngine
{
	void BaseCollider::SetColliderPosition(glm::vec3 pos, glm::vec3 off)
	{
		position = pos + off;
		offset = off;
	}
}