#include <iostream>
#include <cmath>
#include <SDL.h>

#include "cloud.h"

//constructor
Cloud::Cloud(double rToSet, double thetaToSet)
{
	timeNow = 0;
	spawnCoolDown = 1000; //ms
	rVel = 0;
	thetaVel = 0.03;
	thetaVelDirection = +1;
	collisionRadius = 25;
	
	r = rToSet;
	theta = thetaToSet;
	
	thetaOffset = +0;
	thetaOffsetMax = 20;
}
void Cloud::init()
{
}

void Cloud::tick(double d_time, double playerTheta)
{
	//oscillate around playerTheta + 180.
	thetaOffset += thetaVelDirection*thetaVel*d_time;
	if ( abs(thetaOffset) > abs(thetaOffsetMax) )
	{
		thetaOffset = thetaOffsetMax;
		thetaOffsetMax *= - 1;
		thetaVelDirection *= -1;
	}
	theta = playerTheta + 180 + thetaOffset;
	
	updateAnimationFrame(d_time, 150, 5);
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