#pragma once

#include "MooMooEngine.h"
#include <conio.h>

//Include for PhysX
#include "PxConfig.h"
#include "PxPhysicsAPI.h"

#include "Core/Layer.h"
#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"
#include "Scene/StateManager.h"


//Saves us having to type it everywhere for anything PhysX related
using namespace physx;
using namespace std;

namespace MooMooEngine
{
	class PhysicsLayer : public Layer
	{
	public:
		PhysicsLayer();
		~PhysicsLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		//virtual void OnEvent(Event& event) override;
		virtual void OnUpdate(Timestep ts) override;

		/*
		---------------- PHYSX VARIABLES ----------------
		*/
		// As PhysX logs all error messages via PxErrorCallback interfaces, we need one to initialize PhysX.
		//However we can use the simple implementation of one in the PhysX Extensions Library to make it easier for us to get started
		static PxDefaultErrorCallback gDefaultErrorCallback;
		//As PhysX performs all memory allocations via PxAllocatorCallback interfaces, we need one to initialize PhysX.
		//However we can use the simple implementation of one in the PhysX Extensions Library to make it easier for us to get started
		static PxDefaultAllocator gDefaultAllocatorCallback;	

		PxFoundation* foundation;
		PxPhysics* physics;
		PxScene* scene;
	};
}