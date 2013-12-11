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

	//std::shared_ptr<GameComponent> box = make_shared<Box>(1, 1, 1);
	//box->position = glm::vec3(0, 5, -20);
	//Attach(box);

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
	//physicsFactory->CreateWall(glm::vec3(-20,0,20), 50, 10);

	 //Now some constraints
	/*shared_ptr<PhysicsController> box1 = physicsFactory->CreateBox(1,1,4, glm::vec3(5, 5, 0), glm::quat()); 
	shared_ptr<PhysicsController> box2 = physicsFactory->CreateBox(1,1,4, glm::vec3(5, 5, 5), glm::quat()); */
	//shared_ptr<PhysicsController> cap1 = physicsFactory->CreateCapsule(1,1, glm::vec3(5, 50, 5), glm::quat()); 
	//cap1->scale = glm::vec3(0.001,0.001,0.001);

	 //A hinge
	//btHingeConstraint * hinge = new btHingeConstraint(*box1->rigidBody, *box2->rigidBody, btVector3(0,0,2.5f),btVector3(0,0,-2.5f), btVector3(0,1,0), btVector3(0,1,0), true);
	//dynamicsWorld->addConstraint(hinge);

	//box1 = physicsFactory->CreateBox(1,1,4, glm::vec3(10, 5, 0), glm::quat()); 
	//box2 = physicsFactory->CreateBox(1,1,4, glm::vec3(10, 5, 5), glm::quat());
	//cap1 = physicsFactory->CreateCapsule(5,5, glm::vec3(5, 5, 5), glm::quat());






	//physicsFactory->CreateCylinder(10, 3, glm::vec3(0, 20, 0), glm::quat());

	///*std::shared_ptr<GameComponent> ship = make_shared<GameComponent>();
	//ship->ambient = glm::vec3(0.2f, 0.2, 0.2f);
	//ship->specular = glm::vec3(1.2f, 1.2f, 1.2f);
	//std::shared_ptr<Model> model = Content::LoadModel("cobramk3", glm::rotate(glm::mat4(1), 180.0f, GameComponent::basisUp));	
	//std::shared_ptr<GameComponent> steerable = make_shared<Steerable3DController>(model);
	//steerable->position = glm::vec3(20, 5, -20);
	//std::shared_ptr<VectorDrawer> vectorDrawer = make_shared<VectorDrawer>();
	//vectorDrawer->scale = glm::vec3(5,5,10);
	//ship->Attach(steerable);
	//ship->Attach(model);
	//ship->Attach(vectorDrawer);
	//Attach(ship);*/

	//// Create a hierarchy
	//station = make_shared<GameComponent>();
	//station->worldMode = world_modes::from_self;
	//station->ambient = glm::vec3(0.2f, 0.2, 0.2f);
	//station->specular = glm::vec3(0,0,0);
	//station->scale = glm::vec3(2,2,2);
	//std::shared_ptr<Model> cmodel = Content::LoadModel("coriolis", glm::rotate(glm::mat4(1), 90.0f, GameComponent::basisUp));	
	//station->Attach(cmodel);
	//station->Attach(make_shared<VectorDrawer>(glm::vec3(7,7,7)));
	//station->position = glm::vec3(40, 5, -20);
	//Attach(station);

	//// Add a child to the station and update by including the parent's world transform
	//std::shared_ptr<GameComponent> ship1 = make_shared<GameComponent>();
	//ship1->worldMode = world_modes::from_self_with_parent;
	//ship1->ambient = glm::vec3(0.2f, 0.2, 0.2f);
	//ship1->specular = glm::vec3(1.2f, 1.2f, 1.2f);
	//std::shared_ptr<Model> ana = Content::LoadModel("anaconda", glm::rotate(glm::mat4(1), 180.0f, GameComponent::basisUp));	
	//ship1->Attach(ana);
	//ship1->position = glm::vec3(0, 0, -10);
	//station->Attach(ship1);

	
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
							if (pcA->tag == "colObject1" && pcB->tag == "colObject2")
                            {
								  PrintText("Collision between " + pcA->tag + " and " + pcB->tag);
								  //score += 10;
							}
							/*if (pcB->tag == "colObject1" && pcA->tag == "colObject2")
                            {
								  PrintText("Collision between " + pcA->tag + " and " + pcB->tag);
							}*/
                        }
                }
        }

	//stringstream ss;
	//ss << "Score: " << score;
	//Game::Instance()->PrintText(ss.str());

	Game::Update(timeDelta);


}

void BGE::PortalGame::Cleanup()
{
	Game::Cleanup();
}
