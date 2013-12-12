#include "FPSController.h"
#include "Game.h"

using namespace BGE;

FPSController::FPSController(void)
{
	position = glm::vec3(0.0f, 10.0f, 10.0f);
	worldMode = world_modes::to_parent;
	disablePitch = false;
}

bool FPSController::Initialise()
{
	UpdateFromParent();
	return GameComponent::Initialise();
}

FPSController::~FPSController(void)
{
}

void CheckOverflow( int & x )
{
	if (x == -32768)
	{
		x = - x;
	}
}

void FPSController::Update(float timeDelta)
{
	SDL_Joystick * joystick;

	SDL_JoystickEventState(SDL_ENABLE);
    joystick = SDL_JoystickOpen(0);

	Game * game = Game::Instance();

	bool onGround = false;

	const Uint8 * keyState = Game::Instance()->GetKeyState();

	float moveSpeed = speed;

	SDL_Joystick *joy;
		if (SDL_NumJoysticks() > 0) {
			// Open joystick
			joy = SDL_JoystickOpen(0);
			if (joy) 
			{
				float range = 1;

				int strafeAxis = SDL_JoystickGetAxis(joy, 0);
			
				if (glm::abs<int>(strafeAxis) > 8000)
				{
					float strafe = ((float) strafeAxis / (float) numeric_limits<short int>::max()) * range;
					Strafe(strafe);
				}


				int walkAxis = SDL_JoystickGetAxis(joy, 1);
				CheckOverflow(walkAxis);
				if (glm::abs<int>(walkAxis) > 8000)
				{
					float walk = ((float) walkAxis / (float) numeric_limits<short int>::max()) * range;
				
					Walk(-walk);
				}

				range = 3; 
				int x = SDL_JoystickGetAxis(joy, 2);	
				float yaw = ((float) x / (float) numeric_limits<short int>::max()) * range;
				Yaw((int) -yaw);

				if (!disablePitch)
				{
					int y = SDL_JoystickGetAxis(joy, 3);	
					CheckOverflow(y);
					float pitch = ((float) y / (float) numeric_limits<short int>::max()) * range;
					Pitch((int) -pitch);
				}
				
				stringstream ss;
				ss << SDL_JoystickNameForIndex(0) << " detected";
				Game::Instance()->PrintText(ss.str());

			} else {
				Game::Instance()->PrintText("Could not get controller!!");
			}

			// Close if opened
			if (SDL_JoystickGetAttached(joy)) {
				SDL_JoystickClose(joy);
			}
		}
		else
		{
			Game::Instance()->PrintText("No game controller detected");
		}

		stringstream ss;
		ss << "Gamepad Pos: " << position.x << " " << position.y << " " << position.z;
		Game::Instance()->PrintText(ss.str());

		ss.str("");
		ss << "Gamepad Look: " << look.x << " " << look.y << " " << look.z;
		Game::Instance()->PrintText(ss.str());



	if (keyState[SDL_SCANCODE_LSHIFT])
	{
		moveSpeed *= 10.0f;
	}

	//int yb = SDL_JoystickGetButton(joy, 13);
	//if(yb)
	if (keyState[SDL_SCANCODE_W] )
	{
		if(position.y <= 0.00 && look.y <= 0.0)
		{
			game->PrintText("looking down");
			moveSpeed = 0;
		}
		else
		Walk(moveSpeed * timeDelta);
	}

	if (keyState[SDL_SCANCODE_S])
	{
		if(position.y <= 0.00 && look.y <= 0.9)
		{
			moveSpeed = 0;
		}
		else
		Walk(-moveSpeed * timeDelta);
	}

	if (keyState[SDL_SCANCODE_A])
	{
		Strafe(-moveSpeed * timeDelta);
	}

	if (keyState[SDL_SCANCODE_D])
	{
		Strafe(moveSpeed * timeDelta);
	}

	int x, y;
	int midX, midY;
	SDL_GetMouseState(&x,&y);
	midX = Game::Instance()->GetWidth() / 2;
	midY = Game::Instance()->GetHeight() / 2;
	float yaw, pitch;
	yaw = midX - x;
	pitch = midY - y;

	float scale = 0.1f;
	if (yaw != 0)
	{
		Yaw(yaw * scale);
	}
	if (pitch != 0)
	{
		Pitch(pitch * scale);
	}
	SDL_WarpMouseInWindow(
		Game::Instance()->GetMainWindow()
		,midX
		,midY
		);
	GameComponent::Update(timeDelta);
	//Controller::Update(this, this->parent);
}

