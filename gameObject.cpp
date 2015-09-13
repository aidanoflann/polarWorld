#include "gameObject.h"

#include <iostream>

void gameObject::init()
{
	g = -10;
}

double gameObject::getCollisionRadius() const {return collisionRadius;}
double gameObject::getR() const {return r;}
double gameObject::getTheta() const {return theta;}
void gameObject::setR(double rToSet) {r = rToSet;}
void gameObject::setTheta(double thetaToSet) {theta = thetaToSet;}
void gameObject::setRVelDirection(double dirToSet) {rVelDirection = dirToSet;}
void gameObject::setThetaVelDirection(double dirToSet) {thetaVelDirection = dirToSet;}