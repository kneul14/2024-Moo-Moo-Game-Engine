#include "MooMooEngine_PCH.h"
#include "MooMooEngine.h"

#include "Core/EntryPoint.h"

#include "Editor/EditorLayer.h"

namespace MooMooEngine
{
	class MooMooEngineApp : public Application
	{

	public:
		MooMooEngineApp() : Application("Moo Moo Engine Editor") {
			PushLayer(new EditorLayer());
		}

		~MooMooEngineApp() {

		}

	};

	MooMooEngine::Application* MooMooEngine::CreateApplication() {
		return new MooMooEngineApp();
	}

}