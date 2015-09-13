#include <iostream>
#include <cmath>
#include <SDL.h>

#include "skyEnemy.h"

//constructor
SkyEnemy::SkyEnemy( double rToSet, double thetaToSet, double dir)
{
	rVel = 0;
	thetaVel = 0.05;
	collisionRadius = 20;
	
	r = rToSet;
	theta = thetaToSet;
	thetaVelDirection = dir;
}
void SkyEnemy::init()
{
}

void SkyEnemy::tick(double d_time)
{
	theta = theta + thetaVelDirection*thetaVel*d_time;
}