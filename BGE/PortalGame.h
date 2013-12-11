#pragma once
#include "Game.h"
#include "PhysicsController.h"
#include "PhysicsFactory.h"
#include <btBulletDynamicsCommon.h>

namespace BGE
{
	class PortalGame :
		public Game
	{
	private:
		btBroadphaseInterface* broadphase;
 
		// Set up the collision configuration and dispatcher
		btDefaultCollisionConfiguration * collisionConfiguration;
		btCollisionDispatcher * dispatcher;
	
		btAlignedObjectArray<btCollisionShape*> shapes;
 
		// The actual physics solver
		btSequentialImpulseConstraintSolver * solver;

	public:
		PortalGame(void);
		~PortalGame(void);
		bool Initialise();
		void Update(float timeDelta);
		void Cleanup();
		void CreateWall();

		int score;
		
		// The world.
		std::shared_ptr<PhysicsFactory> physicsFactory;
		btDiscreteDynamicsWorld * dynamicsWorld;
	};
}


