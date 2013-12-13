#pragma once
#include "Game.h"
#include "PhysicsController.h"
#include "PhysicsFactory.h"
#include <btBulletDynamicsCommon.h>
#include "FountainEffect.h"

#define FOUNTAIN_HEIGHT 10.0f
#define NUM_FOUNTAINS 20
#define FOUNTAIN_RADIUS 50.0f

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
		shared_ptr<FountainEffect> fountain1;
		shared_ptr<FountainEffect> fountain2;
		shared_ptr<FountainEffect> fountain3;
		//void GravityGun(SDL_Joystick * joy, int axis);
		int score;
		bool disablePitch;
		vector<shared_ptr<FountainEffect>> fountains;
		float fountainTheta;
		
		// The world.
		std::shared_ptr<PhysicsFactory> physicsFactory;
		btDiscreteDynamicsWorld * dynamicsWorld;
	};
}


