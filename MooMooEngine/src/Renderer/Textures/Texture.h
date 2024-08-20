#pragma once

#include <string>
#include "Core/Core.h"

namespace MooMooEngine
{
	class Texture
	{

	public:

		virtual ~Texture() = default;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual uint32_t GetRendererID() const = 0;

		virtual void SetData(void* data, uint32_t size) = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;

		virtual bool operator==(const Texture& other) const = 0;

		inline virtual std::string GetPath() { return m_Path; }
		inline void SetPath(std::string p) { m_Path = p; }

		inline virtual  std::string GetType() { return m_Type; }
		inline void SetType(std::string p) { m_Type = p; }

		inline uint32_t GetID() { return m_RendererID; }
		inline void SetID(uint32_t id) { m_RendererID = id; }

	protected:
		std::string m_Path;
		std::string m_Type;
		uint32_t m_RendererID;

	};

}