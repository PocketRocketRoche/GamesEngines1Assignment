#include "PhysicsCamera.h"
#include "Game.h"
#include "PhysicsGame1.h"
#include "Box.h"
#include <iostream>
#include <sstream>
#include "Utils.h"
using namespace BGE;

PhysicsCamera::PhysicsCamera(PhysicsFactory * physicsFactory):PhysicsController()
{
	elapsed = 10000.0f;
	fireRate = 5.0f;
	pickedUp = NULL;
	hasFired = false;
	tag = "Physics Camera";
	this->physicsFactory = physicsFactory;
}


PhysicsCamera::~PhysicsCamera(void)
{
}

void PhysicsCamera::getWorldTransform(btTransform &worldTrans) const
{
	worldTrans.setOrigin(GLToBtVector(parent->position));
	worldTrans.setRotation(GLToBtQuat(parent->orientation));
}

void PhysicsCamera::setWorldTransform(const btTransform &worldTrans)
{
	// Should never get called, because this body is kinematic??
}

void PhysicsCamera::Update(float timeDelta)
{
	// Override the one in the base class, we do not want to update our world transform from the physics object
	// WHich is what the one in the base class does...

	SDL_Joystick * joystick;

	SDL_JoystickEventState(SDL_ENABLE);
    joystick = SDL_JoystickOpen(0);


	const Uint8 * keyState = Game::Instance()->GetKeyState();
	Game * game = Game::Instance();

	float moveSpeed = speed;
	float timeToPass = 1.0f / fireRate;

	string what = "Nothing";


	SDL_Joystick *joy;
	if (SDL_NumJoysticks() > 0) {
		// Open joystick
		joy = SDL_JoystickOpen(0);
		if (joy) 
		{
			int ab = SDL_JoystickGetButton(joy, 10);
			int rbb = SDL_JoystickGetButton(joy, 9);

	// Handle the gravity gun
	//if (SDL_GetMouseState(NULL, NULL) && SDL_BUTTON(3) || (ab))
	if(rbb)
	{
		float dist = 1000.0f;
		if (pickedUp == NULL)
		{		
			btVector3 rayFrom = GLToBtVector(parent->position + (parent->look * 4.0f)); // Has to be some distance in front of the camera otherwise it will collide with the camera all the time
			btVector3 rayTo = GLToBtVector(parent->position + (parent->look * dist));

			btCollisionWorld::ClosestRayResultCallback rayCallback(rayFrom, rayTo);
			physicsFactory->dynamicsWorld->rayTest(rayFrom, rayTo, rayCallback);
			
			if (rayCallback.hasHit())
			{
				pickedUp = reinterpret_cast<PhysicsController*>(rayCallback.m_collisionObject->getUserPointer());
				if (pickedUp->parent == game->GetGround().get())
				{
					pickedUp = NULL;
				}
			}
		}
		if (pickedUp != NULL && hasFired == false && pickedUp->tag == "colObject2" )
		{
			float powerfactor = 4.0f; // Higher values causes the targets moving faster to the holding point.
            float maxVel = 3.0f;      // Lower values prevent objects flying through walls.
			float holdDist = 12.0f;

            // Calculate the hold point in front of the camera
			glm::vec3 holdPos = parent->position + (parent->look * holdDist);

            glm::vec3 v = holdPos - pickedUp->position; // direction to move the Target
            v *= powerfactor; // powerfactor of the GravityGun

            if (v.length() > maxVel)
            {
                // if the correction-velocity is bigger than maximum
				v = glm::normalize(v);
                v *= maxVel; // just set correction-velocity to the maximum
            }
			
			pickedUp->rigidBody->setLinearVelocity(GLToBtVector(v));    
			pickedUp->rigidBody->activate();		
			what = pickedUp->tag;

			// Check the A button pressed
			

			if (ab && (elapsed > timeToPass))
			{
				//force of H
				float force = 1000.0f;
				pickedUp->rigidBody->applyCentralForce(GLToBtVector(parent->look) * force);
				hasFired = true;
				elapsed = 0.0f;
		
			}
			else
			{
		
				
				elapsed += timeDelta;
			}
			
		}

		}
	else {
				//Game::Instance()->PrintText("Could not get controller!!");
			//SDL_JoystickClose(joy);
		hasFired = false;
			pickedUp = NULL;
			}

			// Close if opened
			if (SDL_JoystickGetAttached(joy)) {
				SDL_JoystickClose(joy);
			}
		}
		
	}

	else
	{ 
		SDL_JoystickClose(joy);
		hasFired = false;
		pickedUp = NULL;
		
	}
	//SDL_JoystickClose(joy);
	stringstream ss;
	ss << "Picked up: " << what;
	game->PrintText(ss.str());
}

