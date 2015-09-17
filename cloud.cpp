#include <iostream>
#include <cmath>
#include <SDL.h>

#include "cloud.h"

//constructor
Cloud::Cloud(double rToSet, double thetaToSet)
{
	timeNow = SDL_GetTicks();
	spawnCoolDown = 1000; //ms
	rVel = 0;
	thetaVel = 0;
	collisionRadius = 25;
	
	r = rToSet;
	theta = thetaToSet;
}
void Cloud::init()
{
}

void Cloud::tick(double d_time, double playerTheta)
{
	theta = playerTheta + 180;
}

bool Cloud::doSpawn(double d_time, int playerKills)
{
	spawnCoolDown = std::max(1000 - playerKills*10, 200 );
	timeNow += d_time;
	bool doSpawn = timeNow > spawnCoolDown;
	if (doSpawn)
	{
		timeNow -= spawnCoolDown;
	}
	return doSpawn;
}