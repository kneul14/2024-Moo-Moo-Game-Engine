#pragma once

#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

#include "Renderer/Shader/Shader.h"

typedef unsigned int GLenum;

namespace MooMooEngine
{
	
	class ShaderLibrary
	{

	public:
		void Add(const Ref<Shader>& shader);
		void Add(const std::string& name, const Ref<Shader>& shader);
		Ref<Shader> Load(const std::string& filepath); //uses name of shader glsl file
		Ref<Shader> Load(const std::string& name, const std::string& filepath);

		Ref<Shader> Get(const std::string& name);

		bool Exists(const std::string& name) const;

	private:
		std::unordered_map<std::string, Ref<Shader>> m_Shaders;


	};

}