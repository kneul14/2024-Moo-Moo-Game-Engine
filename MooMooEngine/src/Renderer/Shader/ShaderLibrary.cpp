#include "MooMooEngine_PCH.h"
#include "ShaderLibrary.h"
#include "Shader.h"

namespace MooMooEngine
{

	void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader)
	{
		CORE_ASSERT(!Exists(name), "Shader already exists");
		m_Shaders[name] = shader;
	}

	void ShaderLibrary::Add(const Ref<Shader>& shader)
	{
		auto& name = shader->GetName();
		Add(name, shader);
	}

	//uses name of shader glsl file
	Ref<Shader> ShaderLibrary::Load(const std::string& filepath)
	{
		Ref<Shader> shader = std::make_shared<Shader>(filepath);
		Add(shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filepath)
	{
		Ref<Shader> shader = std::make_shared<Shader>(filepath);
		Add(name, shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Get(const std::string& name)
	{
		CORE_ASSERT(Exists(name), "Shader not found");
		return m_Shaders[name];
	}

	bool ShaderLibrary::Exists(const std::string& name) const
	{
		return m_Shaders.find(name) != m_Shaders.end();
	}


}