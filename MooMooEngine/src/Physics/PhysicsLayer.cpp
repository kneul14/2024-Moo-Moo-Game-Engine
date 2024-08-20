#include "MooMooEngine_PCH.h"
#include "PhysicsLayer.h"


namespace MooMooEngine
{
	PxDefaultErrorCallback PhysicsLayer::gDefaultErrorCallback;
	PxDefaultAllocator PhysicsLayer::gDefaultAllocatorCallback;

	PhysicsLayer::PhysicsLayer() : Layer("Physics Layer")
	{

	}

	PhysicsLayer::~PhysicsLayer()
	{

	}

	void PhysicsLayer::OnAttach() {

		//Every PhysX module needs a PxFoundation instance avaliable.
		//(Version ID, allocator callback, error callback) 
		//Using PX_PHYSICS_VERSION allows PhysX to check for version mismatches between header files and SDK .dll files
		foundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback);
		//If we can't make a foundation for whatever reason, tell the user
		if (!foundation)
			std::cout << "Could not create foundation\n";

		//Top level PxPhysics object that we use to access functions such as creating scenes and actors
		//Tolerance values allow the correct results to be produced regardless of the scale, units of length and mass, typical speed etc.
		//We use the defaults via the empty constructor/default object for PxTolerancesScale.
		physics = PxCreatePhysics(PX_PHYSICS_VERSION, *foundation, PxTolerancesScale());

		//If we can't make a physics object for whatever reason, tell the user
		if (!physics)
			std::cout << "Could not create physics\n";

		/*
		---------------- SCENE CREATION ----------------
		*/

		//This holds information that we want the scene to have
		//We need the PxTolerancesScale to be the same so we grab it via the PxPhysics object
		PxSceneDesc sceneDesc(physics->getTolerancesScale());

		//We need to tell the scene what value we want the gravity to be applied to objects in the scene,
		//give it a CPU dispatcher and a Filter Shader (which we will use PhysX defaults for to make it easy for us)
		sceneDesc.gravity = PxVec3(0.f, -9.8f, 0.f);
		sceneDesc.cpuDispatcher = PxDefaultCpuDispatcherCreate(1); //schedules execution of tasks passed to it by the SKD, 1 thread
		sceneDesc.filterShader = PxDefaultSimulationFilterShader; //provides logic for handling collision pairs (how should potentially colliding objects be processed by the simulation - process (and how), ignore, discard)

		//Our scene where actors will be held and updated - created with the descriptor above
		//It is a collection of bodies, particle systems and constraints which can interact
		scene = physics->createScene(sceneDesc);

		cout << "Physics Subsystem is made" << endl;

		scene->setVisualizationParameter(PxVisualizationParameter::eSCALE, 1.0f);
		scene->setVisualizationParameter(PxVisualizationParameter::eCOLLISION_SHAPES, 1.0f);
	}

	void PhysicsLayer::OnDetach() {
		/*
		---------------- CLEAN UP / SHUTDOWN ----------------
		*/

		//Dispose of all PhysX objects via the release() function, destroying the object and all contained objects
		physics->release();
		foundation->release();
	}

	void PhysicsLayer::OnUpdate(Timestep ts) {
		//Advances the simulation by delta time - does some processing until physics is computed
		if (StateManager::m_SceneState == EngineState::Play)
		{
			scene->simulate(ts.GetSeconds());
			//Allows the simulation to finish and returns the results
			//true means the simulation will block until its finished (so results of the simulation step are guaranteed to be available)
			scene->fetchResults(true);	
		}
	}
}