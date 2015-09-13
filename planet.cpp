#include "planet.h"

void Planet::init()
{
	//default position is at (0,0) (in both xy and rtheta)
	r = 0;
	theta = 0;
	collisionRadius = 100;
	
}