#pragma once
#include "MooMooEngine.h"

#include "Core/Layer.h"
#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"
#include "Scene/StateManager.h"
#include <Enet\include\enet\enet.h>

using namespace std;

namespace MooMooEngine
{
	class ClientLayer : public Layer
	{
	public:
		ClientLayer();
		~ClientLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		//virtual void OnEvent(Event& event) override;
		virtual void OnUpdate(Timestep ts) override;

	private:
		struct ClientData {
			int packetType = 0;
			int clientIndex;
		};

		struct ClientPacket {
			int clientIndex;
			//Vector2 position;
		};

		/* To make our lives a little easier, we declare this newPosition variable on
		the heap. It's where the data from our received packet is going to be stored,
		if it reaches us. */

		//Vector2* newPosition = new Vector2;

		//PhysicsData* serverData = new PhysicsData;
		ClientData* clientData;
		int* packetType;

		ClientPacket* clientPacket;
		ENetPacket* dataPacket;

		int clientIndex = -1;

		ENetAddress address;
		ENetHost* client;
		ENetPeer* peer;
		ENetEvent enetEvent;
	};
}