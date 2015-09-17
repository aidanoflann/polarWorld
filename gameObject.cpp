#include "gameObject.h"

#include <iostream>
#include <cmath>

void gameObject::init()
{}

gameObject::gameObject()
{
	g = -0.002;
	markedForDeletion = 0;
}

double gameObject::getCollisionRadius() const {return collisionRadius;}
double gameObject::getR() const {return r;}
double gameObject::getTheta() const {return theta;}
void gameObject::setR(double rToSet) {r = rToSet;}
void gameObject::setTheta(double thetaToSet) {theta = thetaToSet;}
void gameObject::setRVelDirection(double dirToSet) {rVelDirection = dirToSet;}
void gameObject::setThetaVelDirection(double dirToSet) {thetaVelDirection = dirToSet;}
double gameObject::getThetaVelDirection() {return thetaVelDirection;}
void gameObject::tick( double d_time ){}
bool gameObject::collidingWithPlanet(double planetCollisionRadius){return 0;}
void gameObject::collideWithPlanet(double planetCollisionRadius){}
bool gameObject::getMarkedForDeletion(){ return markedForDeletion;}
void gameObject::MarkForDeletion(){ markedForDeletion = 1;}


bool gameObject::collidingWithGameObject(double objR, double objTheta, double objCollisionRadius)
{
	return r*r + objR*objR - 2*r*objR*cos( (objTheta - theta) * 0.01745329251) < pow(collisionRadius + objCollisionRadius, 2);
}