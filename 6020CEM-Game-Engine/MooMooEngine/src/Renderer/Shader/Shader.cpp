#include "MooMooEngine_PCH.h"
#include "Shader.h"

#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <fstream>
#include <vector>

#include <filesystem>

namespace MooMooEngine
{
	static GLenum ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex") { return GL_VERTEX_SHADER; }
		if (type == "fragment" || type == "pixel") { return GL_FRAGMENT_SHADER; }

		CORE_ASSERT(false, "Unknown Shader Type");
		return 0;
	}

	Shader::Shader(const std::string& filepath)
	{
		//Load the file and read it as a string
		std::string source = Shader::ReadFile(filepath);

		auto shaderSources = Shader::PreProcess(source);

		Compile(shaderSources);

		const std::filesystem::path pathname = filepath;
		m_Name = pathname.stem().string();
	}

	Shader::Shader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
		: m_Name(name)
	{
		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSrc;
		sources[GL_FRAGMENT_SHADER] = fragmentSrc;
		Compile(sources);
	}

	Shader::~Shader()
	{
		glDeleteProgram(m_RendererID);
	}

	std::string Shader::ReadFile(const std::string& filepath)
	{
		std::string result;
		std::ifstream in(filepath, std::ios::in | std::ios::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);
			result.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());
			in.close();
		}
		else
		{
			LOG_ERROR("Could not open file {0}", filepath);
		}
		return result;
	}

	std::unordered_map<GLenum, std::string> Shader::PreProcess(const std::string& source)
	{

		std::unordered_map<GLenum, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);

		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos);
			CORE_ASSERT(eol != std::string::npos, "Syntax Error");
			size_t begin = pos + typeTokenLength + 1;
			std::string type = source.substr(begin, eol - begin);
			CORE_ASSERT(ShaderTypeFromString(type), "Invalid Shader Type Specified");

			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			pos = source.find(typeToken, nextLinePos);
			shaderSources[ShaderTypeFromString(type)] = source.substr(nextLinePos,
				pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
		}

		return shaderSources;
	}

	void Shader::Compile(const std::unordered_map<GLenum, std::string>& shaderSources)
	{

		GLuint program = glCreateProgram();
		CORE_ASSERT(shaderSources.size() <= 2, "Shader Array is not sufficiently sized for the number of shaders provided");
		std::array<GLenum, 2> glShaderIDs;
		int glShaderIDIndex = 0;
		for (auto& kv : shaderSources)
		{
			GLenum shaderType = kv.first;
			const std::string& shaderSource = kv.second;

			// Create an empty vertex shader handle
			GLuint shader = glCreateShader(shaderType);

			// Send the vertex shader source code to OpenGL
			// Note that std::string's .c_str is NULL character terminated.
			const GLchar* source = shaderSource.c_str();
			glShaderSource(shader, 1, &source, 0);

			// Compile the vertex shader
			glCompileShader(shader);

			GLint result = GL_FALSE;
			int logLength;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
			std::vector<GLchar> vertShaderError((logLength > 1) ? logLength : 1);
			glGetShaderInfoLog(shader, logLength, NULL, &vertShaderError[0]);
			std::cout << &vertShaderError[0] << std::endl;


			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				//If it is false then it means the vertex shader did not compile successfully
				//therefore there is something wrong with it that needs fixing.

				//Get the length of the error log message that has been generated
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength includes the NULL character
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				// We don't need the shader anymore.
				glDeleteShader(shader);

				CORE_ASSERT(false, "Shader Compile Error");
				LOG_ERROR("{0}", infoLog.data());

				break;
			}

			// Attach our shader to our program
			glAttachShader(program, shader);
			glShaderIDs[glShaderIDIndex++] = shader;
		}

		// Link our program
		glLinkProgram(program);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);

			// Don't leak shaders either.
			for (auto id : glShaderIDs)
			{
				glDeleteShader(id);
			}

			// Use the infoLog as you see fit.
			CORE_ASSERT(false, "Shader Linking Failure Error");
			LOG_ERROR("{0}", infoLog.data());

			// In this simple program, we'll just leave
			return;
		}

		// Always detach shaders after a successful link.
		for (auto id : glShaderIDs)
		{
			glDetachShader(program, id);
		}

		// Vertex and fragment shaders are successfully compiled.
		// Now time to link them together into a program.
		// Assign the program object.
		//The program can be run on the GPU
		m_RendererID = program;
	}

	void Shader::Bind() const
	{
		//We do this before we want to render something with this shader
		glUseProgram(m_RendererID);
	}

	void Shader::Unbind() const
	{
		glUseProgram(0);
	}

	void Shader::SetInt(const std::string& name, int value)
	{
		UploadUniformInt(name, value);
	}

	void Shader::SetIntArray(const std::string& name, int* values, uint32_t count)
	{
		UploadUniformIntArray(name, values, count);
	}

	void Shader::SetMat4(const std::string& name, const glm::mat4& value)
	{
		UploadUniformMat4(name, value);
	}

	void Shader::SetFloat4(const std::string& name, const glm::vec4& value)
	{
		UploadUniformFloat4(name, value);
	}

	void Shader::SetFloat3(const std::string& name, const glm::vec3& value)
	{
		UploadUniformFloat3(name, value);
	}

	void Shader::SetFloat2(const std::string& name, const glm::vec2& value)
	{
		UploadUniformFloat2(name, value);
	}

	void Shader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix)
	{
		//glGetUniformLocation returns an integer that represents the location
		//of a specific uniform variable within a program object
		GLint u_location = glGetUniformLocation(m_RendererID, name.c_str());

		glUniformMatrix3fv(u_location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void Shader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
	{
		//glGetUniformLocation returns an integer that represents the location
		//of a specific uniform variable within a program object
		GLint u_location = glGetUniformLocation(m_RendererID, name.c_str());

		//location = Specifies the location of the uniform variable to be modified. 
		//size = how many matrices provided
		//transpose = specifies whether to transpose the matrix as the values are loaded into the uniform variable.
		//Transposing a matrix flips it over its diagonal; that is, it switches the row and column indices of the matrix.
		//[1 2]			[1 3 5]
		//[3 4] becomes [2 4 6] or in essence, the transpose of a matrix is found by interchanging its rows into columns or columns into rows.
		//[5 6]
		glUniformMatrix4fv(u_location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void Shader::UploadUniformInt(const std::string& name, const int value)
	{
		//glGetUniformLocation returns an integer that represents the location
		//of a specific uniform variable within a program object
		GLint u_location = glGetUniformLocation(m_RendererID, name.c_str());

		glUniform1i(u_location, value);
	}

	void Shader::UploadUniformIntArray(const std::string& name, const int* values, const uint32_t count)
	{
		//glGetUniformLocation returns an integer that represents the location
		//of a specific uniform variable within a program object
		GLint u_location = glGetUniformLocation(m_RendererID, name.c_str());

		glUniform1iv(u_location, count, values);
	}

	void Shader::UploadUniformFloat(const std::string& name, float value)
	{
		//glGetUniformLocation returns an integer that represents the location
		//of a specific uniform variable within a program object
		GLint u_location = glGetUniformLocation(m_RendererID, name.c_str());

		glUniform1f(u_location, value);
	}

	void Shader::UploadUniformFloat2(const std::string& name, const glm::vec2& values)
	{
		//glGetUniformLocation returns an integer that represents the location
		//of a specific uniform variable within a program object
		GLint u_location = glGetUniformLocation(m_RendererID, name.c_str());

		glUniform2f(u_location, values.x, values.y);
	}

	void Shader::UploadUniformFloat3(const std::string& name, const glm::vec3& values)
	{
		//glGetUniformLocation returns an integer that represents the location
		//of a specific uniform variable within a program object
		GLint u_location = glGetUniformLocation(m_RendererID, name.c_str());

		glUniform3f(u_location, values.x, values.y, values.z);
	}

	void Shader::UploadUniformFloat4(const std::string& name, const glm::vec4& values)
	{
		//glGetUniformLocation returns an integer that represents the location
		//of a specific uniform variable within a program object
		GLint u_location = glGetUniformLocation(m_RendererID, name.c_str());

		glUniform4f(u_location, values.x, values.y, values.z, values.w);
	}





}