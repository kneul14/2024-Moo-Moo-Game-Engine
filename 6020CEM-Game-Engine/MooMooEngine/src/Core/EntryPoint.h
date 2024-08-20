#pragma once
#include <Core/Core.h>

//This will create the Moo Moo Engine application for us

extern MooMooEngine::Application* MooMooEngine::CreateApplication();

int main(int arc, char** argv) {

	MooMooEngine::Log::Init();

	printf("Moo Moo Engine\n");

	auto app = MooMooEngine::CreateApplication();

	app->Run();

	delete app;
}