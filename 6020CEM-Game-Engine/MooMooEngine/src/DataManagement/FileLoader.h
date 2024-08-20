#pragma once
#include "MooMooEngine.h"

// Json reader
#include <jsoncpp/include/json.h>
#include <filesystem>
#include "Scene/Entity.h"
#include <glm/glm.hpp>


using namespace std;

// https://www.youtube.com/watch?v=zn7N7zHgCcs&list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT&index=93
namespace MooMooEngine
{
	class FileLoader
	{
	public:
		static std::string OpenFile(const char* filter);
		static std::string SaveFile(const char* filter);
		void static JSONSave(entt::registry& scene, std::string location);
		void static JSONLoad(std::string location, Ref<Scene> scene, Ref<MeshRegistry> m_MeshRegistry, Ref<Shader> shader, PhysicsLayer* m_physics);
	};
}

