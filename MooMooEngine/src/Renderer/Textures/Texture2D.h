#pragma once

#pragma once

#include <string>
#include "Renderer/Textures/Texture.h"
#include "Core/Core.h"

#include <glad/glad.h>

namespace MooMooEngine
{
	class Texture2D : public Texture
	{

	public:
		Texture2D();
		Texture2D(uint32_t width, uint32_t height);
		Texture2D(const std::string& path);
		virtual ~Texture2D();

		virtual uint32_t GetWidth() const override { return m_Width; };
		virtual uint32_t GetHeight() const override { return m_Height; }
		virtual uint32_t GetRendererID() const override { return m_RendererID; }

		unsigned int LoadTexture(const char* path);

		virtual void SetData(void* data, uint32_t size) override;

		virtual void Bind(uint32_t slot = 0) const override;

		virtual bool operator==(const Texture& other) const override
		{
			return m_RendererID == ((Texture2D&)other).m_RendererID;
		}

		inline std::string GetPath() const { return m_Path; };
		inline void SetPath(std::string p) { m_Path = p; }

		inline virtual std::string GetType() { return m_Type; }
		inline void SetType(std::string p) { m_Type = p; }

	private:
		uint32_t m_Width, m_Height;
		uint32_t m_RendererID;
		GLenum m_InternalFormat, m_DataFormat;
		std::string m_Type;
	};

}
