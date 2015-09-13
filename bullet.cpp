#include <iostream>

#include "bullet.h"

//constructor
Bullet::Bullet( double rToSet, double thetaToSet, double dir)
{
	rVel = 0;
	thetaVel = 0.1;
	collisionRadius = 5;
	
	r = rToSet;
	theta = thetaToSet;
	thetaVelDirection = dir;
}
void Bullet::init()
{
}

void Bullet::tick(double d_time)
{
	theta = theta + thetaVelDirection*thetaVel*d_time;
}