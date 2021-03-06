#ifndef GUARD_bullet
#define GUARD_bullet

#include "gameObject.h"

class Bullet : public gameObject
{
public:
	void init();
	//constructor
	Bullet( double, double, double );
	void tick(double);
	double getLifeTime();
	double getTimeAlive();
	
private:
	double lifeTime;
	double timeAlive;
};

#endif