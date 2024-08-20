#pragma once

#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

typedef unsigned int GLenum;

namespace MooMooEngine
{
	class Shader
	{
	public:
		Shader(const std::string& filepath);
		Shader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		~Shader();

		void Bind() const;
		void Unbind() const;

		void SetInt(const std::string& name, int value);
		void SetIntArray(const std::string& name, int* values, uint32_t count);
		void SetMat4(const std::string& name, const glm::mat4& value);
		void SetFloat4(const std::string& name, const glm::vec4& value);
		void SetFloat3(const std::string& name, const glm::vec3& value);
		void SetFloat2(const std::string& name, const glm::vec2& value);

		inline const std::string& GetName() { return m_Name; };

		void UploadUniformInt(const std::string& name, const int value);
		void UploadUniformIntArray(const std::string& name, const int* values, const uint32_t count);

		void UploadUniformFloat(const std::string& name, float value);
		void UploadUniformFloat2(const std::string& name, const glm::vec2& values);
		void UploadUniformFloat3(const std::string& name, const glm::vec3& values);
		void UploadUniformFloat4(const std::string& name, const glm::vec4& values);

		void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);
		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);

	private:
		std::string ReadFile(const std::string& filepath);
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
		void Compile(const std::unordered_map<GLenum, std::string>& shaderSources);

	private:
		uint32_t m_RendererID;
		std::string m_Name;
	};

}