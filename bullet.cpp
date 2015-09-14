#include <iostream>
#include <SDL.h>

#include "bullet.h"

//constructor
Bullet::Bullet( double rToSet, double thetaToSet, double dir)
{
	rVel = 0;
	thetaVel = 0.3;
	collisionRadius = 5;
	
	r = rToSet;
	theta = thetaToSet;
	thetaVelDirection = dir;
	lifeTime = 4000; //ms
	
	timeBorn = SDL_GetTicks();
}
void Bullet::init()
{
}

void Bullet::tick(double d_time)
{
	theta = theta + thetaVelDirection*thetaVel*d_time;
}


double Bullet::getTimeBorn() { return timeBorn; }
double Bullet::getLifeTime() { return lifeTime; }