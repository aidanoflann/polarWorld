#include <iostream>
#include <cmath>
#include <SDL.h>

#include "skyEnemy.h"

//constructor
SkyEnemy::SkyEnemy( double rToSet, double thetaToSet, double dir)
{
	rVel = 0;
	thetaVel = 0.05;
	rVelDirection = +1;
	collisionRadius = 20;
	state = Midair;
	
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
	if (state == Midair)
	{
		rVel = rVel + g*d_time;
		r = r + rVelDirection*rVel*d_time;
	}
}
//same as groundenemy, but skyenemies stop falling higher up
bool SkyEnemy::collidingWithPlanet(double planetCollisionRadius)
{
	return ( state == Midair && planetCollisionRadius + 60 + collisionRadius > r );
}

void SkyEnemy::collideWithPlanet(double planetCollisionRadius)
{
	state = Grounded;
	rVel = 0;
	rVelDirection = 0;
	r = collisionRadius + planetCollisionRadius + 60;
}