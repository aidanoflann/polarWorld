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
	double getTimeBorn();
	double getLifeTime();
	bool collidingWithPlayer(double, double, double);
	void collideWithPlayer();
	
private:
	double timeBorn;
	double lifeTime;
};

#endif