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
	//collsion flags
	leftSectionClearFlag = false;
	middleSectionClearFlag = false;
	rightSectionClearFlag = false;
	gameWon = false;
	gameWonCount=0;
}


PortalGame::~PortalGame(void)
{
}


std::shared_ptr<GameComponent> station;
//float theta = 0.0f;

bool PortalGame::Initialise() 
{
	// Initialize the joystick subsystem #


	SDL_InitSubSystem(SDL_INIT_JOYSTICK);
	//return GameComponent::Initialise();

	score = 0;
	riftEnabled = true;
	//fullscreen = true;
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
	colBox_01->tag="colBox1"; 

	shared_ptr<PhysicsController> colCyl_02 = physicsFactory->CreateCylinder(0.5,10, glm::vec3(12, 0, 0), glm::quat());

	shared_ptr<PhysicsController> colBox_02 = physicsFactory->CreateBox(1,1,1, glm::vec3(12, 10, 0), glm::quat()); 
	colBox_02->tag="colBox2"; 

	shared_ptr<PhysicsController> colCyl_03 = physicsFactory->CreateCylinder(0.5,8, glm::vec3(18, 0, 0), glm::quat()); 
	shared_ptr<PhysicsController> colBox_03 = physicsFactory->CreateBox(1,1,1, glm::vec3(18, 10, 0), glm::quat()); 
	colBox_03->tag="colBox3"; 

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
	shared_ptr<PhysicsController> backWall1 = physicsFactory->CreateWall(20.2,13,0.5, glm::vec3(12.18, 0, -6.3), glm::quat()); 
	backWall1->diffuse = glm::vec3(1,0,1);

	//*************Balls****************
	// Ball to throw at Boxes
	shared_ptr<PhysicsController> colBall_01 = physicsFactory->CreateSphere(0.8,glm::vec3(5, 0, 10),glm::quat());
	colBall_01->tag="colBall";

	shared_ptr<PhysicsController> colBall_02 = physicsFactory->CreateSphere(0.8,glm::vec3(12, 0, 10),glm::quat());
	colBall_02->tag="colBall";

	shared_ptr<PhysicsController> colBall_03 = physicsFactory->CreateSphere(0.8,glm::vec3(18, 0, 10),glm::quat());
	colBall_03->tag="colBall";

	

	//fountain particle effects
	fountain1 = make_shared<FountainEffect>(100);
	fountain1->position = glm::vec3(5,0,0);
	fountain1->alive = true;

	fountain2 = make_shared<FountainEffect>(100);
	fountain2-> position = glm::vec3(12,5,0);
	fountain2->diffuse = glm::vec3(1,0,0);
	fountain2->alive = true;

	fountain3 = make_shared<FountainEffect>(100);
	fountain3->position = glm::vec3(18,3,0);
	fountain3->alive = true;

	Attach(fountain1);
	Attach(fountain2);
	Attach(fountain3);
	


	fountainTheta = 0.0f; 
	for (int i = 0 ; i < NUM_FOUNTAINS ; i ++)
	{
		fountainTheta = ((glm::pi<float>() * 2.0f) / NUM_FOUNTAINS) * i;
		shared_ptr<FountainEffect> fountain = make_shared<FountainEffect>(500);
		if (i % 2 == 0)
		{
			fountain->diffuse = glm::vec3(1,0,0);
		}
		else
		{
			fountain->diffuse = glm::vec3(0,0,1);
		}

		fountain->position.x = - glm::cos(fountainTheta) * FOUNTAIN_RADIUS;
		fountain->position.z =  glm::sin(fountainTheta) * FOUNTAIN_RADIUS;
		fountain->position.y = FOUNTAIN_HEIGHT;
		fountains.push_back(fountain);
		Attach(fountain);
	}
	fountainTheta = 0.0f;
	
	if(gameWon == true)
	//spawn monkey
	shared_ptr<PhysicsController> monkey = physicsFactory->CreateFromModel("monkey", glm::vec3(15,20,0), glm::quat(), glm::vec3(5,5,5));


	//if((leftSectionClearFlag==true) && (middleSectionClearFlag==true) && (rightSectionClearFlag==true ))
	//{
	//	//spawn monkey
	//	shared_ptr<PhysicsController> monkey = physicsFactory->CreateFromModel("monkey", glm::vec3(15,20,0), glm::quat(), glm::vec3(5,5,5));
	//							
	//}

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

void BGE::PortalGame::Update(float timeDelta)
{
	float timeToPass = 1.0f;
	dynamicsWorld->stepSimulation(timeDelta,100);
	//station->Yaw(timeDelta * 20.0f);
	

	if((leftSectionClearFlag==true) && (middleSectionClearFlag==true) && (rightSectionClearFlag==true ) && gameWonCount<=0)
	{
		gameWonCount+=1;
		PrintText("You WIN!");
		//spawn monkey
		shared_ptr<PhysicsController> monkey = physicsFactory->CreateFromModel("monkey", glm::vec3(15,100,0), glm::quat(), glm::vec3(2,2,2));
		monkey->tag="monkey";

		//spawn ragdoll
		shared_ptr<PhysicsController> ragDoll =physicsFactory->CreateRagDoll(glm::vec3(2,0,10));
		ragDoll->tag= "ragDoll";
		
		// Create Inca Pyramid
		//position(), baseWidth, blockHeight, blockWidth, blockDepth
		physicsFactory->CreateIncaPyramid(glm::vec3(26,0,0), 3, 1.5, 1.5, 1);
								
	}

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

							//if first box is hit
							if (pcA->tag == "colBox1" && pcB->tag == "colBall")
                            {
								  PrintText("Collision between " + pcA->tag + " and " + pcB->tag);
								  fountain1->alive = false;
								  leftSectionClearFlag=true;
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

							//second box hit
								//if first box is hit
							if (pcA->tag == "colBox2" && pcB->tag == "colBall")
                            {
								  PrintText("Collision between " + pcA->tag + " and " + pcB->tag);
								  fountain2->alive = false;
								  middleSectionClearFlag=true;
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

							//third box is hit
							if (pcA->tag == "colBox3" && pcB->tag == "colBall")
                            {
								  PrintText("Collision between " + pcA->tag + " and " + pcB->tag);
								  fountain3->alive = false;
								  rightSectionClearFlag=true;
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

                        }
                }
        }


		for (int i = 0 ; i < fountains.size() ; i ++)
	{
		if (i % 2 == 0)
		{
			fountains[i]->position.y = FOUNTAIN_HEIGHT + (glm::sin(fountainTheta) * FOUNTAIN_HEIGHT);
		}
		else
		{
			fountains[i]->position.y = FOUNTAIN_HEIGHT - (glm::sin(fountainTheta) * FOUNTAIN_HEIGHT);
		}
	}
		fountain1->position.y = 5 + (glm::sin(fountainTheta) * 5);
		fountain2->position.y = 5 - (glm::sin(fountainTheta) * 5);
		fountain3->position.y = 5 + (glm::sin(fountainTheta) * 5);
	float scale = 30.0f + (glm::sin(fountainTheta) / 3.0f);
	fountainTheta += timeDelta;
	if (fountainTheta >= glm::pi<float>() * 2.0f)
	{
		fountainTheta = 0.0f;
	}

		stringstream ss;
		ss << "Score: " << score;
		Game::Instance()->PrintText(ss.str());

		
		Game::Update(timeDelta);


}

void BGE::PortalGame::Cleanup()
{
	Game::Cleanup();
}
