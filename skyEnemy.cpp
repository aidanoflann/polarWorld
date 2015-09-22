#include <iostream>
#include <cmath>
#include <SDL.h>

#include "skyEnemy.h"

//constructor
SkyEnemy::SkyEnemy( double rToSet, double thetaToSet, double dir, double thetaVelToSet)
{
	rVel = 0.5;
	thetaVel = thetaVelToSet;
	rVelDirection = +1;
	collisionRadius = 20;
	state = Midair;
	
	r = rToSet;
	theta = thetaToSet;
	thetaVelDirection = dir;
	enemyType = "sky";
	timeExploding = 0;
	
	flyHeight = rand()%10 + 45;
}
void SkyEnemy::init()
{
}

void SkyEnemy::tick(double d_time)
{
	if (state != Exploding)
		theta = theta + thetaVelDirection*thetaVel*d_time;
	else
	{
		timeExploding += d_time;
		if (timeExploding > explodingDuration)
			markedForDeletion = 1;
	}
	if (state == Midair)
	{
		rVel = rVel + g*d_time;
		r = r + rVelDirection*rVel*d_time;
	}
	int maxFrames = (state == Exploding)? 6: 3;
	updateAnimationFrame(d_time, 100, maxFrames);
}
//same as groundenemy, but skyenemies stop falling higher up
bool SkyEnemy::collidingWithPlanet(double planetCollisionRadius)
{
	return ( state == Midair && planetCollisionRadius + flyHeight + collisionRadius > r );
}

void SkyEnemy::collideWithPlanet(double planetCollisionRadius)
{
	state = Grounded;
	rVel = 0;
	rVelDirection = 0;
	r = collisionRadius + planetCollisionRadius + flyHeight;
}