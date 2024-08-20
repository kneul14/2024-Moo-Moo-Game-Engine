#include "MooMooEngine_PCH.h"
#include "AudioLayer.h"

MooMooEngine::AudioLayer::AudioLayer() : Layer ("Audio Layer")
{

}

MooMooEngine::AudioLayer::~AudioLayer()
{
}

void MooMooEngine::AudioLayer::OnAttach()
{
	cout << "Audio Subsystem is made" << endl;
}

void MooMooEngine::AudioLayer::OnDetach()
{

}

void MooMooEngine::AudioLayer::OnUpdate(Timestep ts)
{
	if (currentState != StateManager::m_SceneState) {
		if (StateManager::m_SceneState == EngineState::Edit) 
		{
			PlaySound(NULL, NULL, NULL);
			currentState = StateManager::m_SceneState;
		}		
		else if (StateManager::m_SceneState == EngineState::Play) 
		{
			PlaySound(TEXT("assets/audio/cows.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
			currentState = StateManager::m_SceneState;
		}
	}
}
