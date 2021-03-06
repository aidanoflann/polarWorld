#ifndef GUARD_skyEnemy
#define GUARD_skyEnemy

#include "gameObject.h"

class SkyEnemy : public gameObject
{
	//state attribute, i.e. whether groundEnemy is midair or not (maybe invulnerable etc)
	double timeExploding;
	double flyHeight;
	
public:
	void init();
	//constructor
	SkyEnemy( double, double, double, double);
	virtual void tick(double) override;
	bool collidingWithPlanet(double) override;
	void collideWithPlanet(double) override;
};

#endif