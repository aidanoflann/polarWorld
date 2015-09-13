#include <iostream>
#include <cmath>
#include <SDL.h>

#include "cloud.h"

//constructor
Cloud::Cloud(double rToSet, double thetaToSet)
{
	rVel = 0;
	thetaVel = 0;
	collisionRadius = 25;
	
	r = rToSet;
	theta = thetaToSet;
}
void Cloud::init()
{
}

void Cloud::tick(double d_time, double playerTheta)
{
	theta = playerTheta + 180;
}