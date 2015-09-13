#include "gameObject.h"

#include <iostream>

void gameObject::init()
{}

gameObject::gameObject()
{
	g = -0.002;
}

double gameObject::getCollisionRadius() const {return collisionRadius;}
double gameObject::getR() const {return r;}
double gameObject::getTheta() const {return theta;}
void gameObject::setR(double rToSet) {r = rToSet;}
void gameObject::setTheta(double thetaToSet) {theta = thetaToSet;}
void gameObject::setRVelDirection(double dirToSet) {rVelDirection = dirToSet;}
void gameObject::setThetaVelDirection(double dirToSet) {thetaVelDirection = dirToSet;}
void gameObject::tick( double d_time ){}
bool gameObject::collidingWithPlanet(double planetCollisionRadius){return 0;}
void gameObject::collideWithPlanet(double planetCollisionRadius){}