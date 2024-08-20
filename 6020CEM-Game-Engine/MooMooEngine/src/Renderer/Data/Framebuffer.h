#pragma once
#include "Core/Core.h"

namespace MooMooEngine
{
	struct FramebufferSpecification
	{
		uint32_t Width, Height;
		uint32_t Samples = 1;
		bool SwapChainTarget = false;

	};

	class Framebuffer
	{
	public:

		Framebuffer(const FramebufferSpecification& spec);
		~Framebuffer();

		void Bind();
		void Unbind();

		void Invalidate();
		void Cleanup();

		void Resize(uint32_t width, uint32_t height);

		inline uint32_t GetColourAttachmentRendererID() const { return m_ColorAttachment; }
		inline const FramebufferSpecification& GetSpecification() const { return m_Specification; };

	private:
		uint32_t m_RendererID;
		uint32_t m_ColorAttachment, m_DepthAttachment;

		FramebufferSpecification m_Specification;

	};


}