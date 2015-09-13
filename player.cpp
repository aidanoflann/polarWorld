#include "player.h"
#include <iostream>

void Player::init()
{
	//default position is at (0,0) (in both xy and rtheta)
	r = 200;
	rVel = 0;
	rVelDirection = +1;
	theta = 270;
	thetaVel = 0.15;
	thetaVelDirection = 0;
	collisionRadius = 16;
	state = Midair;
	g = -0.002;
	
}

void Player::tick(double d_time)
{
	theta = theta + thetaVelDirection*thetaVel*d_time;
	if (state == Midair)
		rVel = rVel + g*d_time;
		r = r + rVelDirection*rVel*d_time;
}

bool Player::collidingWithPlanet(double planetCollisionRadius)
{
	return ( state == Midair && planetCollisionRadius + collisionRadius > r )? 1:0;
}

void Player::collideWithPlanet(double planetCollisionRadius)
{
	state = Grounded;
	rVel = 0;
	rVelDirection = 0;
	r = collisionRadius + planetCollisionRadius;
}

void Player::jump()
{
	if (state == Grounded)
	{
		state = Midair;
		rVel = 0.5;
		rVelDirection = +1;
	}
}