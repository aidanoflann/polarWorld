#include <iostream>
#include <cmath>
#include <SDL.h>

#include "bullet.h"

//constructor
Bullet::Bullet( double rToSet, double thetaToSet, double dir)
{
	rVel = 0;
	thetaVel = 0.2;
	collisionRadius = 5;
	
	r = rToSet;
	theta = thetaToSet;
	thetaVelDirection = dir;
	lifeTime = 3000; //ms
	
	timeBorn = SDL_GetTicks();
}
void Bullet::init()
{
}

void Bullet::tick(double d_time)
{
	theta = theta + thetaVelDirection*thetaVel*d_time;
}

bool Bullet::collidingWithPlayer(double playerR, double playerTheta, double playerCollisionRadius)
{
	return r*r + playerR*playerR - 2*r*playerR*cos( (playerTheta - theta) * 0.01745329251) < pow(collisionRadius + playerCollisionRadius, 2);
}
void Bullet::collideWithPlayer()
{
	
}

double Bullet::getTimeBorn() { return timeBorn; }
double Bullet::getLifeTime() { return lifeTime; }