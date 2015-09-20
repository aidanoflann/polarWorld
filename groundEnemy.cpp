#include <iostream>
#include <cmath>
#include <SDL.h>

#include "groundEnemy.h"

//constructor
GroundEnemy::GroundEnemy( double rToSet, double thetaToSet, double dir)  : gameObject()
{
	rVel = 0;
	thetaVel = 0.05;
	rVelDirection = +1;
	collisionRadius = 20;
	state = Midair;
	
	r = rToSet;
	theta = thetaToSet;
	thetaVelDirection = dir;
	enemyType = "ground";
	timeExploding = 0;
}

void GroundEnemy::init()
{
}

void GroundEnemy::tick(double d_time)
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

bool GroundEnemy::collidingWithPlanet(double planetCollisionRadius)
{
	return ( state == Midair && planetCollisionRadius + collisionRadius > r )? 1:0;
}

void GroundEnemy::collideWithPlanet(double planetCollisionRadius)
{
	state = Grounded;
	rVel = 0;
	rVelDirection = 0;
	r = collisionRadius + planetCollisionRadius - 5;
}