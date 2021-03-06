#include "gameObject.h"

#include <iostream>
#include <cmath>
#include <string>

void gameObject::init()
{}

gameObject::gameObject()
{
	g = -0.002;
	markedForDeletion = 0;
	animationFrame = 0;
	frameTime = 0;
	explodingDuration = 500;
}

double gameObject::getCollisionRadius() const {return collisionRadius;}
void gameObject::setCollisionRadius(double collisionRadiusToSet){collisionRadius = collisionRadiusToSet;}
double gameObject::getR() const {return r;}
double gameObject::getTheta() const {return theta;}
void gameObject::setR(double rToSet) {r = rToSet;}
void gameObject::setTheta(double thetaToSet) {theta = thetaToSet;}
void gameObject::setRVelDirection(double dirToSet) {rVelDirection = dirToSet;}
void gameObject::setThetaVelDirection(double dirToSet) {thetaVelDirection = dirToSet;}
double gameObject::getThetaVelDirection() {return thetaVelDirection;}
std::string gameObject::getEnemyType() {return enemyType;}
void gameObject::tick( double d_time ){}
bool gameObject::collidingWithPlanet(double planetCollisionRadius){return 0;}
void gameObject::collideWithPlanet(double planetCollisionRadius){}
bool gameObject::getMarkedForDeletion(){ return markedForDeletion;}
void gameObject::MarkForDeletion(){ markedForDeletion = 1;}
int gameObject::getAnimationFrame(){ return animationFrame; }
bool gameObject::isNotExploding()
{
	return (state != Exploding);
}

 //frame duration is in ms
void gameObject::updateAnimationFrame(double d_time, double frameDuration, int max)
{
	frameTime += d_time;
	if ( frameTime > frameDuration )
	{
		animationFrame = (animationFrame + 1) % max;
		frameTime -= frameDuration;
	}
}

void gameObject::startExploding()
{
	state = Exploding;
	animationFrame = 0;
}

bool gameObject::collidingWithGameObject(double objR, double objTheta, double objCollisionRadius)
{
	return r*r + objR*objR - 2*r*objR*cos( (objTheta - theta) * 0.01745329251) < pow(collisionRadius + objCollisionRadius, 2);
}
