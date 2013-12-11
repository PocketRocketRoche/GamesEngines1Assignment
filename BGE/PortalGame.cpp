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
}


PortalGame::~PortalGame(void)
{
}


std::shared_ptr<GameComponent> station;
//float theta = 0.0f;

bool PortalGame::Initialise() 
{
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

	//non kinematic cyl
	shared_ptr<PhysicsController> colCyl = physicsFactory->CreateCylinder(2,1, glm::vec3(5, 0, -10), glm::quat()); 

	colCyl->tag="colObject1";
<<<<<<< HEAD

	//box for collision
	shared_ptr<PhysicsController> colBox = physicsFactory->CreateBox(1,1,1, glm::vec3(5, 0, 0), glm::quat()); 
	colBox->tag="colObject2"; 

	//create walls for games
	//left wall for box 1
	shared_ptr<PhysicsController> leftWall1 = physicsFactory->CreateBox(0.5,10,15, glm::vec3(-10, 0, -20), glm::quat()); 
	leftWall1->diffuse = glm::vec3(1,0,1);
	//leftWall1

	//right wall for box 1
	shared_ptr<PhysicsController> rightWall1 = physicsFactory->CreateBox(0.5,10,15, glm::vec3(-2, 0, -20), glm::quat()); 
	rightWall1->diffuse = glm::vec3(1,0,1);	

	//top wall for box 1
	shared_ptr<PhysicsController> topWall1 = physicsFactory->CreateBox(15,0.5,15, glm::vec3(-4, 0, -20), glm::quat()); 
	topWall1->diffuse = glm::vec3(1,0,1);	




=======

	//box for collision
	shared_ptr<PhysicsController> colBox = physicsFactory->CreateBox(1,1,1, glm::vec3(5, 0, 0), glm::quat()); 
	colBox->tag="colObject2"; 
>>>>>>> parent of 8bc7c50... Added in Stands and Boxes to hit

	//create walls for games
	//left wall for box 1
	shared_ptr<PhysicsController> leftWall1 = physicsFactory->CreateBox(0.5,10,15, glm::vec3(-10, 0, -20), glm::quat()); 
	leftWall1->diffuse = glm::vec3(1,0,1);
	//leftWall1

	//right wall for box 1
	shared_ptr<PhysicsController> rightWall1 = physicsFactory->CreateBox(0.5,10,15, glm::vec3(-2, 0, -20), glm::quat()); 
	rightWall1->diffuse = glm::vec3(1,0,1);	

	//top wall for box 1
	shared_ptr<PhysicsController> topWall1 = physicsFactory->CreateBox(15,0.5,15, glm::vec3(-4, 0, -20), glm::quat()); 
	topWall1->diffuse = glm::vec3(1,0,1);	





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

	camera->GetController()->position = glm::vec3(0,10, 0);


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
				// btCollisionObject* obA = (btCollisionObject*)(contactManifold->colCyl = physicsFactory->CreateCylinder(2,1, glm::vec3(5, 0, -10), glm::quat()); );
				//btCollisionObject* obB = (btCollisionObject*)(contactManifold->colBox = physicsFactory->CreateBox(1,1,1, glm::vec3(5, 0, 0), glm::quat()); );
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
							}
							/*if (pcB->tag == "colObject1" && pcA->tag == "colObject2")
                            {
								  PrintText("Collision between " + pcA->tag + " and " + pcB->tag);
							}*/
                        }
                }
        }

	Game::Update(timeDelta);


}

void BGE::PortalGame::Cleanup()
{
	Game::Cleanup();
}
