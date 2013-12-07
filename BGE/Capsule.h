#pragma once
#include "GameComponent.h"

namespace BGE
{
	class Capsule :
		public GameComponent
	{
	public:
		Capsule(float radius, float height);
		~Capsule(void);
	};
}

