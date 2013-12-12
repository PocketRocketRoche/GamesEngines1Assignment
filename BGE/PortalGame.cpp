#include "PortalGame.h"
#include "PhysicsController.h"
#include "Sphere.h"
#include "PhysicsCamera.h"
#include "Box.h"
#include "Cylinder.h"
#include "Steerable3DController.h"
#include "Ground.h"
#include "Content.h"
#include <btBulletDynamicsCommon.h>
#include <gtc/quaternion.hpp>
#include <gtx/quaternion.hpp>
#include <gtx/euler_angles.hpp>
#include <gtx/norm.hpp>
#include "VectorDrawer.h"

using namespace BGE;


PortalGame::PortalGame(void)
{
	physicsFactory = NULL;
	dynamicsWorld = NULL;
	broadphase = NULL;
	dispatcher = NULL;
	solver = NULL;
	fullscreen = false;
	score = 0;
}


PortalGame::~PortalGame(void)
{
}


std::shared_ptr<GameComponent> station;
//float theta = 0.0f;

bool PortalGame::Initialise() 
{
	// Initialize the joystick subsystem
	SDL_InitSubSystem(SDL_INIT_JOYSTICK);
	//return GameComponent::Initialise();

	score = 0;
	riftEnabled = false;
	// Set up the collision configuration and dispatcher
    collisionConfiguration = new btDefaultCollisionConfiguration();
    dispatcher = new btCollisionDispatcher(collisionConfiguration);
 

    // The world.
	btVector3 worldMin(-1000,-1000,-1000);
	btVector3 worldMax(1000,1000,1000);
	broadphase = new btAxisSweep3(worldMin,worldMax);
	solver = new btSequentialImpulseConstraintSolver();
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,broadphase,solver,collisionConfiguration);
    dynamicsWorld->setGravity(btVector3(0,-9,0));

	width = 800;
	height = 600;

	physicsFactory = make_shared<PhysicsFactory>(dynamicsWorld);

	physicsFactory->CreateGroundPhysics();
	physicsFactory->CreateCameraPhysics();

	/*std::shared_ptr<GameComponent> box = make_shared<Box>(1, 1, 1);
	box->position = glm::vec3(0, 5, -20);
	Attach(box);*/

	//*************cyl box items****************
	// Stands for Box Objects 
	shared_ptr<PhysicsController> colCyl_01 = physicsFactory->CreateCylinder(0.5,5, glm::vec3(5, 0, 0), glm::quat()); 
	//colCyl_01->tag="colObject_01";
	shared_ptr<PhysicsController> colBox_01 = physicsFactory->CreateBox(1,1,1, glm::vec3(5, 7, 0), glm::quat()); 
	colBox_01->tag="colObject1"; 

	shared_ptr<PhysicsController> colCyl_02 = physicsFactory->CreateCylinder(0.5,10, glm::vec3(12, 0, 0), glm::quat());

	shared_ptr<PhysicsController> colBox_02 = physicsFactory->CreateBox(1,1,1, glm::vec3(12, 10, 0), glm::quat()); 
	colBox_02->tag="colObject1"; 

	shared_ptr<PhysicsController> colCyl_03 = physicsFactory->CreateCylinder(0.5,8, glm::vec3(18, 0, 0), glm::quat()); 
	shared_ptr<PhysicsController> colBox_03 = physicsFactory->CreateBox(1,1,1, glm::vec3(18, 10, 0), glm::quat()); 
	colBox_03->tag="colObject1"; 

	//*************Walls****************
	//left wall 
	shared_ptr<PhysicsController> leftWall1 = physicsFactory->CreateWall(0.47,13,15, glm::vec3(3.05, 0, 0), glm::quat()); 
	leftWall1->diffuse = glm::vec3(1,0,1);

	//middle wall 1 
	shared_ptr<PhysicsController> middleWall1 = physicsFactory->CreateWall(0.5,13,15, glm::vec3(9, 0, 0), glm::quat()); 
	leftWall1->diffuse = glm::vec3(1,0,1);

	//middle wall 2
	shared_ptr<PhysicsController> middleWall2 = physicsFactory->CreateWall(0.5,13,15, glm::vec3(15, 0, 0), glm::quat()); 
	middleWall2->diffuse = glm::vec3(1,0,1);	

	//right wall 
	shared_ptr<PhysicsController> rightWall1 = physicsFactory->CreateWall(0.5,13,15, glm::vec3(21.25, 0, 0), glm::quat()); 
	rightWall1->diffuse = glm::vec3(1,0,1);	

	//roof
	shared_ptr<PhysicsController> roof = physicsFactory->CreateWall(20,0.5,15.3, glm::vec3(12, 14, 0), glm::quat()); 
	roof->diffuse = glm::vec3(1,0,1);	

	//back wall
	shared_ptr<PhysicsController> backWall1 = physicsFactory->CreateWall(19.95,13,0.5, glm::vec3(12.45, 0, -6.3), glm::quat()); 
	backWall1->diffuse = glm::vec3(1,0,1);

	//*************Balls****************
	// Ball to throw at Boxes
	shared_ptr<PhysicsController> colBall_01 = physicsFactory->CreateSphere(0.5,glm::vec3(5, 0, 10),glm::quat());
	colBall_01->tag="colObject2";

	shared_ptr<PhysicsController> colBall_02 = physicsFactory->CreateSphere(0.5,glm::vec3(12, 0, 10),glm::quat());
	colBall_02->tag="colObject2";

	shared_ptr<PhysicsController> colBall_03 = physicsFactory->CreateSphere(0.5,glm::vec3(18, 0, 10),glm::quat());
	colBall_03->tag="colObject2";



	centFountain = make_shared<FountainEffect>(50);
	centFountain->position.x = centFountain->position.y =  centFountain->position.z = 0;
	centFountain->diffuse = glm::vec3(1,1,0);
	Attach(centFountain);
	
	

	

	//physicsFactory->CreateWall(glm::vec3(-20,0,20), 50, 10);

	
	//physicsFactory->CreateVehicle(glm::vec3(0,10,-30));

	// Create Inca Pyramid
	//position(), baseWidth, blockHeight, blockWidth, blockDepth
    //physicsFactory->CreateIncaPyramid(glm::vec3(20,0,-20), 6, 1.5, 1.5, 1.5);

	//Create Rag Doll
	//physicsFactory->CreateRagDoll(glm::vec3(25,0,-50));

	

	if (!Game::Initialise()) {
		return false;
	}

	camera->GetController()->position = glm::vec3(11,10, 33);


	return true;
}


//void CheckOverflow( int & x )
//{
//	if (x == -32768)
//	{
//		x = - x;
//	}
//}


void BGE::PortalGame::Update(float timeDelta)
{
	float timeToPass = 1.0f;
	dynamicsWorld->stepSimulation(timeDelta,100);
	//station->Yaw(timeDelta * 20.0f);

	//collision detection check
	 int numManifolds = dynamicsWorld->getDispatcher()->getNumManifolds();
        for (int i=0;i<numManifolds;i++)
        {
                btPersistentManifold* contactManifold =  dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
                btCollisionObject* obA = (btCollisionObject*)(contactManifold->getBody0());
				btCollisionObject* obB = (btCollisionObject*)(contactManifold->getBody1());
                PhysicsController * pcA = reinterpret_cast<PhysicsController*>(obA->getUserPointer());
                PhysicsController * pcB = reinterpret_cast<PhysicsController*>(obB->getUserPointer());

                int numContacts = contactManifold->getNumContacts();
                if (numContacts > 0)
                {
                        if ((pcA != nullptr) && (pcB != nullptr))
                        {
							//PrintText("Collision between " + pcA->tag + " and " + pcB->tag);
							static bool lastHit = false;
							if (pcA->tag == "colObject1" && pcB->tag == "colObject2")
                            {
								  PrintText("Collision between " + pcA->tag + " and " + pcB->tag);
								  if( ! lastHit)
								  {
									  score += 10;
								  }
								  lastHit = true;

								  
							}
							else
							{
								lastHit = false;
							}
							/*if (pcB->tag == "colObject1" && pcA->tag == "colObject2")
                            {
								  PrintText("Collision between " + pcA->tag + " and " + pcB->tag);
							}*/
                        }
                }
        }


		if ((keyState[SDL_SCANCODE_SPACE]))
		{
			//Attach Fountain here 
		}

	stringstream ss;
	ss << "Score: " << score;
	Game::Instance()->PrintText(ss.str());

		//stringstream ss;
		//ss << "Score: " << score;
		//Game::Instance()->PrintText(ss.str());


		PrintText("Score" + score);
		Game::Update(timeDelta);


}

void BGE::PortalGame::Cleanup()
{
	Game::Cleanup();
}
